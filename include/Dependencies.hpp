/*
 * Dependencies.hpp
 *
 *  Created on: 22 Mar 2022
 *      Author: Dr238
 */

/* Copyright (c) 2022 Dr238 */
/* Copyright (c) 2022 Shafil Alam */

#ifndef DEPENDENCIES_HPP
#define DEPENDENCIES_HPP

namespace Check {

void Dependencies() {
  std::cout << "[i] Please enter your password if requested." << '\n';

  std::ifstream brew("/usr/local/bin/brew");
  if (!brew) {
#if defined(__APPLE__)
    std::cout << "[i] Brew not found. Installing..." << '\n';
    system("/bin/bash -c \"$(curl -fsSL "
           "https://raw.githubusercontent.com/Homebrew/install/HEAD/"
           "install.sh)\"");
#endif
#if defined(__linux__)
    std::cout << "[i] Brew not found, but is not needed." << '\n';
#endif
  }

  std::ifstream img4("/usr/local/bin/img4");
  if (!img4) {
#if defined(__APPLE__)
    std::cout << "[i] Img4 not found. Installing it for you.." << '\n';
    system("git clone https://github.com/xerub/img4lib/ --recursive");
    chdir("img4lib");
    chdir("lzfse");
    system("make");
    chdir("../");
    system("make");
    system("cp -v img4 /usr/local/bin");
    system("chmod +x /usr/local/bin/img4");
    chdir("../");
    system("rm -rf img4lib");
#endif
#if defined(__linux__)
    std::cout << "[i] Img4 not found. Installing it for you.." << '\n';
    system("git clone https://github.com/xerub/img4lib/ --recursive");
    chdir("img4lib");
    chdir("lzfse");
    system("make");
    chdir("../");
    system("make");
    system("sudo cp -v img4 /usr/local/bin");
    system("sudo chmod +x /usr/local/bin/img4");
    chdir("../");
    system("rm -rf img4lib");
#endif
  }

  std::ifstream img4tool("/usr/local/bin/img4tool");
  if (!img4tool) {
#if defined(__APPLE__)
    std::cerr << "[!] Img4tool not found. Installing it for you..." << '\n';
    system("curl -LO https://github.com/tihmstar/img4tool/releases/download/197/buildroot_macos-latest.zip");
    system("unzip buildroot_macos-latest.zip");
    chdir("buildroot_macos-latest");
    system("rsync --ignore-existing -avhuK ./usr \"/\"");
    chdir("..");
    system("chmod +x /usr/local/bin/img4tool");
    system("rm -rf buildroot_macos-latest.zip && rm -rf buildroot_macos-latest");
#endif
#if defined(__linux__)
    std::cerr << "[!] Img4tool not found. Installing it for you..." << '\n';
    system("curl -LO https://github.com/tihmstar/img4tool/releases/download/197/buildroot_ubuntu-latest.zip");
    system("unzip buildroot_ubuntu-latest.zip");
    chdir("buildroot_ubuntu-latest");
    system("sudo rsync --ignore-existing -avhuK ./usr \"/\"");
    chdir("..");
    system("sudo chmod +x /usr/local/bin/img4tool");
    system("rm -rf buildroot_ubuntu-latest.zip && rm -rf buildroot_ubuntu-latest");
#endif
  }

  std::ifstream kpatcher("/usr/local/bin/Kernel64Patcher");
  if (!kpatcher) {
#if defined(__APPLE__)
    std::cerr << "[!] Kernel64Patcher not found. Installing it for you.." << '\n';
    system("git clone https://github.com/Ralph0045/Kernel64Patcher.git");
    chdir("Kernel64Patcher");
    system("gcc Kernel64Patcher.c -o Kernel64Patcher");
    system("cp -v Kernel64Patcher /usr/local/bin");
    chdir("..");
    system("rm -rf Kernel64Patcher");
#endif
#if defined(__linux__)
    std::cerr << "[!] Kernel64Patcher not found. Installing it for you..";
    system("git clone https://github.com/Ralph0045/Kernel64Patcher.git");
    chdir("Kernel64Patcher");
	// Copy apple includes to system (mach-o)
    system("curl -LO https://opensource.apple.com/tarballs/cctools/cctools-927.0.2.tar.gz");
    system("mkdir tmp");
    system("tar -xvzf cctools-927.0.2.tar.gz -C tmp");
    system("sed -i \'s_#include_//_g\' tmp/cctools-927.0.2/include/mach-o/loader.h");
    system("sed -i -e \'s=<stdint.h>=\\n#include <stdint.h>\\ntypedef int integer_t;\\ntypedef integer_t cpu_type_t;\\ntypedef integer_t cpu_subtype_t;\\ntypedef integer_t cpu_threadtype_t;\\ntypedef int vm_prot_t;=g\' tmp/cctools-927.0.2/include/mach-o/loader.h ");
    system("sudo cp -r tmp/cctools-927.0.2/include/* /usr/local/include/");
    system("rm -rf tmp cctools-927.0.2.tar.gz");
    system("sudo gcc Kernel64Patcher.c -o Kernel64Patcher");
    system("sudo cp -v Kernel64Patcher /usr/local/bin");
    system("sudo chmod +x /usr/local/bin/Kernel64Patcher");
    chdir("..");
	// Remove apple includes from system
    system("sudo rm -rf /usr/local/include/architecture/ /usr/local/include/coff/ /usr/local/include/gnu/ /usr/local/include/llvm-c/ /usr/local/include/mach-o/ /usr/local/include/opcode/ /usr/local/include/stuff/ /usr/local/include/xar/ /usr/local/include/cbt/ /usr/local/include/elf/ /usr/local/include/i386/ /usr/local/include/mach/ /usr/local/include/i386/ /usr/local/include/mach/ /usr/local/include/Makefile /usr/local/include/standalone/ /usr/local/include/sys/");
    system("rm -rf Kernel64Patcher");
#endif
  }

  std::ifstream kairos("/usr/local/bin/kairos");
  if (!kairos) {
#if defined(__APPLE__)
    std::cerr << "[!] Kairos not found. Installing it for you.." << '\n';
    system("git clone https://github.com/dayt0n/kairos.git");
    chdir("kairos");
    system("make");
    system("cp -v kairos /usr/local/bin");
    chdir("..");
    system("rm -rf kairos");
#endif
#if defined(__linux__)
    std::cerr << "[!] Kairos not found. Installing it for you.." << '\n';
    system("git clone https://github.com/dayt0n/kairos.git");
    chdir("kairos");
	// Copy apple includes to system (mach-o)
    system("curl -LO https://opensource.apple.com/tarballs/cctools/cctools-927.0.2.tar.gz");
    system("mkdir tmp");
    system("tar -xvzf cctools-927.0.2.tar.gz -C tmp");
    system("sed -i \'s_#include_//_g\' tmp/cctools-927.0.2/include/mach-o/loader.h");
    system("sed -i -e \'s=<stdint.h>=\\n#include <stdint.h>\\ntypedef int integer_t;\\ntypedef integer_t cpu_type_t;\\ntypedef integer_t cpu_subtype_t;\\ntypedef integer_t cpu_threadtype_t;\\ntypedef int vm_prot_t;=g\' tmp/cctools-927.0.2/include/mach-o/loader.h ");
    system("sudo cp -r tmp/cctools-927.0.2/include/* /usr/local/include/");
    system("rm -rf tmp cctools-927.0.2.tar.gz");
    system("sudo make");
    system("sudo cp -v kairos /usr/local/bin");
    system("sudo chmod +x /usr/local/bin/kairos");
    chdir("..");
	// Remove apple includes from system
    system("sudo rm -rf /usr/local/include/architecture/ /usr/local/include/coff/ /usr/local/include/gnu/ /usr/local/include/llvm-c/ /usr/local/include/mach-o/ /usr/local/include/opcode/ /usr/local/include/stuff/ /usr/local/include/xar/ /usr/local/include/cbt/ /usr/local/include/elf/ /usr/local/include/i386/ /usr/local/include/mach/ /usr/local/include/i386/ /usr/local/include/mach/ /usr/local/include/Makefile /usr/local/include/standalone/ /usr/local/include/sys/");
    system("rm -rf kairos");
#endif
  }

    std::ifstream irecovery_cellar("/usr/local/Cellar/libirecovery/1.0.0/bin/irecovery");
    std::ifstream irecovery("/usr/local/bin/irecovery");
  if (!irecovery || (!irecovery_cellar && !irecovery)) {
#if defined(__APPLE__)
    std::cerr << "[!] iRecovery not found. Installing it for you.." << '\n';
    system("brew install libirecovery");
#endif
#if defined(__linux__)
    std::cerr << "[!] iRecovery not found. Installing it for you.." << '\n';
    system("curl -LO https://github.com/libimobiledevice/libirecovery/releases/download/1.0.0/libirecovery-1.0.0.tar.bz2");
    system("tar -xvf libirecovery-1.0.0.tar.bz2");
    chdir("libirecovery-1.0.0");
    system("./configure");
    system("sudo make install");
    system("sudo chmod +x /usr/local/bin/irecovery");
    chdir("..");
    system("rm -rf libirecovery-1.0.0 libirecovery-1.0.0.tar.bz2 ");
#endif
  }

  std::ifstream asr("/usr/local/bin/asr64_patcher");
  if (!asr) {
#if defined(__APPLE__)
    std::cout << "[!] asr64_patcher not found. Installing..." << '\n';
    system("git clone https://github.com/exploit3dguy/asr64_patcher.git");
    chdir("asr64_patcher");
    system("make && cp -v asr64_patcher /usr/local/bin");
    chdir("..");
    system("rm -rf asr64_patcher");
#endif
#if defined(__linux__)
    std::cout << "[!] asr64_patcher not found. Installing..." << '\n';
    system("git clone https://github.com/exploit3dguy/asr64_patcher.git");
    chdir("asr64_patcher");
    system("sed -i \'1s/^/#include <stdint.h>\\n/\' asr64_patcher.c");
    system("make");
    system("sudo cp -v asr64_patcher /usr/local/bin");
    system("sudo chmod +x /usr/local/bin/asr64_patcher");
    chdir("..");
    system("rm -rf asr64_patcher");
#endif
  }

  std::ifstream ldid("/usr/local/bin/ldid2");
  if (!ldid) {
#if defined(__APPLE__)
    std::cout << "[i] ldid not found. Installing it for you..." << '\n';
    system("brew install ldid");
#endif
#if defined(__linux__)
    std::cout << "[i] ldid not found. Installing it for you..." << '\n';
    system("git clone https://github.com/xerub/ldid");
    chdir("ldid");
    system("./make.sh");
    system("sudo cp -v ldid2 /usr/local/bin");
    system("sudo chmod +x /usr/local/bin/ldid2");
    chdir("..");
    system("rm -rf ldid");
#endif
  }

  std::ifstream python("/usr/bin/python");
  if (!python) {
#if defined(__APPLE__)
    std::cout << "[i] python not found... But wait.. That's not possible bruv. Go and downgrade to Big Sur!!" << '\n';
#endif
#if defined(__linux__)
    std::cout << "[i] Python not found, please install it using your package manager." << '\n';
#endif
  }

  std::cout << "[i] Done!" << '\n';
}

}; // namespace Check

#endif /* DEPENDENCIES_HPP */
