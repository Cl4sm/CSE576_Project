unsigned opt_max_width (const mpegd_option_t *opt)
{
	unsigned i, n, w;

	w = 0;

	i = 0;
	while (opt[i].name1 >= 0) {
		n = opt_get_width (&opt[i]);

		if (n > w) {
			w = n;
		}

		i += 1;
	}

	return (w);
}
