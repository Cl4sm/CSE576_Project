static void load_mount_filter_file(const char *filename)
{
	FILE *filter_file;
	if ((filter_file = fopen(filename, "r")) == NULL) {
		fprintf(stderr, "Failed to open filter file '%s'\n", filename);
		exit(1);
	}

	char *line = NULL;
	ssize_t llen;
	size_t lsize;
#ifdef HAVE_GETLINE
	while ((llen = getline(&line, &lsize, filter_file)) != -1)
#else				// HAVE_FGETLN
	while (line = fgetln(filter_file, &llen))
#endif
	{
		if (llen >= 1) {
			if (line[0] == '#')
				continue;

			if (line[llen - 1] == '\n') {
				line[llen - 1] = '\0';
				llen--;
			}
		}

		if (llen > 0)
			add_mount_filter(line);
	}

	free(line);

	fclose(filter_file);
}