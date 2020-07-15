# htcondor_version

This is the htcondor version of the demo of producing ara simulation in "mass production."

To submit, do `bash submit.sh`

There are three key components of the scripting architecture:

- `submit.sh`: the job that submits to htcondor
- `job.sub`: this is the htcondor submission file (the file with the job parameters)
- `job.sh`: this is the file that is actually executed and "does the work"

## submit.sh

This actually submits the job to htcondor.

You can see two things defined: `output_dir` and `setup_file`.

`output_dir` is the *directory* where you want your output files written to.

`setup_file` is the *full path* to the setup file you want to run AraSim with.

Both `output_dir` and `setup_file` have to be used with `export` because those environment variables will be accessed in `job.sh`.

The `condor_submit` line actually submits the job.

## job.sub

This is the htcondor submission file. There are resources elsewhere about what all the variables in here mean (e.g. https://research.cs.wisc.edu/htcondor/manual/v7.8/2_5Submitting_Job.html).

The log/output/error directories are where you want the job's log/output/err files written to. Note that at UW, these *must* be in the /scratch directory on the submit machine.

`getenv=true` is required because we are using the environment to set the `output_dir` and the `setup_file`. There are ways to avoid needing `getenv=true`, and not using environment variables to pass information to the script, but that's not covered here.

To run AraSim twice, we use `queue 2`. This will spawn two copies of the job, differing by the fact that one will have `$(Process)=0` and the other will have `$(Process)=1`. We can then pass this `$(Process)` variable to the `job.sh` script as an *argument*, and use it to index the AraSim output files, so we get two files called `AraOut.0.root` and `AraOut.1.1root`. This is elaborated more elsewhere in the htcondor documentation (e.g. https://research.cs.wisc.edu/htcondor/manual/quickstart.html)

## job.sh

This is the file that actually contains the commands that will be run.

The one argument we take *in* is the index we want to use for the run number this was done by setting "arguments = $(Process)" in the job.sub script (see above).

Then we activate the ARA cvmfs installation. You will otherwise need to have a script that sets up the software.

Then we go to the AraSim directory.

Then we execute AraSim.

Then we move the output of the simulation to a final output directory.
