#!/usr/bin/perl
 
use strict;
use v5.18;

use AnyEvent;                           
use AnyEvent::Socket qw(tcp_server);    
use AnyEvent::Handle;                  

use Coro;

use Data::Dumper qw(Dumper);

use Class::Struct;

print("Server is running...\n");

# dirs function

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

# thread struct
struct clt_thread => {
	id => '$',
	thread => '$',
	is_busy => '$',
	args => '$',
	client => '$',
	clt_key => '$'
};

my $threads_num = 16;
my $thread_id = 0;
my @pool = ();

# incrementing threads

sub inc_pool {
	for my $j (0..($threads_num-scalar(@pool))) {
		my $coro = new Coro(\&handle_request, $thread_id);
		my $thread = clt_thread->new(id => $thread_id, thread => $coro, is_busy => 0, args => '', client => '', clt_key => '');
		push @pool, $thread;
		$thread_id = $thread_id+1;
	}
	return;
}

# handling request

my $cv = AE::cv;
my %client = ();

sub handle_request {

		my $thread_id = shift;
		my $thread;
		foreach my $thr (@pool) {
			if ($thr->id == $thread_id) { $thread = $thr; }
		}
		my $self = $thread->client;
		my $client_key = $thread->clt_key;
		my $dir_list = $thread->args;
		if ($thread->client != '') {
		
			say "Directories read: " . $dir_list . "\n";
        	my @clients = keys %client;
				
        	for my $key (grep {$_ ne $client_key} @clients) {
               	my $response = print_dirs($dir_list);
               	$client{$key}->push_write("$response");
			   	$self->push_shutdown;
               	delete $client{$client_key};
				delete $client{$self};
			}
			
		}
		$thread->is_busy(0);
		pop @pool, $thread;
		if (scalar(@pool) < 3) { inc_pool; }
		Coro::cede();
}

# threads creation

for my $i (0..$threads_num) {
	my $coro = new Coro(\&handle_request, $thread_id);
	my $thread = clt_thread->new(id => $thread_id, thread => $coro, is_busy => 0, args => '', client => '', clt_key => '');
	push @pool, $thread;
	$thread_id = $thread_id+1;
}

# tcp server creation - main part

tcp_server '127.0.0.1', 8015, sub {

    my ($fh, $host, $port) = @_;
 	my $client_key = "$host:$port";
	
    my $hdl = AnyEvent::Handle->new(
        	fh => $fh,
			poll => 'r',
			on_read => sub {
					my ($self) = @_;
					
					foreach my $thr (@pool) {
						if (!($thr->is_busy)) {
						$thr->client($self);
						$thr->args($self->rbuf); 
						$thr->clt_key($client_key);
						$thr->is_busy(1);
						$thr->thread->ready();
						return;
						}	
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
