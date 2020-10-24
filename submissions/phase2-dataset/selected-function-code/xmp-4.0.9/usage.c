static void usage(char *s, struct options *options)
{
	struct list_head *head;
	struct sound_driver *sd;
	const char *const *hlp;

	printf("Usage: %s [options] [modules]\n", s);

	printf("\nAvailable drivers:\n");

	list_for_each(head, &sound_driver_list) {
		sd = list_entry(head, struct sound_driver, list);
		printf("    %s (%s)\n", sd->id, sd->description);
	}

	list_for_each(head, &sound_driver_list) {
		sd = list_entry(head, struct sound_driver, list);
		if (sd->help)
			printf("\n%s options:\n", sd->description);
		for (hlp = sd->help; hlp && *hlp; hlp += 2)
			printf("   -D%-20.20s %s\n", hlp[0], hlp[1]);
	}

	printf("\nPlayer control options:\n"
"   -D parameter[=val]     Pass configuration parameter to the output driver\n"
"   -d --driver name       Force output to the specified device\n"
"   --fix-sample-loops     Use sample loop start /2 in MOD/UNIC/NP3\n"
"   -l --loop              Enable module looping\n"
"   --loop-all             Loop over entire module list\n"
"   -M --mute ch-list      Mute the specified channels\n"
"   --nocmd                Disable interactive commands\n"
"   --norc                 Don't read configuration files\n"
"   --offset-bug-emulation Emulate Protracker 2.x bug in effect 9\n"
"   -R --random            Random order playing\n"
"   -S --solo ch-list      Set channels to solo mode\n"
"   -s --start num         Start from the specified order\n"
"   -t --time num          Maximum playing time in seconds\n"
"   --vblank               Force vblank timing in Amiga modules\n"
"   -Z                     Play all sequences (subsongs) in module\n"
"   -z --sequence num      Play the specified sequence (0=main)\n" 
"\nMixer options:\n"
"   -a --amplify {0|1|2|3} Amplification factor: 0=Normal, 1=x2, 2=x4, 3=x8\n"
"   -b --bits {8|16}       Software mixer resolution (8 or 16 bits)\n"
"   -c --stdout            Mix the module to stdout\n"
"   -f --frequency rate    Sampling rate in hertz (default 44100)\n"
"   -i --interpolation {nearest|linear|spline}\n"
"                          Select interpolation type (default spline)\n"
"   -m --mono              Mono output\n"
"   -n --null              Use null output driver (same as --driver=null)\n"
"   -F --nofilter          Disable IT lowpass filters\n"
"   -o --output-file name  Mix the module to file ('-' for stdout)\n"
"   -P --pan pan           Percentual pan separation\n"
"   -p --default-pan       Percentual default pan setting (default %d%%)\n"
"   -r --reverse           Reverse left/right stereo channels\n"
"   -u --unsigned          Set the mixer to use unsigned samples\n"
"\nEnvironment options:\n"
"   -I --instrument-path   Set pathname to external samples\n"
"\nInformation options:\n"
"   -C --show-comment      Display the module comment text, if any\n"
"   -h --help              Display a summary of the command line options\n"
"   -L --list-formats      List supported module formats\n"
"   --probe-only           Probe audio device and exit\n"
"   --load-only            Load module and exit\n"
"   -q --quiet             Quiet mode (verbosity level = 0)\n"
"   -V --version           Print version information\n"
"   -v --verbose           Verbose mode (incremental)\n",
		options->defpan);
}