#!/bin/perl

## So that we can have more devices and frequency to play with

## now change the c files
system("mv kernel/device.c kernel/backup_dev_c");
open(INF2,"kernel/backup_dev_c");
open(OF2,'>>kernel/device.c');

@lines = <INF2>;
foreach $loc (@lines)
{
	@code = split(/\s+/,$loc);
	## ok, this wont work if thet have other unsigned long
	if(($code[0] eq "const") && ($code[1] eq "unsigned")
&& ($code[2] eq "long") && ($code[3] eq "dev_freq[NUM_DEVICES]")) {
		$code[4] = "=";
		$code[5] = "{";
		
		for($i =0;$i < 59;$i++) {
			$code [$i+6] = (($i+1) * 100) . ",";
		}
		$code[$i+6] = "9000};\n";
		$loc2 = join(' ',@code);
		print OF2 $loc2 . "\n";
	} 
	else {
		print OF2 $loc;
	}
}
close(INF2);
close(OF2);
