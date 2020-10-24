unsigned opt_get_width (const mpegd_option_t *opt)
{
	unsigned n;

	if (opt->optdesc == NULL) {
		return (0);
	}

	n = 0;

	if (opt->name1 <= 255) {
		n += 2;

		if (opt->name2 != NULL) {
			n += 2;
		}
	}

	if (opt->name2 != NULL) {
		n += 2 + strlen (opt->name2);
	}

	if (opt->argdesc != NULL) {
		n += 1 + strlen (opt->argdesc);
	}

	return (n);
}
