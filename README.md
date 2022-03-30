# iRestore

A tool to downgrade to unsigned iOS version without its required APTicket
Only the latest signed SEP is compatible!! So please research it beforehand if the iOS version you're restoring to is compatible :)


Compiling:
```C++
make
```

All checkm8 compatible devices are supported ig?  
# DO NOT USE '-U' IF YOUR DEVICE IS JAILBROKEN!!


```Usage
 Options: 
        -f,		--ipsw         Drag and drop the ipsw file of the version you are downgrading to. This should be the same as the version you are going to specify.
        -c,		--dependencies Check the dependencies needed. Automatically installs if not found.
        -t,		--ticket       Drag and drop the APticket blob file of the device. [IOS VERSION DOES NOT MATTER!!]
	-d,		--identifier   Device identifier e.g. iPad7,5
	-i,		--version      The iOS version you are DOWNGRADING to.
	-b,		--board        Boardconfig of the device e.g. j71bap.
	-u,		--update       Just update don't wipe user data.
	-r,		--restore      Wipe everything from Nand and put new firmware on it.
	-a,		--bootargs     Specify your own iBoot boot args. For more bootargs check: https://www.theiphonewiki.com/wiki/Boot-args_(iBoot_variable)
	-p,		--pwn 	       Pwn the device and remove signature checks. This is required so first just hit this opt (or -c)
	-s,		--ramdisk      Just boot ssh ramdisk. THIS WILL NOT DOWNGRADE THE DEVICE!!
  ```

So your command should look something like:
``` example
./iRestore -f ipswfile.ipsw -t myblob.shsh2 -d iPad7,5 -i 14.5.1 -b j71bap [-u/-r] -a if you want to add more bootargs to iBEC --pwn will ask you to connect device in DFU. If it found it it will pwn and remove sighcks automatically :)) Good Luck!!
```

# No one but YOU is responsible for what happens with the device :) But there shouldn't be much of a problem but just fo sho

FDR Trippin almost there!

# Why?
Inferius was not working for me so i made my own :)  
But still. Try out inferius first i think this one is a bit messy?

# Credits
Thanks to everyone whose dependencies I'm using :))
