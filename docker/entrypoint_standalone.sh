#!/bin/bash --login

cleanup() {
    current_time=$(date "+%Y.%m.%d-%H.%M.%S")
    conda env export -p /home/${USER}/workspace/env> /home/${USER}/workspace/project/.conda_snapshots/$current_time.environment.yml
}

trap 'true' SIGTERM

#Execute command
tail -f /dev/null &

#Wait
wait $!

#Cleanup
cleanup