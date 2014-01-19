#!/bin/bash

# Absolute path to this script.
SCRIPTPATH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Update ACE
cd ${SCRIPTPATH}/ace.git
git svn rebase | grep -v "Current branch master is up to date."
git push -q origin master --force

# Update MPC
cd ${SCRIPTPATH}/mpc.git
git svn rebase | grep -v "Current branch master is up to date."
git push -q origin master --force

# Update MADARA
cd ${SCRIPTPATH}/madara.git
git svn rebase | grep -v "Current branch master is up to date."
git push -q origin master --force
