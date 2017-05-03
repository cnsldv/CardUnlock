# CardUnlock

## What's it for?

Sony added some annoying features to the Vita, one of which is that you can't swap memory cards between devices with different FW versions due to the "You must update" message. If one device runs HENkaku on 3.60 and the other is above 3.60 you can't use the card in the 3.60 device.

## How to build

Get vitasdk for your platform, clone this repo, cd to the dir and run:
```
mkdir build
cd build
cmake ..
make
```

## How to install

You need a Vita running HENkaku to be able to use CardUnlock, since you need to be able to run HENkaku without a card it needs to be a slim/TV. Install the vpk in the usual way (MolecularShell or VitaShell). Due to what it does CardUnlock is unsafe homebrew. I recommend building it from source, you should always do this where you can.

## How to use it

Start your Vita/TV without the card in, fire up HENkaku and start CardUnlock. Insert your card now and answer No to the system message asking you to restart. Press the X button and wait for it to do it's stuff. Press X again to reboot the Vita and gain access to the card or O to exit so you can remove the card to use elsewhere. Since the Vita thinks the card is new you will get questions about copying data from internal, say No to these.

## How does it work?

The Vita looks at the file id.dat in the root of the card to see if it's attached to another user/version when mounting the card. Removing this file makes the Vita think this is a new card allowing you to mount it. The problem is that you need to mount the card to remove the file and mounting ux0: causes the Vita to check the file before you can delete it. CardUnlock gets around this by mounting the card as xmc0: which is the actual mount point for external cards. The Vita doesn't check for id.dat when you mount xmc0: so we can mount the device and delete id.dat.

## Credits

The code to mount the card is based on sections of VitaShell by TheFl0w. Other stuff was taken from vitastick by Xerpi. Thanks go to Team Molecule & everyone else involved in the Vita homebrew scene, especially those who share their code to help others learn and produce more homebrew.

