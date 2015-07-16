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


words="init str time badapple ip"
for i in ${words}; do
	make ${1} moudle="${i}"
done

