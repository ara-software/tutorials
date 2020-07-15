#!/bin/bash

# these are environment variables that job.sh will grab
# the user should change them!

output_dir="/data/user/brianclark/demo"
export output_dir

setup_file="/home/brianclark/tutorials/simulation/generate_arasim/htcondor_version/sample_setup.txt"
export setup_file

condor_submit job.sub