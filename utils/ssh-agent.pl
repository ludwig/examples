#!/usr/bin/perl -w
##
## Copyright (c)2001 Aaron Hopkins <tools@die.net>; All Rights Reserved
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
##
## http://ftp.die.net/pub/ssh-agent-restore/ssh-agent-restore

#
#
# Possible usage:
# 
#   alias restore-ssh-agent='eval `ssh-agent.pl`'
#
#

use strict;

my $style = (defined $ARGV[0] && $ARGV[0] eq "-c") ? "-c" : "-s";

sub listdir {
    opendir DIR, $_[0] or return;
    my @list = readdir DIR;
    closedir DIR;
    return @list;
}

sub find_socket {
    my ($dir) = @_;
    foreach my $ssh (grep /^ssh-[a-z0-9]+$/i, listdir($dir)) {
        $ssh = join "/", $dir, $ssh;
        foreach my $agent (listdir($ssh)) {
            next unless $agent =~ /^agent\.([0-9]+)$/;
            my $pid = $1 + 1;   # FIXME, this sucks
            if (kill(0, $pid) == 1) {
                return $pid, join("/", $ssh, $agent);
            }
        }
    }
    return undef;
}

my ($pid, $file) = find_socket("/tmp");

if (defined $pid) {
    if ($style eq "-c") {
        print "setenv SSH_AUTH_SOCK $file;\n";
        print "setenv SSH_AGENT_PID $pid;\n";
    } else {
        print "SSH_AUTH_SOCK=$file; export SSH_AUTH_SOCK;\n";
        print "SSH_AGENT_PID=$pid; export SSH_AGENT_PID;\n"
    }
    print "echo Agent pid $pid;\n";
} else {
    exec "ssh-agent", $style;
}

exit 0;
