#!/bin/perl

## So that we can have more devices and frequency to play with

## change device number
system("mv kernel/include/device.h kernel/include/backup_dev");
open(INF1,"kernel/include/backup_dev");
open(OF1,'>>kernel/include/device.h');

@lines = <INF1>;
foreach $loc (@lines)
{
	@code = split(/\s+/,$loc);
	if(($code[0] eq "#define") && ($code[1] eq "NUM_DEVICES")) {
		$code[2] = 60;
		$loc2 = join(' ',@code);
		print OF1 $loc2 . "\n";
	} 
	else {
		print OF1 $loc;
	}
}
close(INF1);
close(OF1);
