static void usage(char *argv0)
{
	fprintf(stderr,"Usage:\n"
	"%s flags traceuri [traceuri...]\n"
	"-f --filter=bpf	\tApply BPF filter. Can be specified multiple times\n"
	"-c --count=N		Stop after reading N packets\n"
	"-e --error		Report packet errors (e.g. checksum failures, rxerrors)\n"
	"-F --flow		Report flows\n"
	"-m --misc		Report misc information (start/end times, duration, pps)\n"
	"-P --protocol		Report transport protocols\n"
	"-p --port		Report port numbers\n"
	"-T --tos		Report IP TOS\n"
	"-t --ttl		Report IP TTL\n"
	"-O --tcpoptions	\tReport TCP Options\n"
	"-o --synoptions	\tReport TCP Options seen on SYNs\n"
	"-n --nlp		Report network layer protocols\n"
	"-d --direction		Report direction\n"
	"-C --ecn		Report TCP ECN information\n"
	"-s --tcpsegment	\tReport TCP segment size\n"
	"-H --help		Print libtrace runtime documentation\n"
	,argv0);
	exit(1);
}