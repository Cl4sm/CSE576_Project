static void usage(void)
{
	fprintf(stderr,
		"Usage: %s [options] [master_map_name]\n"
		"	-h --help	this text\n"
		"	-p --pid-file f write process id to file f\n"
		"	-t --timeout n	auto-unmount in n seconds (0-disable)\n"
		"	-v --verbose	be verbose\n"
		"	-d --debug	log debuging info\n"
		"	-D --define	define global macro variable\n"
		"	-f --foreground do not fork into background\n"
		"	-r --random-multimount-selection\n"
		"			use ramdom replicated server selection\n"
		"	-m --dumpmaps [<map type> <map name>]\n"
		"			dump automounter maps and exit\n"
		"	-n --negative-timeout n\n"
		"			set the timeout for failed key lookups.\n"
		"	-O --global-options\n"
		"			specify global mount options\n"
		"	-l --set-log-priority priority path [path,...]\n"
		"			set daemon log verbosity\n"
		"	-C --dont-check-daemon\n"
		"			don't check if daemon is already running\n"
		"	-F --force	forceably clean up known automounts at start\n"
		"	-V --version	print version, build config and exit\n"
		, program);
}