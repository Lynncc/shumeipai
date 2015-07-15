#!/bin/sh

# get_opt()
# {
# 	select para in "build" "clean" "distclean" "help" "show"; do
# 		break;
# 	done
# 	select project in "init" "str" "time" "badapple"; do
# 		break;
# 	done
# }

if [ $# -lt 1 ]; then
	echo "please enter moudle name"
	echo "for example:"
	echo "    ./build init"
	echo "    ./build clean init "
	echo "    ./build help init "
	exit
else
	para=$1
	project=$2
fi

make ${para} moudle="${project}"
