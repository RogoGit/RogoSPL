#!/usr/bin/perl -T

use strict;
use warnings qw(FATAL all);

our %file_info = ();
our $files_number = 0;

sub handle_file($) {
	
	$files_number+=1;
	my $filename = $_[0];
	
	# format is: 0-newlines, 1-word count, 2-bytes count, 3-characters count
	my @data_array = (0,0,0,0);
	
	if (open(my $file, '<', $filename)) {
	$data_array[2]= (stat $filename)[7];
	
	while (my $row = <$file>) {
  		chomp $row;
  		$data_array[0]+=1;
		$data_array[1]+=scalar(split(" ",$row));
		$data_array[3]+=length($row);
	}
	
	$file_info{$filename} = \@data_array;
	
	close($file);
	} else {
		print("$filename - no such file found\n");
	}
}

#main

my $bytes_flag = 0; # -c
my $chrs_flag = 0; # -m
my $lines_flag = 0; # -l
my $words_flag = 0; # -w

if (scalar(@ARGV) == 0) { die "No arguments supplied!\n" }

foreach my $arg (@ARGV) {

	if ($arg =~ /^-/) {
		# it is a flag
		for my $c (split //, $arg) {
  			if 	   ($c eq 'l') { $lines_flag = 1; }
			elsif  ($c eq 'c') { $bytes_flag = 1; }
			elsif  ($c eq'm') { $chrs_flag = 1; }
			elsif  ($c eq 'w') { $words_flag = 1; }
			elsif  ($c eq '-') { }
			else   { die "$c - illegal option"; }
		}		
	} else {
		# actually it is ordinary file
		handle_file($arg);
	}
}

foreach my $key (keys %file_info) {
	print("  @{$file_info{$key}}[0]   @{$file_info{$key}}[1]   @{$file_info{$key}}[2]   $key\n");
}
