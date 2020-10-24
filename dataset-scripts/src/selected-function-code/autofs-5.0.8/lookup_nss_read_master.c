int lookup_nss_read_master(struct master *master, time_t age)
{
	unsigned int logopt = master->logopt;
	struct list_head nsslist;
	struct list_head *head, *p;
	int result = NSS_STATUS_UNKNOWN;

	/* If it starts with a '/' it has to be a file or LDAP map */
	if (*master->name == '/') {
		if (*(master->name + 1) == '/') {
			debug(logopt, "reading master ldap %s", master->name);
			result = do_read_master(master, "ldap", age);
		} else {
			debug(logopt, "reading master file %s", master->name);
			result = do_read_master(master, "file", age);
		}

		if (result == NSS_STATUS_UNAVAIL)
			master->read_fail = 1;

		return !result;
	} else {
		char *name = master->name;
		char *tmp;

		/* Old style name specification will remain I think. */
		tmp = strchr(name, ':');
		if (tmp) {
			char source[10];

			memset(source, 0, 10);
			if (!strncmp(name, "file:", 5) ||
			    !strncmp(name, "yp:", 3) ||
			    !strncmp(name, "nis:", 4) ||
			    !strncmp(name, "nisplus:", 8) ||
			    !strncmp(name, "ldap:", 5) ||
			    !strncmp(name, "ldaps:", 6) ||
			    !strncmp(name, "sss:", 4) ||
			    !strncmp(name, "dir:", 4)) {
				strncpy(source, name, tmp - name);

				/*
				 * If it's an ldap map leave the source in the
				 * name so the lookup module can work out if
				 * ldaps has been requested.
				 */
				if (strncmp(name, "ldap", 4)) {
					master->name = tmp + 1;
					debug(logopt, "reading master %s %s",
					      source, master->name);
				} else {
					master->name = name;
					debug(logopt, "reading master %s %s",
					      source, tmp + 1);
				}

				result = do_read_master(master, source, age);
				master->name = name;

				if (result == NSS_STATUS_UNAVAIL)
					master->read_fail = 1;

				return !result;
			}
		}
	}

	INIT_LIST_HEAD(&nsslist);

	result = nsswitch_parse(&nsslist);
	if (result) {
		if (!list_empty(&nsslist))
			free_sources(&nsslist);
		error(logopt, "can't to read name service switch config.");
		return 0;
	}

	/* First one gets it */
	head = &nsslist;
	list_for_each(p, head) {
		struct nss_source *this;
		int status;

		this = list_entry(p, struct nss_source, list);

		debug(logopt,
		      "reading master %s %s", this->source, master->name);

		result = read_master_map(master, this->source, age);

		/*
		 * If the name of the master map hasn't been explicitly
		 * configured and we're not reading an included master map
		 * then we're using auto.master as the default. Many setups
		 * also use auto_master as the default master map so we
		 * check for this map when auto.master isn't found.
		 */
		if (result != NSS_STATUS_SUCCESS &&
		    !master->depth && !defaults_master_set()) {
			char *tmp = strchr(master->name, '.');
			if (tmp) {
				debug(logopt,
				      "%s not found, replacing '.' with '_'",
				       master->name);
				*tmp = '_';
				result = read_master_map(master, this->source, age);
				if (result != NSS_STATUS_SUCCESS)
					*tmp = '.';
			}
		}

		if (result == NSS_STATUS_UNKNOWN) {
			debug(logopt, "no map - continuing to next source");
			continue;
		}

		if (result == NSS_STATUS_UNAVAIL)
			master->read_fail = 1;

		status = check_nss_result(this, result);
		if (status >= 0) {
			free_sources(&nsslist);
			return status;
		}
	}

	if (!list_empty(&nsslist))
		free_sources(&nsslist);

	return !result;
}