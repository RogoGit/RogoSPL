#!/usr/bin/perl

use strict;
use warnings qw(FATAL all);
use IO::Socket::INET;

my $host = $ARGV[0];
my $port = $ARGV[1];
my @dirs = @ARGV[2..(scalar(@ARGV)-1)];

if (scalar(@ARGV) < 3) { die "Usage: host, port, directories\n"; }

my $request = join(" ", @dirs);

my $client = new IO::Socket::INET (
  	PeerHost => $host,
	PeerPort => $port, 
	Proto => 'tcp'
) or die "Failed to connect to server\n";

$client->send($request);
while(<$client>) {
	print("$_");
}
close($client);
