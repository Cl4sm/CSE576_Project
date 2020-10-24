int populate_root_dir(char *pop_cmd, struct list_t **dir_entry_listptr,
		      fuse_fill_dir_t filler, void *buf)
{
	FILE *browser;
	size_t hsize = 0;
	ssize_t hlen;
	char *dir_entry = NULL;

	if (!pop_cmd)
		return -1;

	if ((browser = popen(pop_cmd, "r")) == NULL) {
		fprintf(stderr, "Failed to execute populate_root_command=%s\n",
			pop_cmd);
		return -errno;
	}

	int loop_error = 0;
#ifdef HAVE_GETLINE
	while ((hlen = getline(&dir_entry, &hsize, browser)) != -1)
#else				// HAVE_FGETLN
	while (dir_entry = fgetln(browser, &hsize))
#endif
	{
		if (hlen >= 1 && dir_entry[hlen - 1] == '\n')
			dir_entry[hlen - 1] = '\0';

		fprintf(stderr, "Got entry \"%s\"\n", dir_entry);

		int insert_err =
		    insert_sorted_if_unique(dir_entry_listptr, dir_entry);
		if (insert_err == 1)	// already in list
			continue;
		else if (insert_err) {
			fprintf(stderr,
				"populate_root_command: failed on inserting new entry into sorted list.\n");
			loop_error = 1;
		}

		if (strlen(dir_entry) != 0)
			filler(buf, dir_entry, NULL, 0);
	}

	free(dir_entry);

	int pclose_err = pclose(browser);
	if (pclose_err) {
		int pclose_errno = errno;
		fprintf(stderr,
			"populate_root_command: pclose failed, ret %d, status %d, errno %d (%s)\n",
			pclose_errno, WEXITSTATUS(pclose_errno), pclose_errno,
			strerror(pclose_errno));
	}

	return loop_error || pclose_err;
}