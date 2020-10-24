char *
pvmgetroot()
{
	static char *rd = 0;

	if (!rd) {

#ifdef IMA_CSPP

		struct stat buf;

		rd = STRALLOC("/usr/convex/pvm");
		if (stat(rd, &buf) == -1) {
			pvmlogperror(
				"Unable to default PVM_ROOT to /usr/convex/pvm");
			pvmbailout(0);
			exit(1);		/* the other meaning of bail out */
		}
		pvmputenv("PVM_ROOT=/usr/convex/pvm");
		pvmlogerror("Defaulting PVM_ROOT to /usr/convex/pvm");

#else

#ifdef CYGWIN
		rd = getenv("PVM_ROOT_U");
#else
		rd = getenv("PVM_ROOT");
#endif

#ifdef WIN32
		if (!rd)
			rd = read_pvmregistry("PVM_ROOT");
#endif

#endif

		if (!rd) {
			pvmlogerror("PVM_ROOT environment variable not set.\n");
			pvmbailout(0);
			exit(1);		/* the other meaning of bail out */
		}
	}

	return rd;
}
