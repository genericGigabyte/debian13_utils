#!/usr/bin/env bash
#PS1 power line generation script

RETURN_CODE=$?

# time, user, directory, return code
all=(t u d rc)
#colour codes
col_types=(BG FG)

TEXT_FG="\e[30;"

declare -A GREYS
GREYS[BG]="47m"
GREYS[FG]="37"

declare -A REDS
REDS[BG]="41m"
REDS[FG]="31"

declare -A YELLOWS
YELLOWS[BG]="43m"
YELLOWS[FG]="33"

declare -A GREENS
GREENS[BG]="42m"
GREENS[FG]="32"

declare -A PURPLES
PURPLES[BG]="45m"
PURPLES[FG]="35"

declare -A BLUES
BLUES[BG]="44m"
BLUES[FG]="34"

declare -A CYANS
CYANS[BG]="46m"
CYANS[FG]="36"

# the colours of each section
declare -A colours
colours[t]="CYANS"
colours[u]="BLUES"
colours[d]="PURPLES"
colours[rc]="REDS"
END="\e[0m\n"

#the actual for each request display
declare -A codes
codes[t]="\t"
codes[u]="\u"
codes[d]="\w"
codes[rc]=$RETURN_CODE

#power line character
POWERLINE_CHAR=$'\uE0B0 '

#generate the power line
POWERLINE=""
for ((i=0; i<${#all[@]}; i++))
do
	entry="${all[$i]}"

	declare -n current_colours="${colours[$entry]}"

	current_bg="${current_colours[BG]}"
	current_fg="${current_colours[FG]}"

	if [ $entry = ${all[-1]} ]
	then
		line="$TEXT_FG${current_bg} ${codes[$entry]} \e[0;${current_fg}m$POWERLINE_CHAR"
	else
		next_entry="${all[$i+1]}"
		declare -n next_colours="${colours[$next_entry]}"

		next_bg="${next_colours[BG]}"
		line="$TEXT_FG${current_bg} ${codes[$entry]} \e[${current_fg};${next_bg}$POWERLINE_CHAR"
	fi
	POWERLINE+=$line
done
POWERLINE+=$END

PS1=$POWERLINE
