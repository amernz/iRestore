/* Copyright (c) 2022 Dr238 */

#include "include/Parser.hpp"
#include "include/Dependencies.hpp"
#include "include/Pwn.hpp"

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		std::cout << "Usage: " << argv[0] << " -d iPad7,5 -i 14.6 -b j71bap -s [-r/-u] (-c/-p/-s)" << '\n';
		return -1;
	}
	while((opt = getopt_long(argc, (char * const *)argv, "hf:t:d:i:b:urpsc", longopts, NULL)) > 0)
	{
		switch(opt)
		{
		case 'f':
			ipswfile = optarg;
			break;
		case 't':
			blob = optarg;
			break;
		case 'd':
			identifier = optarg;
			break;
		case 'i':
			version = optarg;
			break;
		case 'b':
			board = optarg;
			break;
		case 'r':
			Restore = true;
			break;
		case 'u':
			Update = true;
			break;
		case 'c':
			Check::Dependencies();
			return 0;
		case 'p':
			Pwn::Device(VendorID, ProductID);
			return 0;
		case 's':
			std::cerr << "Coming soon.." << '\n';
			return -1;
		case 'h':
		default:
			Parser::PrintHelp();
			return -1;
		}
	}

	//std::cout << ipswfile << " " << blob << " " << identifier << " " << version << " " << board << " " << bootargs << " " << Restore << " " << Update;

std::cout << "[1] Getting all components from ipsw..." << '\n';
Parser::Manifest(ipswfile);
Parser::FirmwareKeysPage(identifier, version);
Parser::iBSS(Parser::tolower(board));
Parser::iBEC(Parser::tolower(board));
Parser::KernelCache(Parser::tolower(board));
Parser::GetRestoreLogo();
if(Update == true){
	Parser::GetUpdateRamdisk();
}
else if(Restore == true){
	Parser::GetRestoreRamdisk();
}

std::cout << "[i] Got all the components." << "\n\n";


std::cout << "[2] Converting ticket to IM4M format.." << '\n';

system((std::string("img4tool -e -s ") + blob + " -m IM4M").c_str());

std::cout << "[i] Done." << "\n\n";

system("pwd");
std::cout << "[i] Patching iBoot..." << '\n';
system((std::string("img4 -i ") + ipsw.iBSS + " -o ibss.raw -k " + Parser::iBSSIV(board) + Parser::iBSSIv() + Parser::iBSSKEY() + Parser::iBSSKey()).c_str());
system((std::string("img4 -i ") + ipsw.iBEC + " -o ibec.raw -k " + Parser::iBECIV()+ Parser::iBECKEY()).c_str());

system("kairos ibss.raw ibss.pwn");

system((std::string("rm ") + ipsw.iBSS + " && rm " + ipsw.iBEC).c_str());

std::cout << "[?] If you wish to enter custom bootargs now is your time! If you don't have any boot args just press enter '-progress -restore rd=md0 -v' is already set : ";

if(std::cin.get() == '\n'){
	system("kairos ibec.raw ibec.pwn -b \"-progress -restore rd=md0\"");
}
else {
	system((std::string("kairos ibec.raw ibec.pwn -b ") + "\"-progress -restore rd=md0 -v " + bootargs + "\"").c_str());
}

system((std::string("img4 -i ibss.pwn -o ") + ipsw.iBSS + " -M IM4M -A -T ibss").c_str());
system((std::string("img4 -i ibec.pwn -o ") + ipsw.iBEC + " -M IM4M -A -T ibec").c_str());

chdir((std::string("WD_Restore_") + identifier + "_" + version).c_str());

system((std::string("cp -v ") + ipsw.iBSS + " ipswdir/Firmware/dfu").c_str());
system((std::string("cp -v ") + ipsw.iBEC + " ipswdir/Firmware/dfu").c_str());

std::cout << "[i] Done." << "\n\n";


std::cout << "[3] Patching amfi.." << '\n';

Parser::PatchKernel(ipsw.Kernel);
system((std::string("cp -v ") + ipsw.Kernel + " ipswdir/").c_str());
std::cout << "[i] Done." << "\n\n";

std::cout << "[4] Getting and patching asr.." << '\n';

if(Restore == true){
	system((std::string("img4 -i ") + ipsw.RRamdisk + " -o ramdisk.dmg").c_str());
}
else if(Update == true){
	system((std::string("img4 -i ") + ipsw.URamdisk + " -o ramdisk.dmg").c_str());
}
system("hdiutil resize -size 160mb ramdisk.dmg");
Parser::mountrd();
Parser::GetnPatchasr(ipsw.rdpath);
Parser::Detachrd(ipsw.rdpath);

if(Restore == true){
	system((std::string("img4 -i ramdisk.dmg -o ") + ipsw.RRamdisk + " -M IM4M -A -T rdsk").c_str());
	system((std::string("cp -v ") + ipsw.RRamdisk + " ipswdir").c_str());
	chdir("ipswdir/Firmware");
	system((std::string("img4 -i ") + ipsw.RRamdisk + ".trustcache " + "-o " + ipsw.RRamdisk + ".trustcache -M ../../IM4M -T rtsc").c_str());
	chdir("../..");
}
else if(Update == true){
	system((std::string("img4 -i ramdisk.dmg -o ") + ipsw.URamdisk + " -M IM4M -A -T rdsk").c_str());
	system((std::string("cp -v ") + ipsw.URamdisk + " ipswdir").c_str());
	chdir("ipswdir/Firmware");
	system((std::string("img4 -i ") + ipsw.URamdisk + ".trustcache " + "-o " + ipsw.URamdisk + ".trustcache -M ../../IM4M -T rtsc").c_str());
	chdir("../..");
}

std::cout << "[i] Done." << "\n\n";

std::cout << "[5] Signing RestoreLogo with ticket..." << '\n';

chdir("ipswdir/Firmware/all_flash");
system((std::string("img4 -i ") + ipsw.RestoreLogo + " -o " + ipsw.RestoreLogo + " -M ../../../IM4M -T logo").c_str());
system("pwd");
chdir("../../..");

std::cout << "[6] Signing DeviceTree with ticket..." << '\n';

Parser::Devicetree(board);
system((std::string("img4 -i ") + ipsw.DeviceTree + " -o " + ipsw.DeviceTree + " -M IM4M -T rdtr").c_str());
system((std::string("cp -v ") + ipsw.DeviceTree + " ipswdir/Firmware/all_flash/").c_str());

std::cout << "[i] Done." << '\n';
	
// have to use idevicerestore futurerestore was trippin
return 0;
}

