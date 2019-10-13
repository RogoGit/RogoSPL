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
		my @row_split = split(" ",$row);
		$data_array[1]+=scalar(@row_split);
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

my @parameters_sum = (0,0,0,0);  

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
			else   { die "$c - illegal option\n"; }
		}		
	} else {
		# actually it is ordinary file
		handle_file($arg);
	}
}

foreach my $key (keys %file_info) {
	
	$parameters_sum[0]+=@{$file_info{$key}}[0];
	$parameters_sum[1]+=@{$file_info{$key}}[1];
	$parameters_sum[2]+=@{$file_info{$key}}[2];
	$parameters_sum[3]+=@{$file_info{$key}}[3];

	# default behaviour
	if (!$lines_flag && !$words_flag && !$bytes_flag && !$chrs_flag) {
		print("  @{$file_info{$key}}[0]   @{$file_info{$key}}[1]   @{$file_info{$key}}[2]   $key\n");
	} else {
	
	# constructing output
	my $output_str = "";
	
	if ($lines_flag) {
		$output_str.="  @{$file_info{$key}}[0]";
	}
	if ($words_flag) {
		$output_str.="  @{$file_info{$key}}[1]";
	}
	if ($bytes_flag) {
		$output_str.="  @{$file_info{$key}}[2]";
	}
	if ($chrs_flag) {
		$output_str.="  @{$file_info{$key}}[3]";
	}
		$output_str.="	$key\n";
		print($output_str);
	}
}

# printing sum of bytes/words/lines/characters when several files
if ($files_number > 1) {
	
	# default behaviour
	if (!$lines_flag && !$words_flag && !$bytes_flag && !$chrs_flag) {
		print("  $parameters_sum[0]   $parameters_sum[1]   $parameters_sum[2]   итого\n");
	} else {
	
	my $sum_string = "";
	
	if ($lines_flag) {
		$sum_string.="  $parameters_sum[0]";
	}
	if ($words_flag) {
		$sum_string.="  $parameters_sum[1]";
	}
	if ($bytes_flag) {
		$sum_string.="  $parameters_sum[2]";
	}
	if ($chrs_flag) {
		$sum_string.="  $parameters_sum[3]";
	}
		$sum_string.="	итого\n";
		print($sum_string);
	}
}


