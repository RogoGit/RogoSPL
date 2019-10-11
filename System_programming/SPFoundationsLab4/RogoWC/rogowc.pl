#!/usr/bin/perl -T

use strict;
use warnings qw(FATAL all);

our %file_info = ();

sub handle_file($) {

	my $filename = $_[0];
	
	# format is: 0-newlines, 1-word count, 2-bytes count, 3-characters count
	my @data_array = (0,0,0,0);
	
	open(my $file, '<', $filename) or die "Could not open file '$filename' $!\n";
	$data_array[2]= (stat $filename)[7];
	
	while (my $row = <$file>) {
  		chomp $row;
  		$data_array[0]+=1;
		$data_array[1]+=scalar(split(" ",$row));
		$data_array[3]+=length($row);
	}
	
	$file_info{$filename} = \@data_array;
	
	close($file);
}

#main

if (scalar(@ARGV) == 0) { die "No arguments supplied!\n" }

foreach my $curr_file (@ARGV) {
	handle_file($curr_file);
}

foreach my $key (keys %file_info) {
	print("@{$file_info{$key}}[0]   @{$file_info{$key}}[1]   @{$file_info{$key}}[2]   $key\n");
}