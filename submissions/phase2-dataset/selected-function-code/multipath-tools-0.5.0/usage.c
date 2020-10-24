{
	fprintf(stderr,
			"Usage: mpathpersist [OPTIONS] [DEVICE]\n"
			" Options:\n"
			"    --verbose|-v level         verbosity level\n"
			"                   0           Critical messages\n"
			"                   1           Error messages\n"
			"                   2           Warning messages\n"
			"                   3           Informational messages\n"
			"                   4           Informational messages with trace enabled\n"
			"    --clear|-C                 PR Out: Clear\n"
			"    --device=DEVICE|-d DEVICE  query or change DEVICE\n"
			"    --help|-h                  output this usage message\n"
			"    --hex|-H                   output response in hex\n"
			"    --in|-i                    request PR In command \n"
			"    --out|-o                   request PR Out command\n"
			"    --param-aptpl|-Z           PR Out parameter 'APTPL'\n"
			"    --read-keys|-k             PR In: Read Keys\n"
			"    --param-sark=SARK|-S SARK  PR Out parameter service "
			"action\n"
			"                               reservation key (SARK is in "
			"hex)\n"
			"    --preempt|-P               PR Out: Preempt\n"
			"    --preempt-abort|-A         PR Out: Preempt and Abort\n"
			"    --prout-type=TYPE|-T TYPE  PR Out command type\n"
			"    --read-full-status|-s      PR In: Read Full Status\n"
			"    --read-keys|-k             PR In: Read Keys\n"
			"    --read-reservation|-r      PR In: Read Reservation\n"
			"    --register|-G              PR Out: Register\n"
			"    --register-ignore|-I       PR Out: Register and Ignore\n"
			"    --release|-L               PR Out: Release\n"
			"    --report-capabilities|-c   PR In: Report Capabilities\n"
			"    --reserve|-R               PR Out: Reserve\n"
			"    --transport-id=TIDS|-X TIDS  TransportIDs can be mentioned \n"
			"                               in several forms\n"
			" Examples:\n"
			"     mpathpersist --out --register --param-sark=123abc --prout-type=5 /dev/mapper/mpath9\n"
			"     mpathpersist -i -k /dev/mapper/mpath9\n"	);
}
