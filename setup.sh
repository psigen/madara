#!/bin/bash
#
# Set up necessary environement variables for MADARA
#
# Absolute path to this script. /home/user/bin/foo.sh
SCRIPTPATH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

export ACE_ROOT=$SCRIPTPATH/ace/ACE_wrappers
export TAO_ROOT=$ACE_ROOT/TAO
export MADARA_ROOT=$SCRIPTPATH/madara
export LD_LIBRARY_PATH=$ACE_ROOT/lib:$TAO_ROOT/lib:$MADARA_ROOT/lib:$LD_LIBRARY_PATH
export PATH=$ACE_ROOT/bin:$TAO_ROOT/bin:$MADARA_ROOT/bin:$PATH