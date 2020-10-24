char **add_argv(int argc, char **argv, char *str)
{
	char **vector;
	size_t vector_size;
	int i;

	vector_size = (argc + 1) * sizeof(char *);
	vector = (char **) malloc(vector_size);
	if (!vector)
		return NULL;

	for (i = 0; i < argc - 1; i++) {
		if (argv[i]) {
			vector[i] = strdup(argv[i]);
			if (!vector[i]) {
				logerr("failed to strdup arg");
				break;
			}
		} else
			vector[i] = NULL;
	}

	if (i < argc - 1) {
		free_argv(argc - 1, (const char **) vector);
		return NULL;
	}

	vector[argc - 1] = strdup(str);
	if (!vector[argc - 1]) {
		free_argv(argc - 1, (const char **) vector);
		return NULL;
	}

	vector[argc] = NULL;

	free_argv(argc - 1, (const char **) argv);

	return vector;
}