int clean_nolongerbouncing(const char *listdir)
{
	DIR *bouncedir;
	char *dirname = concatstr(2, listdir, "/bounce/");
	char *filename, *probetimestr, *s;
	int probefd;
	time_t probetime, t;
	struct dirent *dp;
	struct stat st;
	
	if(chdir(dirname) < 0) {
		log_error(LOG_ARGS, "Could not chdir(%s)", dirname);
		myfree(dirname);
		return 1;
	}
		
	if((bouncedir = opendir(dirname)) == NULL) {
		log_error(LOG_ARGS, "Could not opendir(%s)", dirname);
		myfree(dirname);
		return 1;
	}

	myfree(dirname);

	while((dp = readdir(bouncedir)) != NULL) {
		if((strcmp(dp->d_name, "..") == 0) ||
		   (strcmp(dp->d_name, ".") == 0))
				continue;

		filename = mystrdup(dp->d_name);

		s = strrchr(filename, '-');
		if(s && (strcmp(s, "-probe") == 0)) {
			if(stat(filename, &st) < 0) {
				log_error(LOG_ARGS, "Could not stat(%s)",
					  filename);
				myfree(filename);
				continue;
			}

			probefd = open(filename, O_RDONLY);
			if(probefd < 0) {
				myfree(filename);
				continue;
			}
			probetimestr = mygetline(probefd);
			if(probetimestr == NULL) {
				myfree(filename);
				continue;
			}
			close(probefd);
			chomp(probetimestr);
			probetime = (time_t)strtol(probetimestr, NULL, 10);
			myfree(probetimestr);
			t = time(NULL);
			if(t - probetime > WAITPROBE) {
				unlink(filename);
				/* remove -probe onwards from filename */
				*s = '\0';
				unlink(filename);
				s = concatstr(2, filename, ".lastmsg");
				unlink(s);
				myfree(s);
			}
		}
		myfree(filename);
	}

	closedir(bouncedir);

	return 0;
}