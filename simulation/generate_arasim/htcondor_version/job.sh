#!/bin/bash

# the one argument we take *in* is the index we want to use for the run number
# this was done by setting "arguments = $(Process)" in the job.sub script
# see this page (https://research.cs.wisc.edu/htcondor/manual/quickstart.html)
# for more information on the $(Process) macro
index=$1

# activate the ARA cvmfs installation
# you will otherwise need to have a script that sets up the software
source /cvmfs/ara.opensciencegrid.org/trunk/centos7/setup.sh

# go to the AraSim directory
# you could replace this with a "local" installation if you wanted
cd /cvmfs/ara.opensciencegrid.org/trunk/centos7/source/AraSim

# execute AraSim
# arg 1 = full path to the setup file
# arg 2 = AraSim index, i.e. AraOut.1.root
# arg 3 = output directory, here we are writing to local disk space ($TMPDIR)
./AraSim $setup_file $index $TMPDIR

# after we are done, move the root output file 
# from $TMPDIR into our permanent ouput location
mv $TMPDIR/*.root $output_dir