#!/usr/bin/perl -T
use strict;
use warnings qw(FATAL all);
$ENV{'PATH'} = '/usr/bin';

if (scalar(@ARGV) != 1) { die "Choose one file!\n"; }
my $filename = shift(@ARGV);
open (my $file, '<', $filename) or die "Cannot open $filename\n";
pipe (my $pipe_read, my $pipe_write) or die "Cannot create pipe\n";

my $wc_process = fork();
if ($wc_process < 0) { die "Cannot create child process\n"; }
if ($wc_process == 0) {
	# child
	close ($pipe_write) or die "Cannot close write end of the pipe\n";
    open (STDIN, '<&', $pipe_read) or die "Cannot replace STDIN with the pipe\n"; 
	print "Character count:\n";
  	exec ("wc -c") or die "Cannot run wc\n";
	print "\n";
} else {
	# parent
	close ($pipe_read) or die "Cannot close read end of the pipe\n";
	my $buffer = "";
  	while (read ($file, $buffer, 2)) {
   	 	print $pipe_write substr($buffer, 1, 1); 
 	}
}
