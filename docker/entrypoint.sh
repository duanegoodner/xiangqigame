#!/bin/bash 

cleanup() {
    current_time=$(date "+%Y.%m.%d-%H.%M.%S")
    conda env export -p $1 > $2/$current_time.environment.yml
}

trap 'true' SIGTERM

#Execute command
tail -f /dev/null &

#Wait
wait $!

#Cleanup
cleanup $1 $2