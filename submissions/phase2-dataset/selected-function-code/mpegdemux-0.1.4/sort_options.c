static
void sort_options (mpegd_option_t *opt)
{
	unsigned      i, j;
	mpegd_option_t tmp;

	if (opt[0].name1 < 0) {
		return;
	}

	i = 1;
	while (opt[i].name1 >= 0) {
		if (opt_cmp (&opt[i], &opt[i - 1]) >= 0) {
			i += 1;
			continue;
		}

		j = i - 1;

		tmp = opt[i];
		opt[i] = opt[j];

		while ((j > 0) && (opt_cmp (&tmp, &opt[j - 1]) < 0)) {
			opt[j] = opt[j - 1];
			j -= 1;
		}

		opt[j] = tmp;

		i += 1;
	}
}
