#!/bin/bash

file="${1:-./}"
output_path="${2:-./}"
options="${3:-}"

log_folder="trexfitter_logs"
log_output_path="${output_path}/${log_folder}"
mkdir -p "${log_output_path}"


# Iterate over the files and run the trexfitter command

echo "Running trexfitter for file: $file"
filename=$(basename "$file")
mkdir -p "${log_output_path}/log_${filename}"

for option in {h,w,d,b,f,p,l,s,r,i}; do
# for option in {h}; do
    trex-fitter "$option" "$file" "$options" | tee "${log_output_path}/log_${filename}/fit_$option.txt"
done

