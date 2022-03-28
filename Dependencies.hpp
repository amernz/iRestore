/*
 * Dependencies.hpp
 *
 *  Created on: 22 Mar 2022
 *      Author: Dr238
 */

#ifndef DEPENDENCIES_HPP
#define DEPENDENCIES_HPP


namespace Check
{

void Dependencies()
{
	 std::ifstream brew("/usr/local/bin/brew");
	    if(!brew) {
	      std::cout << "[i] Brew not found. Installing..." << '\n';
	      system("/bin/bash -c \"$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)\"");
	    }

	    std::ifstream img4("/usr/local/bin/img4");
	    if(!img4){
	      std::cout << "[i] Img4 not found. Installing it for you.." << '\n';
	      system("curl -LO https://github.com/xerub/img4lib/releases/download/1.0/img4lib-2020-10-27.tar.gz");
	      system("tar -xzf img4lib-2020-10-27.tar.gz");
	      chdir("img4lib-2020-10-27/apple");
	      system("cp -v img4 /usr/local/bin");
	      chdir("../..");
	      system("chmod +x /usr/local/bin/img4");
	      system("rm -rf img4lib-2020-10-27 && rm -rf img4lib-2020-10-27.tar.gz");
	    }
	    std::ifstream img4tool("/usr/local/bin/img4tool");
	    if(!img4tool){
	      std::cerr << "[!] Img4tool not found. Installing it for you..." << '\n';
	      system("curl -LO https://github.com/tihmstar/img4tool/releases/download/197/buildroot_macos-latest.zip");
	      system("unzip buildroot_macos-latest.zip");
	      chdir("buildroot_macos-latest");
	      system("rsync --ignore-existing -avhuK ./usr \"/\"");
	      chdir("..");
	      system("chmod +x /usr/local/bin/img4tool");
	      system("rm -rf buildroot_macos-latest.zip && rm -rf buildroot_macos-latest");
	    }
	    std::ifstream kpatcher("/usr/local/bin/Kernel64Patcher");
	    if(!kpatcher){
	      std::cerr << "[!] Kernel64Patcher not found. Installing it for you.." << '\n';
	      system("git clone https://github.com/Ralph0045/Kernel64Patcher.git");
	      chdir("Kernel64Patcher");
	      system("gcc Kernel64Patcher.c -o Kernel64Patcher");
	      system("cp -v Kernel64Patcher /usr/local/bin");
	      chdir("..");
	      system("rm -rf Kernel64Patcher");
	    }
	    std::ifstream kairos("/usr/local/bin/kairos");
	    if(!kairos){
	      std::cerr << "[!] Kairos not found. Installing it for you.." << '\n';
	      system("git clone https://github.com/dayt0n/kairos.git");
	      chdir("kairos");
	      system("make");
	      system("cp -v kairos /usr/local/bin");
	      chdir("..");
	      system("rm -rf kairos");
	    }
	    std::ifstream irecv("/usr/local/Cellar/libirecovery/1.0.0/bin/irecovery");
	    std::ifstream irecovery("/usr/local/bin/irecovery");
	    if(!irecv){
	      std::cerr << "[!] Irecovery not found. Installing it for you.." << '\n';
	      system("brew install libirecovery");
	    }
	    else if(!irecovery) {
	      system("brew install libirecovery");
	    }
	    std::ifstream asr("/usr/local/bin/asr64_patcher");
	    if(!asr){
	    	std::cout << "[!] asr64_patcher not found. Installing..." << '\n';
	    	system("git clone https://github.com/exploit3dguy/asr64_patcher.git");
	    	chdir("asr64_patcher");
	    	system("make && cp -v asr64_patcher /usr/local/bin");
	    	chdir("..");
	    	system("rm -rf asr64_patcher");
	    }
	    std::ifstream ldid("/usr/local/bin/ldid2");
	        if(!ldid) {
	          std::cout << "[i] ldid not found. Installing it for you..." << '\n';
	          system("brew install ldid");
	        }
	        std::cout << "[i] Done!" << '\n';
	        sleep(1);
	        system("clear");
}

};

#endif /* DEPENDENCIES_HPP */
