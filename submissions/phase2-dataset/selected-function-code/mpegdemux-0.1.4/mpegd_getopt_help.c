{
	unsigned w;

	sort_options (opt);

	w = opt_max_width (opt);

	if (tag != NULL) {
		printf ("%s\n\n", tag);
	}

	if (usage != NULL) {
		printf ("%s\n", usage);
	}

	while (opt->name1 >= 0) {
		print_option (opt, w + 2);
		opt += 1;
	}
}
