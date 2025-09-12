#!/usr/bin/env bash
#screenshot tool script

#TODO
#1 - fix the notification being sent even if a screenshot is not taken
#2 - stretch goal: figure out why text is printed off-center

#variables
PIC_NAME="$(date +%d%m%y-%H%M%S).png"
SAVE_DIR=$HOME/Pictures
ROFI_CONFIG=$HOME/.config/rofi/screenshot.rasi
TEXT_ENABLE=false
NOTIF_ICON=$HOME/Documents/profile/man.png

#choices to pass into rofi
all=(screen region window)

#text
declare -A texts
texts[screen]="Screen"
texts[region]="Region"
texts[window]="Window"

#icons
declare -A icons
icons[screen]="󰍹"
icons[region]=""
icons[window]=""

#generate the elements for rofi list view
get_options() {
	for entry in ${all[@]}
	do
		if [ "$TEXT_ENABLE" = true ]
		then
			echo "${texts[$entry]} ${icons[$entry]}"
		else
			echo "${icons[$entry]}"
		fi
	done
}
#start rofi with the generated choices, and capture which choice was selected
choice=$((get_options) | rofi -dmenu -p "Maim" -config $ROFI_CONFIG)

#perform chosen action
case $choice in
	"${texts[screen]} ${icons[screen]}" | "${icons[screen]}")
	maim --hidecursor $SAVE_DIR/$PIC_NAME -d 1 && xclip -selection clipboard -t image/png "$SAVE_DIR/$PIC_NAME"
	;;

	"${texts[region]} $icons[region]" | "${icons[region]}")
	maim --hidecursor -s $SAVE_DIR/$PIC_NAME -d 1 && xclip -selection clipboard -t image/png "$SAVE_DIR/$PIC_NAME"
	;;

	"${texts[window]} ${icons[window]}" | "${icons[window]}")
	maim --hidecursor -i "$(xdotool getactivewindow)" $SAVE_DIR/$PIC_NAME -d 1 && xclip -selection clipboard -t image/png "$SAVE_DIR/$PIC_NAME"
	;;
esac
#notify the user
notify-send --app-name=Maim 'Screenshot Taken' $SAVE_DIR/$PIC_NAME --icon=$NOTIF_ICON

exit 0
