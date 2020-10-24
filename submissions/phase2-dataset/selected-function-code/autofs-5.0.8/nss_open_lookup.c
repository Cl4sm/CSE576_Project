static struct lookup_mod *nss_open_lookup(const char *format, int argc, const char **argv)
{
	struct list_head nsslist;
	struct list_head *head, *p;
	struct lookup_mod *mod;
	char buf[MAX_ERR_BUF], *estr;

	if (!argv || !argv[0])
		return NULL;

	if (*argv[0] == '/')
		return open_lookup("file", MODPREFIX, format, argc, argv);

	if (!strncmp(argv[0], "file", 4) ||
	    !strncmp(argv[0], "yp", 2) ||
	    !strncmp(argv[0], "nisplus", 7) ||
	    !strncmp(argv[0], "nis", 3) ||
	    !strncmp(argv[0], "ldaps", 5) ||
	    !strncmp(argv[0], "ldap", 4)) {
		const char *fmt = strchr(argv[0], ',');
		if (fmt)
			fmt++;
		else
			fmt = format;
		return open_lookup(argv[0], MODPREFIX, fmt, argc -1, argv + 1);
	}

	INIT_LIST_HEAD(&nsslist);

	if (nsswitch_parse(&nsslist)) {
		if (!list_empty(&nsslist))
			free_sources(&nsslist);
		logerr("can't to read name service switch config.");
		return NULL;
	}

	head = &nsslist;
	list_for_each(p, head) {
		struct nss_source *this;

		this = list_entry(p, struct nss_source, list);

		if (!strcmp(this->source, "files")) {
			char src_file[] = "file";
			char src_prog[] = "program";
			struct stat st;
			char *type, *path, *save_argv0;

			path = malloc(strlen(AUTOFS_MAP_DIR) + strlen(argv[0]) + 2);
			if (!path) {
				estr = strerror_r(errno, buf, MAX_ERR_BUF);
				logerr(MODPREFIX "error: %s", estr);
				free_sources(&nsslist);
				return NULL;
			}
			strcpy(path, AUTOFS_MAP_DIR);
			strcat(path, "/");
			strcat(path, argv[0]);

			if (stat(path, &st) == -1 || !S_ISREG(st.st_mode)) {
				free(path);
				continue;
			}

			if (st.st_mode & __S_IEXEC)
				type = src_prog;
			else
				type = src_file;

			save_argv0 = (char *) argv[0];
			argv[0] = path;

			mod = open_lookup(type, MODPREFIX, format, argc, argv);
			if (mod) {
				free_sources(&nsslist);
				free(save_argv0);
				return mod;
			}

			argv[0] = save_argv0;
			free(path);
		}

		mod = open_lookup(this->source, MODPREFIX, format, argc, argv);
		if (mod) {
			free_sources(&nsslist);
			return mod;
		}
	}
	free_sources(&nsslist);

	return NULL;
}