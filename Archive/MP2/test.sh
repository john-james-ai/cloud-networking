#!/bin/dash
./manager 3 send 2 "Test: 1 Case: 1 From 3 to 2"
sleep 1
./manager 2 send 1 "Test: 2 Case: 2 From 2 to 1"
sleep 1
./manager 3 send 0 "Test: 3 Case: 3 From 3 to 0"
sleep 1
./manager 4 send 5 "Test: 4 Case: 4 From 4 to 5"
sleep 1
./manager 7 send 5 "Test: 5 Case: 4 From 7 to 5"
sleep 2
perl link_drop.pl 2 5
sleep 5
perl link_drop.pl 4 1
sleep 5
./manager 3 send 1 "Test: 6 Case: 5 From 3 to 1"
sleep 1
perl link_drop.pl 4 7
sleep 5
./manager 2 send 1 "Test: 7 Case: 5 From 2 to 1"
sleep 1
perl link_create.pl 4 1
sleep 5
./manager 2 send 1 "Test: 8 Case: 6 From 2 to 1"
sleep 1
perl link_drop.pl 255 1
sleep 5
./manager 4 send 0 "Test: 9 Case: 7 From 4 to 0"
sleep 2
perl link_create.pl 255 1
perl link_create.pl 4 7
perl link_create.pl 2 5