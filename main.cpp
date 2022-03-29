
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

std::cout << "[?] If you wish to enter custom bootargs now is your time! If you don't have any boot args just press enter '-v' is already set : ";

if(std::cin.get() == '\n'){
	system("kairos ibec.raw ibec.pwn -b \"-v\"");
}
else {
	system((std::string("kairos ibec.raw ibec.pwn -b ") + "\"-v " + bootargs + "\"").c_str());
}

system((std::string("img4 -i ibss.pwn -o ") + ipsw.iBSS + " -M IM4M -A -T ibss").c_str());
system((std::string("img4 -i ibec.pwn -o ") + ipsw.iBEC + " -M IM4M -A -T ibec").c_str());

chdir("..");
mkdir("Bootchain", S_IRWXU);
chdir((std::string("WD_Restore_") + identifier + "_" + version).c_str());

system("mv -v iBSS.* ../Bootchain && mv -v iBEC.* ../Bootchain");

std::cout << "[i] Done." << "\n\n";


std::cout << "[3] Patching amfi.." << '\n';

Parser::PatchKernel(ipsw.Kernel);
system((std::string("mv -v ") + ipsw.Kernel + " ../Bootchain").c_str());

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
	system((std::string("mv -v ramdisk.dmg ") + ipsw.RRamdisk).c_str());
	system((std::string("cp -v ") + ipsw.RRamdisk + " ipswdir").c_str());
}
else if(Update == true){
	system((std::string("mv -v ramdisk.dmg ") + ipsw.URamdisk).c_str());
	system((std::string("cp -v ") + ipsw.URamdisk + " ipswdir").c_str());
}

std::cout << "[i] Done." << "\n\n";

if(SSHRamdisk == true){
	Parser::Devicetree(board);
	system((std::string("img4 -i ") + ipsw.DeviceTree + " -o DeviceTree.img4 -M IM4M -T rdtr").c_str());
	system("mv -v DeviceTree.img4 ../Bootchain");// create if statement that looks if sshramdisk true get ramdisk trustcache
}

std::cout << "[5] Packing ipsw dir back.." << '\n';

system("zip -r ipsw.zip ipswdir");

std::cout << "[i] Done!" << '\n';
sleep(1);

std::cout << "[i] Everything should be done now.. The device is going to boot in a few seconds and after that restore." << '\n';

Info("Sending iBSS...\n");

chdir("../Bootchain");
system((std::string("irecovery -f ") + ipsw.iBSS).c_str());

Info("Done.\n\n");

Info("Sending iBEC...\n");

system((std::string("irecovery -f ") + ipsw.iBEC).c_str());

Info("Done.\n\n");

chdir((std::string("../WD_Restore_") + identifier + "_" + version).c_str());

Info("The device should now be in recovery mode with no recovery logo. The restore process will begin in a few seconds.\n");
sleep(2);

	chdir((std::string("WD_Restore_") + identifier + "_" + version).c_str());
std::cout << RED << "Going to restore the device. If you want to stop now's your chance." << RESET << '\n';
sleep(1);

system("cp -v ../../Resource/futurerestore .");

for(auto &i : cellular){
	if(identifier == i){
		Info("The device is cellular. Using latest baseband.\n");
		Info("Restoring...\n\n");
		if(Restore == true){
		system((std::string("./futurerestore -t ") + blob + " --latest-sep --latest-baseband ipsw.zip").c_str());
		Info("If everything went correctly the device should now be booting. :]\n");
		}
		else if(Update == true){
			system((std::string("./futurerestore -t ") + blob + " -u --latest-sep --latest-baseband ipsw.zip").c_str());
			Info("If everything went correctly the device should now be booting. :]\n");
		}
	}
}

Info("Restoring...\n\n");
if(Restore == true){
system((std::string("./futurerestore -t ") + blob + " --latest-sep --no-baseband ipsw.zip").c_str());
Info("Restoring...\n\n");
}
else if(Update == true){
	system((std::string("./futurerestore -t ") + blob + " -u --latest-sep --no-baseband ipsw.zip").c_str());
	Info("Restoring...\n\n");
}

return 0;
}
