void read_modconf(struct options *o, unsigned char *md5)
{
#if defined __EMX__
	char myrc[PATH_MAX];
	char *home = getenv("HOME");

	snprintf(myrc, PATH_MAX, "%s\\.xmp\\modules.conf", home);
	parse_modconf(o, "xmp-modules.conf", md5);
	parse_modconf(o, myrc, md5);
#elif defined __AMIGA__
	parse_modconf(o, "PROGDIR:modules.conf", md5);
#elif defined WIN32
	char myrc[PATH_MAX];
	const char *home = getenv("USERPROFILE");
	if (!home) home = "C:";

	snprintf(myrc, PATH_MAX, "%s/modules.conf", home);
	parse_modconf(o, myrc, md5);
#else
	char myrc[PATH_MAX];
	char *home = getenv("HOME");

	snprintf(myrc, PATH_MAX, "%s/.xmp/modules.conf", home);
	parse_modconf(o, SYSCONFDIR "/modules.conf", md5);
	parse_modconf(o, myrc, md5);
#endif
}