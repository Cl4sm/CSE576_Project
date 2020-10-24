int main(int argc, char **argv)
{
	int opt, ret, count = 0, docount = 0;
	char *listdir = NULL, *fileiter = NULL, *tmp;
	char *subddir, *subdir, *subfile = NULL;
	DIR *dirp;
	struct dirent *dp;
	enum subtype typesub = SUB_NORMAL;

	while ((opt = getopt(argc, argv, "cdhmnosVL:")) != -1) {
		switch(opt) {
		case 'c':
			docount = 1;
			break;
		case 'd':
			typesub = SUB_DIGEST;
			break;
		case 'h':
			print_help(argv[0]);
			break;
		case 'L':
			listdir = optarg;
			break;
		case 'm':
			typesub = SUB_FILE;
			subfile = "/control/moderators";
			break;
		case 'n':
			typesub = SUB_NOMAIL;
			break;
		case 'o':
			typesub = SUB_FILE;
			subfile = "/control/owner";
			break;
		case 'V':
			print_version(argv[0]);
			exit(EXIT_SUCCESS);
		default:
		case 's':
			typesub = SUB_NORMAL;
			break;
		}
	}

	if(listdir == NULL) {
		fprintf(stderr, "You have to specify -L\n"
				"%s -h for help\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	switch(typesub) {
		default:
		case SUB_NORMAL:
			subddir = "/subscribers.d/";
			break;
		case SUB_DIGEST:
			subddir = "/digesters.d/";
			break;
		case SUB_NOMAIL:
			subddir = "/nomailsubs.d/";
			break;
		case SUB_FILE:
			subddir = NULL;
			break;
	}
	
	if(subddir)
		subdir = concatstr(2, listdir, subddir);
	else
		subdir = NULL;

	if(subdir) {
		dirp = opendir(subdir);
		if(dirp == NULL) {
			fprintf(stderr, "Could not opendir(%s);\n", subdir);
			exit(EXIT_FAILURE);
		}
		while((dp = readdir(dirp)) != NULL) {
			if((strcmp(dp->d_name, "..") == 0) ||
					(strcmp(dp->d_name, ".") == 0))
				continue;

			fileiter = concatstr(2, subdir, dp->d_name);

			if(docount)
				dumpcount(fileiter, &count);
			else
				ret = dumpcount(fileiter, NULL);

			myfree(fileiter);
		}
		myfree(subdir);
		closedir(dirp);
	} else {
		tmp = concatstr(2, listdir, subfile);
		if(docount)
			dumpcount(tmp, &count);
		else
			dumpcount(tmp, NULL);
	}

	if(docount)
		printf("%d\n", count);


	return 0;
}