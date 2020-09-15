#!/bin/sh
#http://stackoverflow.com/questions/96882/how-do-i-create-a-nice-looking-dmg-for-mac-os-x-using-command-line-tools

set -o verbose #echo onset +o verbose #echo off

# Note: this must run on a Mac

APP_NAME="CWP"
OUT_MAC=out/
DMG_PATH=${OUT_MAC}${APP_NAME}.dmg
DMG_CONTENT_PATH=${OUT_MAC}contents
BUNDLE_PATH=${DMG_CONTENT_PATH}/${APP_NAME}.app

#clean old dmg if exist
rm -rf ${DMG_PATH}

SetFile -a B "${BUNDLE_PATH}"

hdiutil create -srcfolder ${DMG_CONTENT_PATH} -volname ${APP_NAME} -fs HFS+ \
-fsargs "-c c=64,a=16,e=16" -format UDRW -size 550m ${DMG_PATH}.temp.dmg

device=$(hdiutil attach -readwrite -noverify -noautoopen "${DMG_PATH}".temp.dmg | \
egrep '^/dev/' | sed 1q | awk '{print $1}')

osascript <<EOT
tell application "Finder"
tell disk "${APP_NAME}"
open
set current view of container window to icon view
set toolbar visible of container window to false
set statusbar visible of container window to false
set the bounds of container window to {200, 100, 900, 530}
set theViewOptions to the icon view options of container window
set arrangement of theViewOptions to snap to grid
set icon size of theViewOptions to 96
set background picture of theViewOptions to file ".background:installer_dmg_bg.png"
make new alias file at container window to POSIX file "/Applications" with properties {name:"Applications"}
delay 1
set position of item "${APP_NAME}" of container window to {200, 200}
set position of item "Applications" of container window to {500, 200}
-- update without registering applications
delay 5
-- eject
end tell
end tell
EOT

chmod -Rf go-w "/Volumes/${APP_NAME}"
sync
sync
hdiutil detach ${device}
sync
hdiutil convert "${DMG_PATH}".temp.dmg -format UDZO -imagekey zlib-level=9 -o ${DMG_PATH}
rm -rf "${DMG_PATH}".temp.dmg

