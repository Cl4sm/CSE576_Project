static const char *get_sub(void *state)
{
	subs_list_state *s = (subs_list_state *)state;
	char *filename;
	struct dirent *dp;

	if (s == NULL) return NULL;
	if (s->dirp == NULL) return NULL;

	if (s->line != NULL) {
		myfree(s->line);
		s->line = NULL;
	}

	for (;;) {
		if (s->fd == -1) {
			dp = readdir(s->dirp);
			if (dp == NULL) {
				closedir(s->dirp);
				s->dirp = NULL;
				return NULL;
			}
			if ((strcmp(dp->d_name, "..") == 0) ||
					(strcmp(dp->d_name, ".") == 0))
					continue;
			filename = concatstr(2, s->dirname, dp->d_name);
			s->fd = open(filename, O_RDONLY);
			if(s->fd < 0) {
				log_error(LOG_ARGS,
						"Could not open %s for reading",
						filename);
				myfree(filename);
				continue;
			}
			myfree(filename);
		}
		s->line = mygetline(s->fd);
		if (s->line == NULL) {
			close(s->fd);
			s->fd = -1;
			continue;
		}
		chomp(s->line);
		return s->line;
	}
}