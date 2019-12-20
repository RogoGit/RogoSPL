#!/usr/bin/perl
 
use strict;
use v5.18;

use AnyEvent;                           
use AnyEvent::Socket qw(tcp_server);    
use AnyEvent::Handle;                  

use Data::Dumper qw(Dumper);

print("Server is running...\n");

sub print_dirs {

	my $dir_list = $_[0];
	my @dirs = split(" ", $dir_list);
	my $result = "";

	for my $dir (@dirs) {
    	if (opendir my $dirent, $dir) {
      		my @files = readdir $dirent;
      		closedir $dirent;
      		$result = $result . "\nContents of $dir:\r\n" . join("\r\n", @files) . "\r\n";
    	} else {
      		$result = $result . "Failed to open $dir: $!\r\n";
		}
	}
	
	return $result;
}

my $cv = AE::cv;
my %client = ();

tcp_server '127.0.0.1', 8015, sub {

    my ($fh, $host, $port) = @_;
 	my $client_key = "$host:$port";
	
    my $hdl = AnyEvent::Handle->new(
        	fh => $fh,
			poll => 'r',
			on_read => sub {
	
					my ($self) = @_;
        			say "Directories read: " . $self->rbuf . "\n";
        			my @clients = keys %client;
            		my $dir_list = $self->rbuf;
					
            		for my $key (grep {$_ ne $client_key} @clients) {
                		my $response = print_dirs($dir_list);
                		$client{$key}->push_write("$response");
						$self->push_shutdown;
                		delete $client{$client_key};
						delete $client{$self};
					}
			},
			on_error => sub {
				say "Something went wrong: $!\n";
			},
    );
	
    $client{$client_key} = $hdl;
	$client{$hdl} = $hdl;
};

$cv->recv;
