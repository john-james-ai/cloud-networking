use warnings;
use strict; 

die ("Usage: perl link_create.pl node1 node2\n") unless (defined $ARGV[0]);

my $node1 = $ARGV[0];
my $node2 = $ARGV[1];

my $addr0 = "10.1.1.$node1";
my $addr1 = "10.1.1.$node2";

`sudo iptables -I OUTPUT -s $addr0 -d $addr1 -j ACCEPT`;
`sudo iptables -I OUTPUT -s $addr1 -d $addr0 -j ACCEPT`;
