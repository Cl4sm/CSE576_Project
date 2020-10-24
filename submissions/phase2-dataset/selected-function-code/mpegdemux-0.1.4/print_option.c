void print_option (const mpegd_option_t *opt, unsigned w)
{
	unsigned n;

	n = 0;

	if (opt->name1 <= 255) {
		printf ("  -%c", opt->name1);
		n += 2;

		if (opt->name2 != NULL) {
			printf (", ");
			n += 2;
		}
	}
	else {
		printf ("  ");
	}

	if (opt->name2 != NULL) {
		printf ("--%s", opt->name2);
		n += 2 + strlen (opt->name2);
	}

	if (opt->argdesc != NULL) {
		printf (" %s", opt->argdesc);
		n += 1 + strlen (opt->argdesc);
	}

	while (n < w) {
		fputc (' ', stdout);
		n += 1;
	}

	printf ("%s\n", opt->optdesc);
}
