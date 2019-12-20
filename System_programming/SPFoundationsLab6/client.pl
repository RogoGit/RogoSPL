#!/usr/bin/perl

use strict;
use warnings qw(FATAL all);
use IO::Socket::INET;

my $host = '127.0.0.1';
my $port = 8015;

my $request = join(" ", @ARGV);

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
