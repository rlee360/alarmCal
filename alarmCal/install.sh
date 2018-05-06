#!/bin/bash

chmod a+x calwidget
cp -r alarmCal_sounds $HOME/Desktop

cat <<EOT > $HOME/Desktop/alarmCal.desktop
[Desktop Entry]
Type=Application
Name=alarmCal
Comment=An inclusive alarm/calendar system
Exec=$PWD/calwidget
Icon=$PWD/icon.png
Categories=Office;
EOT

chmod a+x $HOME/Desktop/alarmCal.desktop
