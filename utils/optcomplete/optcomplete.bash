#!/bin/sh
#
# $Id: optcomplete.bash,v 1.6 2004/01/26 06:24:48 blais Exp $
# $Source: /home/blais/repos/cvsroot/optcomplete/etc/optcomplete.bash,v $
#

# optcomplete harness for bash shell. You then need to tell 
# bash to invoke this shell function with a command like 
# this::
#
#   complete -F _optcomplete optcomplete-test
#

_optcomplete()
{
    COMPREPLY=( $( \
	COMP_LINE=$COMP_LINE  COMP_POINT=$COMP_POINT \
	COMP_WORDS="${COMP_WORDS[*]}"  COMP_CWORD=$COMP_CWORD \
	OPTPARSE_AUTO_COMPLETE=1 $1 ) )
}
