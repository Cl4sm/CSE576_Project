char **append_argv(int argc1, char **argv1, int argc2, char **argv2)
{
	char **vector;
	size_t vector_size;
	int len, i, j;

	len = argc1 + argc2;
	vector_size = (len + 1) * sizeof(char *);
	vector = (char **) realloc(argv1, vector_size);
	if (!vector) {
		free_argv(argc1, (const char **) argv1);
		free_argv(argc2, (const char **) argv2);
		return NULL;
	}

	for (i = argc1, j = 0; i <= len; i++, j++) {
		if (argv2[j]) {
			vector[i] = strdup(argv2[j]);
			if (!vector[i]) {
				logerr("failed to strdup arg");
				break;
			}
		} else
			vector[i] = NULL;
	}

	if (i < len) {
		free_argv(len, (const char **) vector);
		free_argv(argc2, (const char **) argv2);
		return NULL;
	}

	vector[len] = NULL;

	free_argv(argc2, (const char **) argv2);

	return vector;
}