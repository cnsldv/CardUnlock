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

You need a Vita running HENkaku to be able to use CardUnlock. Install the vpk in the usual way (MolecularShell or VitaShell). If you are using a model 1K Vita you should use Application Storage Manager to copy CardUnlock to ur0: since you can't have a card in ux0: when you run it - this is only known to work with HENkaku R6 and apprently does not work with the latest. Due to what it does CardUnlock is unsafe homebrew. I recommend building it from source, you should always do this where you can.

## How to use it

* start Vita without the card inserted
* run HENkaku and start CardUnlock
* insert the memory card, answer "no" to the "do you want to reboot now" message from the system
* press X to rename id.dat
* press X to reboot or O to exit (after pressing O you can remove the card to insert in another device)
* answer "no" to questions about copying content to the card from the system
* you now have access to the card

## How does it work?

The Vita looks at the file id.dat in the root of the card to see if it's attached to another user/version when mounting the card. Removing/renaming this file makes the Vita think this is a new card allowing you to mount it. The problem is that you need to mount the card to access the file and mounting ux0: causes the Vita to check the file before you can delete it. CardUnlock gets around this by mounting the card as xmc0: which is the actual mount point for external cards. The Vita doesn't check for id.dat when you mount xmc0: so we can mount the device and rename id.dat.

## Credits

The code to mount the card is based on sections of VitaShell by TheFl0w. Other stuff was taken from vitastick by Xerpi. Thanks go to Team Molecule & everyone else involved in the Vita homebrew scene, especially those who share their code to help others learn and produce more homebrew.

