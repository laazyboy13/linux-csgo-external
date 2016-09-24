linux-csgo-external
===================

Linux cheat for x64 linux systems

Features:
* *Wallhack*
* *Trigger bot*
* *Bhop*
* *No Flash*

## Wall hack
![Walls png](http://i.imgur.com/ztp5WBf.jpg)

## Bhop
![Bhops gif](http://i.imgur.com/0DXp8bA.gif)

## Toggles
| Hack          | Key           |
| ------------- |:-------------:|
| ESP           | F7            |
| No Flash      | F8            |
| Bhop          | SPACE         |
| Bhop lock     | F9            |


## TriggerKey
Bind your own key to alt1:
bind yourkey +alt1

### Compiling & Running the hack
* The hack requires the Xlib libraries, install them on Debian based system with the command `sudo apt-get install libx11-dev libx11-dev:i386 libxtst-dev libconfig-dev`
* Make sure you're inside of the `linux-csgo-external` directory (`cd linux-csgo-external`)
* Now, compile the hack by entering the command `make`
* You can now run the hack ***before or after*** *csgo* is launched by running the hack as **root** `sudo ./csgo_external`

## Tested Distributions
- Ubuntu
- Xubuntu
- Archlinux
- Debian (Jessie + Sid)

## Known Bugs
* Trigger doesn't work at far distance

## Credits:
- Original author: s0beit
- Original author credits: BoToX & buFFy
- Maintainer/Updater: laazyboy13, owerosu, McSwaggens & community
