# alarmCal
An inclusive alarm/calendar system for Linux systems.

## Installation Instructions
The main dependency needed for alarmCal is `aplay`. To install it on Debian-based systems (e.g. Ubuntu), use `sudo apt install aplay`.

The main program is calwidget, located in the alarmCal directory. To download this folder, either clone the github repo, or use the Download button on Github to download the repo. The alarmCal directory contains some files necessary for alarmCal to function properly, so it should be put in a place where the folder will not be disturbed. From there, run the install.sh script by typing `./install.sh`. This will make the calwidget application executable, and create a Desktop application shortcut. From there, double clicking on the calwidget icon or typing `./calwidget` in a terminal will open the program. An application shortcut will be made in the current user's Desktop directory, if such a directory exists.

If there are any issues with the program, then some Qt dependencies may be necessary. Installing qtcreator will generally include all the necessary dependencies. For Debian-based systems (e.g. Ubuntu), it is `sudo apt install qtcreator`.

## Usage Notes
There are several files in the alarmCal directory that are necessary for running, so do not remove them or alarmCal may fail to run properly.

If an alarm has started to ring, then it will continue to ring even after the program has closed. If the alarm is set to repeat, then the alarm will be reset to ring at the proper time, but only if the program is re-opened.

Stopwatch (in minutes), event, and alarm functionalities are available, and clicking on any given day will report the selected day's activities on the left panel.

Also, once an alarm, event, or stopwatch is initiated, it is added to the **Selected Day's** panels, even if the alarm, event, or stopwatch is not on the selected day. This is to ensure that the activity is sucessfully created. Clicking on the **Next** button for either events or alarms will advance it to the next activity. 

The **Next** button is unlocked, so it will loop around to the beginning of the list, but the **Previous** button is locked to the beginning, so a user can find the first event of the day easily.


## Compiling from Source
The source code is available in the calwidget directory. In order to compile the code, the Qt Creator IDE is needed. For Debian-based systems (e.g. Ubuntu), it is `sudo apt install qtcreator`.

From inside the Qt Creator IDE, open the calwidget.pro to load all the C++ source and header files. Be sure to check that qt5 dependencies have been installed has well, or the code may fail to run.

## Future Updates
Currently, window size is static, so a future update will likely allow for resizing the window.

When an activity is created, it displays some of its data, such as Interval of Repeat or the Time unformatted as it was entered into the program. Clicking back and forth between another activity will change the Interval of Repeat back to seconds and the Time to be formatted. A future update will likely fully format the activity parameters before displaying.
