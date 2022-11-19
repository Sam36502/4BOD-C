# 4BOD-C

This is a C-reimplementation of [4BOD Fantasy Console](https://puarsliburf.itch.io/4bod-fantaly-console)
using raylib.

This was just a quick attempt to fix some of the annoying bugs
of the desktop version of the 4BOD system.

At the moment, this is a messy first build, so it will likely
be quite annoying to build on Windows and still have a few glaring bugs.
Feel free to file an issue or PR if you come across any of these, but I
do hope to make a build script for Windows and tidy up the code over the
next few day. (Though I will likely forget about it, lol)

## Controls
 - Arrow keys to move around in the menus and as input to the program
 - *[Return]* to select one of the menu items.
 - *[Backspace]* to return to the menu.
 - *'S'* to save current program to `resources/save`
 - *'L'* to load `resources/save` to current program

## Changes from original
 - I added the option to have a grid in the editor screen to make it
   easier to see where the bits are.
 - The program pages don't overlap. (the 16th row of one editor "page"
   used to show up on the next page)
 - Scrolling for the editor isn't supported at the moment.
 - The file save format is just binary where each 2 bytes is one instruction.
 - File system is just a basic debug implementation, but would be nice to have
   proper file selection at some point.
 - Forgot about the splash screen, but might get around to implementing it later.
