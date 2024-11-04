#!/bin/bash

# Define input and output paths
input_path="./include/*.hpp"
output_path="./docs/plantuml/my_project.puml"

# Run hpp2plantuml to generate the initial .puml file
hpp2plantuml -i "$input_path" -o "$output_path"

# Check if the file was created successfully
if [ ! -f "$output_path" ]; then
    echo "Error: The file '$output_path' was not created."
    exit 1
fi

# Use awk to replace the first line with formatted text for better readability
awk 'NR==1 {print "@startuml\n"\
"!define RECTANGLE_PACKAGE\n"\
"skinparam dpi 300\n"\
"skinparam packageStyle rect\n"\
"skinparam wrapWidth 400\n"\
"left to right direction\n"\
"scale 0.75"} \
NR!=1' "$output_path" > temp.puml && mv temp.puml "$output_path"

echo "Updated .puml file is ready at '$output_path'."

# Generate SVG from the .puml file
plantuml "$output_path" -tsvg

# Construct the SVG file path by replacing the .puml extension with .svg
svg_output_path="${output_path%.puml}.svg"

# Echo the SVG file path
echo "SVG file generated at: $svg_output_path"
