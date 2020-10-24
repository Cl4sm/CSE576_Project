const char **copy_argv(int argc, const char **argv)
{
	char **vector;
	size_t vector_size;
	int i;

	vector_size = (argc + 1) * sizeof(char *);
	vector = (char **) malloc(vector_size);
	if (!vector)
		return NULL;

	for (i = 0; i < argc; i++) {
		if (argv[i]) {
			vector[i] = strdup(argv[i]);
			if (!vector[i]) {
				logerr("failed to strdup arg");
				break;
			}
		} else
			vector[i] = NULL;
	}

	if (i < argc) {
		free_argv(argc, (const char **) vector);
		return NULL;
	}

	vector[argc] = NULL;

	return (const char **) vector;

}