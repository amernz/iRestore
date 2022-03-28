# iRestore

Compiling:
```C++
make
```

All checkm8 compatible devices are supported ig?


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
	-a,		--bootargs     Specify your own iBoot boot args.
	-p,		--pwn 	       Pwn the device and remove signature checks.
	-s,		--ramdisk      Just boot ssh ramdisk. THIS WILL NOT DOWNGRADE THE DEVICE!!
  ```


# Why?
Inferius was not working for me so i made my own :)
