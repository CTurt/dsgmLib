DS Game Maker Library (dsgmLib)
===============================
dsgmLib is an object-orientated C library for Nintendo DS game development. Although it's designed specifically for [DS Game Maker](https://github.com/DSGameMaker/dsgmApp) (which generates dsgmLib code files), it's ideal for general Nintendo DS homebrew games as well.

In particular dsgmLib is a successor to [PAlib](http://sourceforge.net/projects/pands/) and an alternative to [NightFox’s Lib](http://www.nightfoxandco.com/index.php/main-en/coding/nfl-en/).

devkitARM Installation
-------------

You will need the [devkitARM](http://sourceforge.net/projects/devkitpro/files/devkitARM/) toolchain from the vendor [devkitPro](http://devkitpro.org/). You can read the instructions from the [devkitPro wiki](http://devkitpro.org/wiki/Getting_Started/devkitARM) in full or get going right away as follows.

### Windows
Download and run [devkitProUpdater-1.5.3.exe](http://sourceforge.net/projects/devkitpro/files/Automated%20Installer/devkitProUpdater-1.5.3.exe/download). Only "Minimal System" and "devkitARM" are required; install devkitPPC and devkitPSP only if you want to develop for the Nintendo Wii/Sony PSP respectively. Log out and log in again if there are compilation problems.

### OS X, Linux, etc.
Download [devkitARMupdate.pl](http://sourceforge.net/projects/devkitpro/files/Automated%20Installer/devkitARMupdate.pl/download) to your home directory (`/Users/(username)` on OS X or `/home/(username)` on Ubuntu); it's a Perl script. Open a terminal and run this:

    cd ~
    perl devkitARMupdate.pl
    export DEVKITPRO=~/devkitPro
    export DEVKITARM=~/devkitPro/devkitARM

Great! We're done.

Getting Started
---------------
[Download dsgmLib](https://github.com/DSGameMaker/dsgmLib/archive/master.zip), extract the archive, and open a terminal in the extracted directory. Run `ls` to (hopefully) print this list of files and directories:

    dsgmLib
    examples
    template
    LICENSE
    README.md

`cd` to `examples` and run `ls` to list the examples:

    Collision
    DrawableBackground
    RoomPersistency
    TopDownShooter
    CustomVariables
    Priority
    RotsetEffects

Let's compile the `Collision` example.

`cd` to `Collision` and run `make`:

    cd Collision
    make

You'll see `Collision.nds` is generated which you can run with an emulator like [NO$GBA](http://problemkaputt.de/gba.htm) (for Windows) or [DeSmuME](http://desmume.org/download/) (for Windows/OS X/Linux).

Recompiling the Library
---------
If you make changes to the library's source code (dsgmLib directory), you'll need to recompile it by running `make` in that this directory. Now recompile any projects that link against the library.

Getting Help
------------
[There's a forum](http://dsgamemaker.com/dsgmforum/viewforum.php?f=31) specifically for discussing bugs/feature requests, or [create a GitHub issue](https://github.com/DSGameMaker/dsgmLib/issues/new).
