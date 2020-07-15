#!/bin/bash

index=$1

source /cvmfs/ara.opensciencegrid.org/trunk/centos7/setup.sh

cd /cvmfs/ara.opensciencegrid.org/trunk/centos7/source/AraSim

./AraSim $setup_file $index $TMPDIR
cp $TMPDIR/*'*.root' $output_dir