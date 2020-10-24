{
	mpegd_option_t     *ret;
	static int        atend = 0;
	static int        index1 = -1;
	static int        index2 = -1;
	static const char *curopt = NULL;

	if (index1 < 0) {
		atend = 0;
		index1 = 0;
		index2 = 1;
		curopt = NULL;
	}

	if (atend) {
		if (index2 >= argc) {
			return (GETOPT_DONE);
		}

		index1 = index2;
		index2 += 1;

		*optarg = argv + index1;

		return (0);
	}

	if ((curopt == NULL) || (*curopt == 0)) {
		if (index2 >= argc) {
			return (GETOPT_DONE);
		}

		index1 = index2;
		index2 += 1;

		curopt = argv[index1];

		if ((curopt[0] != '-') || (curopt[1] == 0)) {
			*optarg = argv + index1;
			curopt = NULL;
			return (0);
		}

		if (curopt[1] == '-') {
			if (curopt[2] == 0) {
				atend = 1;

				if (index2 >= argc) {
					return (GETOPT_DONE);
				}

				index1 = index2;
				index2 += 1;

				*optarg = argv + index1;

				return (0);
			}

			ret = find_option_name2 (opt, curopt + 2);

			if (ret == NULL) {
				fprintf (stderr, "%s: unknown option (%s)\n",
					argv[0], curopt
				);
				return (GETOPT_UNKNOWN);
			}

			if ((index2 + ret->argcnt) > argc) {
				fprintf (stderr,
					"%s: missing option argument (%s)\n",
					argv[0], curopt
				);
				return (GETOPT_MISSING);
			}

			*optarg = argv + index2;
			index2 += ret->argcnt;
			curopt = NULL;

			return (ret->name1);
		}

		curopt += 1;
	}

	ret = find_option_name1 (opt, *curopt);

	if (ret == NULL) {
		fprintf (stderr, "%s: unknown option (-%c)\n",
			argv[0], *curopt
		);
		return (GETOPT_UNKNOWN);
	}

	if ((index2 + ret->argcnt) > argc) {
		fprintf (stderr,
			"%s: missing option argument (-%c)\n",
			argv[0], *curopt
		);
		return (GETOPT_MISSING);
	}

	*optarg = argv + index2;
	index2 += ret->argcnt;
	curopt += 1;

	return (ret->name1);
}
