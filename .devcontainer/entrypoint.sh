#!/bin/bash


trap 'true' SIGTERM
trap 'true' SIGINT

pip install ./project
tail -f /dev/null &
wait $!

echo closing