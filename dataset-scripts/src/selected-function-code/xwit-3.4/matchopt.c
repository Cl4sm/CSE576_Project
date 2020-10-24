static int
matchopt(const char *key, int nargs, int *argc, char **argv)
{
	int enough = 0;
	int match = 1;
	char *ap;
	const char *kp;

	ap = *argv;
	if (*ap == '-')
		ap++;

	for (kp = key; *kp; kp++, ap++) {
		if (*kp == '*') {
			enough = 1;
			ap--;
			continue;
		}
		if (*ap == 0) {
			match = enough;
			break;
		}
		if (*ap != *kp) {
			match = 0;
			break;
		}
	}

	if (match) {
		if (argc[0] <= nargs) {
			char option[32];
			int dash, skip;

			strncpy(option, *argv, sizeof option - 1);
			option[sizeof option - 1] = 0;

			dash = *argv[0] == '-';

			for (ap = option; *ap; ap++)
				/* nothing */ ;
			skip = ap - option - dash;
			enough = 0;
	
			for (kp = key; *kp && skip--; kp++) {
				if (*kp == '*')
					skip++;
			}
			for (; *kp; kp++) {
				if (*kp == '*')
					continue;
				if (enough == 0) {
					*ap++ = '(';
					enough = 1;
				}
				*ap++ = *kp;
			}
			if (enough)
				*ap++ = ')';
			*ap = 0;

			fprintf(stderr,
				"%s: option %s needs %d argument%s\n\n",
				program_name, option,
				nargs, nargs > 1 ? "s" : "");
			usage();
		}
		argc[0] -= nargs;
	}
	return match;
}