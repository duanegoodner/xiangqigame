#!/bin/bash

cd ${HOME}/workspace/project/src/core

find . -name "*.hpp" -exec hpp2plantuml {} + -o ../output.puml

