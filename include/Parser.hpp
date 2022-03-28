/*
 * Parser.hpp
 *
 *  Created on: 22 Mar 2022
 *      Author: Dr238
 */

#ifndef PARSER_HPP
#define PARSER_HPP
#include <cstdlib>
#include <iostream>
#include <getopt.h>
#include <fstream>
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <cstring>
#include <chrono>
#include <array>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define Info(...) fprintf(stdout, "[i] " __VA_ARGS__)

int opt = 0;
const char* identifier = NULL;
const char* version = NULL;
const char* board = NULL;
const char* blob = NULL;
const char* ipswfile = NULL;
const char* ecid = NULL;
char bootargs[50];
bool Update = false;
bool Restore = false;
bool SSHRamdisk = false;

const char* cellular[] = {
		"iPad4,2",
        "iPad4,3",
        "iPad4,5",
        "iPad4,6",
        "iPad4,8",
        "iPad4,9",
        "iPad5,2",
        "iPad5,4",
        "iPad6,8",
        "iPad6,4",
        "iPad7,2",
        "iPad7,4",
        "iPad8,3",
        "iPad8,4",
        "iPad8,7",
        "iPad8,8",
        "iPad8,10",
        "iPad8,12",
        "iPad11,2",
        "iPad11,4",
        "iPad13,2"
};

struct Parsers
{
	std::string StringParse = "<string>";
	std::string StringEndParse = "</string>";
};

struct firmware
{
	std::string iBSS;
	std::string iBEC;
	std::string DeviceTree;
	std::string Kernel;
	std::string RRamdisk;
	std::string URamdisk;
	std::string rdpath;
};

struct FirmwareKeys
{
	std::string BuildID;
	std::string Page;
	std::string iBSSiv, iBSS_iv;
	std::string iBSSkey, iBSS_key;
	std::string iBECiv, iBEC_iv;
	std::string iBECkey, iBEC_key;
};

firmware ipsw;
FirmwareKeys Keys;
Parsers string;

static struct option longopts[] = {
  { "help",         no_argument      , NULL , 'h' },
  { "ipsw",         required_argument, NULL , 'f' },
  { "dependencies", no_argument      , NULL , 'c' },
  { "identifier",   required_argument, NULL , 'd' },
  { "version",      required_argument, NULL , 'i' },
  { "board",        required_argument, NULL , 'b' },
  { "ticket",       required_argument, NULL , 't' },
  { "ecid",         required_argument, NULL , 'e' }, // ecid isn't working or sum shi
  { "update",	    required_argument, NULL , 'u' },
  { "restore", 	    required_argument, NULL , 'r' },
  { "pwn",          no_argument      , NULL , 'p' },
  { "ramdisk", 	    no_argument      , NULL , 's' },
  { NULL, 0, NULL, 0 }
};

namespace Parser {

void PrintHelp()
{
	std::cout << "Options:" << '\n';
	std::cout << "	-h,		--help\t\t Prints this help message" << '\n';
	std::cout << "	-f,		--ipsw\t\t Drag and drop the ipsw file of the version you are downgrading to. This should be the same as the version you are going to specify" << '\n';
	std::cout << "	-c,		--dependencies\t Check the dependencies needed. Automatically installs if not found" << '\n';
	std::cout << "	-t,		--ticket\t Drag and drop the APticket blob file of the device. [IOS VERSION DOES NOT MATTER!!]" << '\n';
	std::cout << "	-d,		--identifier\t Device identifier e.g. iPad7,5" << '\n';
	std::cout << "	-i,		--version\t The iOS version you are DOWNGRADING to" << '\n';
	std::cout << "	-b,		--board\t\t Boardconfig of the device e.g. j71bap" << '\n';
	std::cout << "	-u,		--update\t Just update don't wipe user data" << '\n';
	std::cout << "	-r,		--restore\t Wipe everything from Nand and put new firmware on it" << '\n';
	std::cout << "	-a,		--bootargs\t Specify your own iBoot boot args" << '\n';
	std::cout << "	-p,		--pwn\t\t Pwn the device and remove signature checks" << '\n';
	std::cout << "	-s,		--ramdisk\t Just boot ssh ramdisk. THIS WILL NOT DOWNGRADE THE DEVICE!!" << '\n';
}

std::string KernelCache(std::string Board)
{

chdir((std::string("WD_Restore_") + identifier + "_" + version).c_str());
std::string Krn;
std::ifstream Manifest("BuildManifest.plist");

while(getline(Manifest, Krn))
{
	if(Krn.find(Board) != std::string::npos){
      for(int i = 0; i < 4000 && getline(Manifest, Krn); i++){
	    if(Krn.find("kernelcache.") != std::string::npos){
		  Manifest.close();
		    Krn.erase(0, 14); // removes '<string>'
			  auto endstr = Krn.find(string.StringEndParse);
			    Krn.erase(endstr, Krn.length());  // removes '</string>'
			        ipsw.Kernel = Krn;
			}
		}
	}
}
system((std::string("cp -v ipswdir/") + ipsw.Kernel + " .").c_str());
return ipsw.Kernel;
}

std::string Devicetree(std::string board)
{

chdir((std::string("WD_Restore_") + identifier + "_" + version).c_str());
std::string Devtre;
std::ifstream Manifest("BuildManifest.plist");

while(getline(Manifest, Devtre))
{
	if(Devtre.find(board) != std::string::npos){
	  for(int i = 0; i < 4000 && getline(Manifest, Devtre); i++){
	    if(Devtre.find("DeviceTree.") != std::string::npos){
	      Manifest.close();
	        Devtre.erase(0, 33);
	    	  auto estr = Devtre.find(string.StringEndParse);
	    	    Devtre.erase(estr, Devtre.length());
	    	      ipsw.DeviceTree = Devtre;
	    	}
	    }
	}
}
system((std::string("cp -v ipswdir/Firmware/all_flash/") + ipsw.DeviceTree + " .").c_str());
return ipsw.DeviceTree;
}

int Manifest(std::string file)
{
mkdir((std::string("WD_Restore_") + identifier + "_" + version).c_str(), S_IRWXU);
chdir((std::string("WD_Restore_") + identifier + "_" + version).c_str());

std::ifstream BManifest("BuildManifest.plist");
if(BManifest){
	std::cout << "ipswdir already exists. Not extracting again.." << '\n';
	return 0;
}

system((std::string("curl -s -LO \"https://api.ipsw.me/v2.1/") + identifier + "/" + version + "/info.json\"").c_str());

system((std::string("unzip ") + file + " -d ipswdir").c_str());
system("clear");
system("cp -v ipswdir/BuildManifest.plist .");

return 0;

}

void PatchKernel(std::string Kernelcache)
{
	system((std::string("img4 -i ") + Kernelcache + " -o kernel.raw").c_str());
	system((std::string("rm ") + Kernelcache).c_str());
	system("Kernel64Patcher kernel.raw kernel.patched -a");
	system("img4tool -c kernel.im4p -t rkrn kernel.patched --compression complzss");
	system((std::string("img4tool -c ") + Kernelcache + " -p kernel.im4p").c_str());
	system((std::string("img4 -i ") + Kernelcache + " -M IM4M -T rkrn").c_str());

}

std::string GetRestoreRamdisk()
{
std::string rdisk;
chdir((std::string("WD_Restore_") + identifier + "_" + version).c_str());

std::ifstream keys("keys.html");
 if(!keys) {
   std::cout << "[e] It seems the script could not get the firmware keys. This is common on macs lower than catalina. Sorry but please update your mac to catalina or higher (Or you specified your device with lowercase p :) )" << '\n';
   exit(1);
 }
 while(getline(keys, rdisk)) {
  if(rdisk.find("keypage-restoreramdisk") != std::string::npos) {
   for(int i = 0; i < rdisk.size(); i++) {
    if(rdisk[i] == '0') {
     rdisk.erase(0, i);
      for(int o = 0; o < rdisk.size(); o++) {
       if(rdisk[o] == '<') {
        rdisk.erase(o);
          ipsw.RRamdisk = rdisk;
      }
     }
    }
   }
  }
 }
system((std::string("cp -v ipswdir/") + ipsw.RRamdisk + " .").c_str());
return ipsw.RRamdisk;
}
// keypage-updateramdisk

std::string GetUpdateRamdisk()
{
	std::string urdisk;
	chdir((std::string("WD_Restore_") + identifier + "_" + version).c_str());

	std::ifstream keys("keys.html");
	 if(!keys) {
	   std::cout << "[e] It seems the script could not get the firmware keys. This is common on macs lower than catalina. Sorry but please update your mac to catalina or higher (Or you specified your device with lowercase p :) )" << '\n';
	   exit(1);
	 }
	 while(getline(keys, urdisk)) {
	  if(urdisk.find("keypage-updateramdisk") != std::string::npos) {
	   for(int i = 0; i < urdisk.size(); i++) {
	    if(urdisk[i] == '0') {
	     urdisk.erase(0, i);
	      for(int o = 0; o < urdisk.size(); o++) {
	       if(urdisk[o] == '<') {
	        urdisk.erase(o);
	          ipsw.URamdisk = urdisk;
	      }
	     }
	    }
	   }
	  }
	 }
	system((std::string("cp -v ipswdir/") + ipsw.URamdisk + " .").c_str());
	return ipsw.URamdisk;
}

std::string buildid()
{

std::string bldid;
chdir((std::string("WD_Restore_") + identifier + "_" + version).c_str());

	 std::ifstream json("info.json");
	  while(getline(json, bldid)){
	   if(bldid.find("\"buildid\": \"") != std::string::npos){
	    bldid.erase(0, 16);
	     for(int b = 0; b < bldid.size(); b++){
	      if(bldid[b] == '"'){
	       bldid.erase(b);
	       Keys.BuildID = bldid;
	     }
	    }
	   }
	  }
return Keys.BuildID;
}

void FirmwareKeysPage(std::string identifier, std::string version)
{
chdir((std::string("WD_Restore_") + identifier + "_" + version).c_str());

	for(int r = 0; r < version.size(); r++){
	 if(version[r] == '.'){
	  version.erase(r);
	   system((std::string("curl -s -LO \"https://www.theiphonewiki.com/wiki/Firmware_Keys/") + version + ".x" + "\"").c_str());
	    std::ifstream x((version + ".x"));
	     while(getline(x, Keys.Page)){
	      if(Keys.Page.find(identifier) != std::string::npos){
	       if(Keys.Page.find(buildid()) != std::string::npos){
	        x.close();
	         for(int w = 0; w < Keys.Page.size(); w++){
	          if(Keys.Page[w] == '/'){
	           Keys.Page.erase(0, w);
	            for(int p = 0; p < Keys.Page.size(); p++){
	             if(Keys.Page[p] == '"'){
	              Keys.Page.erase(p);
	               system((std::string("curl -s \"https://www.theiphonewiki.com") + Keys.Page + "\" -o keys.html").c_str());
	     }
	    }
	   }
	  }
	 }
	}
   }
  }
 }

}

std::string iBSS(std::string board){

chdir((std::string("WD_Restore_") + identifier + "_" + version).c_str());
std::string GetiBSSName;

std::ifstream Manifest("BuildManifest.plist");
 while(getline(Manifest, GetiBSSName)) {
  if(GetiBSSName.find(board) != std::string::npos) {
   for(int i = 0; i < 4000 && getline(Manifest, GetiBSSName); i++) {
    if(GetiBSSName.find("iBSS.")!= std::string::npos) {
     Manifest.close();
      GetiBSSName.erase(0, 14);
      auto find = GetiBSSName.find(string.StringEndParse);
       GetiBSSName.erase(find, GetiBSSName.length());
        GetiBSSName.erase(0, 13);
         ipsw.iBSS = GetiBSSName;
      }
     }
    }
   }
system((std::string("cp -v ipswdir/Firmware/dfu/") + ipsw.iBSS + " .").c_str());
return ipsw.iBSS;
 }

std::string iBEC(std::string board){

chdir((std::string("WD_Restore_") + identifier + "_" + version).c_str());
std::string GetiBECName;

std::ifstream Manifest("BuildManifest.plist");
 while (getline(Manifest, GetiBECName)) {
  if(GetiBECName.find(board) != std::string::npos) {
   for(int t = 0; t < 4000 && getline(Manifest, GetiBECName); t++) {
    if(GetiBECName.find("iBEC.") != std::string::npos){
     Manifest.close();
      GetiBECName.erase(0, 14);
       auto find = GetiBECName.find(string.StringEndParse);
        GetiBECName.erase(find, GetiBECName.length());
         GetiBECName.erase(0, 13);
          ipsw.iBEC = GetiBECName;
       }
      }
     }
    }
system((std::string("cp -v ipswdir/Firmware/dfu/") + ipsw.iBEC + " .").c_str());
return ipsw.iBEC;
 }



std::string iBSSIV(std::string board) {
std::string IV;
chdir((std::string("WD_Restore_") + identifier + "_" + version).c_str());

 std::ifstream keys("keys.html");
  while(getline(keys, IV)) {
   if(IV.find(ipsw.iBSS) != std::string::npos) {
    for(int i = 0; i < 2 && getline(keys, IV); i++) {
     if(IV.find("-iv") != std::string::npos){
      for(int o = 0; o < IV.size(); o++) {
       if(IV[o] == '"') {
        IV.erase(0, o);
         for(int u = 0; u < IV.size(); u++) {
          if(IV[u] == '>') {
           IV.erase(0, u); IV.erase(0, 1);
            for(int y = 0; y < IV.size(); y++) {
             if(IV[y] == '<') {
              IV.erase(y); Keys.iBSS_iv = IV;
           }
          }
         }
        }
       }
      }
     }
    }
   }
  }
return Keys.iBSS_iv;
}

std::string iBSSIv() {
std::string iv;
chdir((std::string("WD_Restore_") + identifier + "_" + version).c_str());

 std::ifstream key("keys.html");
  while(getline(key, iv)) { // stores everything from instream to std::string
   if(iv.find("| file   = iBSS") != std::string::npos) {
    for(int i = 0; i < 3 && getline(key, iv); i++) {
     if(iv.find("iv") != std::string::npos) {
      iv.erase(0, 11);
       Keys.iBSSiv = iv;
     }
    }
   }
  }
  return Keys.iBSSiv;
}

std::string iBSSKey() {
std::string Key;
chdir((std::string("WD_Restore_") + identifier + "_" + version).c_str());

 std::ifstream key("keys.html");
  while(getline(key, Key)) { // stores everything from instream to std::string
   if(Key.find("| file   = iBSS") != std::string::npos) {
    for(int i = 0; i < 3 && getline(key, Key); ++i) {
     if(Key.find("key") != std::string::npos) {
      Key.erase(0, 11);
       Keys.iBSS_key = Key;
     }
    }
   }
  else {
    return Keys.iBSS_key;
  }
 }
  return Keys.iBSS_key;
}

std::string iBSSKEY() {
std::string key;
chdir((std::string("WD_Restore_") + identifier + "_" + version).c_str());

chdir((std::string("WD_") + identifier + "_" + version).c_str());
 std::ifstream keys("keys.html");
  while(getline(keys, key)) {
   if(key.find(ipsw.iBSS) != std::string::npos) {
    for(int i = 0; i < 3 && getline(keys, key); i++) {
     if(key.find("-key") != std::string::npos){
      for(int i = 0; i < key.size(); i++) {
       if(key[i] == '"') {
        key.erase(0, i);
         for(int u = 0; u < key.size(); u++) {
          if(key[u] == '>') {
           key.erase(0, u); key.erase(0, 1);
            for(int y = 0; y < key.size(); y++) {
             if(key[y] == '<') {
              key.erase(y); Keys.iBSSkey = key;
           }
          }
         }
        }
       }
      }
     }
    }
   }
  }
return Keys.iBSSkey;
}

std::string iBECIV() {
std::string Iv;
chdir((std::string("WD_Restore_") + identifier + "_" + version).c_str());

 std::ifstream keys("keys.html");
  while(getline(keys, Iv)) {
   if(Iv.find(ipsw.iBEC) != std::string::npos) {
    for(int i = 0; i < 3 && getline(keys, Iv); i++) {
     if(Iv.find("-iv") != std::string::npos){
      for(int i = 0; i < Iv.size(); i++) {
       if(Iv[i] == '"') {
        Iv.erase(0, i);
         for(int u = 0; u < Iv.size(); u++) {
          if(Iv[u] == '>') {
           Iv.erase(0, u); Iv.erase(0, 1);
            for(int y = 0; y < Iv.size(); y++) {
             if(Iv[y] == '<') {
              Iv.erase(y); Keys.iBEC_iv = Iv;
           }
          }
         }
        }
       }
      }
     }
    }
   }
  }
return Keys.iBEC_iv;
}

std::string iBECKEY() {
std::string KEY;
chdir((std::string("WD_Restore_") + identifier + "_" + version).c_str());

 std::ifstream keys("keys.html");
  while(getline(keys, KEY)) {
   if(KEY.find(ipsw.iBEC) != std::string::npos) {
    for(int i = 0; i < 3 && getline(keys, KEY); i++) {
     if(KEY.find("-key") != std::string::npos){
      for(int i = 0; i < KEY.size(); i++) {
       if(KEY[i] == '"') {
        KEY.erase(0, i);
         for(int u = 0; u < KEY.size(); u++) {
          if(KEY[u] == '>') {
           KEY.erase(0, u); KEY.erase(0, 1);
            for(int y = 0; y < KEY.size(); y++) {
             if(KEY[y] == '<') {
              KEY.erase(y); Keys.iBECkey = KEY;
           }
          }
         }
        }
       }
      }
     }
    }
   }
  }
return Keys.iBECkey;
}

std::string tolower(std::string Boardid){
 std::for_each(Boardid.begin(), Boardid.end(), [](char & conv) {
   conv = ::tolower(conv);
 });
 return Boardid;
}

void mountrd() {

  FILE* rd = popen("hdiutil attach ramdisk.dmg", "r");
  char path[200];

  while(fgets(path, sizeof(path)-22, rd) != NULL) {
   ipsw.rdpath = path;
    pclose(rd);
     ipsw.rdpath.erase(std::remove(ipsw.rdpath.begin(), ipsw.rdpath.end(), '\n'), ipsw.rdpath.end());
      ipsw.rdpath.erase(0, 15);
       ipsw.rdpath.erase(0, ' ');
        ipsw.rdpath.erase(0, 6);

  }
}

void GetnPatchasr(std::string RamdiskPath)
{
	system((std::string("cp -v ") + RamdiskPath + "/usr/sbin/asr .").c_str());
	system("asr64_patcher asr asr_patched");
	system("ldid2 -e asr > ents.plist");
	system("ldid2 -Sents.plist asr_patched");
	system("rm asr");
	system("mv -v asr_patched asr");
	system("chmod 100755 asr");
	system((std::string("mv -v asr ") + RamdiskPath + "/usr/sbin/asr").c_str());
}

void Detachrd(std::string path)
{
	system((std::string("hdiutil detach ") + path).c_str());
}

};


#endif /* PARSER_HPP */
