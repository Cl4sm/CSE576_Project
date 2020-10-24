static int sun_mount(struct autofs_point *ap, const char *root,
			const char *name, int namelen,
			const char *loc, int loclen, const char *options,
			struct parse_context *ctxt)
{
	char *fstype = "nfs";	/* Default filesystem type */
	int nonstrict = 1;
	int use_weight_only = ap->flags & MOUNT_FLAG_USE_WEIGHT_ONLY;
	int rv, cur_state;
	char *mountpoint;
	char *what;
	char *type;

	if (*options == '\0')
		options = NULL;

	if (options) {
		char *noptions;
		const char *comma;
		char *np;
		int len = strlen(options) + 1;

		noptions = np = alloca(len);
		*np = '\0';

		/* Extract fstype= pseudo option */
		for (comma = options; *comma != '\0';) {
			const char *cp;

			while (*comma == ',')
				comma++;

			cp = comma;

			while (*comma != '\0' && *comma != ',')
				comma++;

			if (strncmp("fstype=", cp, 7) == 0) {
				int typelen = comma - (cp + 7);
				fstype = alloca(typelen + 1);
				memcpy(fstype, cp + 7, typelen);
				fstype[typelen] = '\0';
			} else if (strncmp("nonstrict", cp, 9) == 0) {
				nonstrict = 1;
			} else if (strncmp("strict", cp, 6) == 0) {
				nonstrict = 0;
			} else if (strncmp("nobrowse", cp, 8) == 0 ||
				   strncmp("browse", cp, 6) == 0 ||
				   strncmp("timeout=", cp, 8) == 0) {
				if (strcmp(fstype, "autofs") == 0 ||
				    strstr(cp, "fstype=autofs")) {
					memcpy(np, cp, comma - cp + 1);
					np += comma - cp + 1;
				}
			} else if (strncmp("no-use-weight-only", cp, 18) == 0) {
				use_weight_only = -1;
			} else if (strncmp("use-weight-only", cp, 15) == 0) {
				use_weight_only = MOUNT_FLAG_USE_WEIGHT_ONLY;
			} else if (strncmp("bg", cp, 2) == 0 ||
				   strncmp("nofg", cp, 4) == 0) {
				continue;
			} else {
				memcpy(np, cp, comma - cp + 1);
				np += comma - cp + 1;
			}
		}

		if (np > noptions + len) {
			warn(ap->logopt, MODPREFIX "options string truncated");
			np[len] = '\0';
		} else
			*(np - 1) = '\0';

		options = noptions;
	}

	if (!strcmp(fstype, "autofs") && ctxt->macros) {
		char *noptions = NULL;

		if (!options || *options == '\0') {
			noptions = alloca(strlen(ctxt->macros) + 1);
			*noptions = '\0';
		} else {
			int len = strlen(options) + strlen(ctxt->macros) + 2;
			noptions = alloca(len);

			if (noptions) {
				strcpy(noptions, options);
				strcat(noptions, ",");
			}
		}

		if (noptions && *noptions != '\0') {
			strcat(noptions, ctxt->macros);
			options = noptions;
		} else {
			error(ap->logopt,
			      MODPREFIX "alloca failed for options");
		}
	}

	mountpoint = alloca(namelen + 1);
	sprintf(mountpoint, "%.*s", namelen, name);

	type = ap->entry->maps->type;
	if (type && !strcmp(type, "hosts")) {
		if (options && *options != '\0') {
			int len = strlen(options);
			int suid = strstr(options, "suid") ? 0 : 7;
			int dev = strstr(options, "dev") ? 0 : 6;
			int nointr = strstr(options, "nointr") ? 0 : 5;

			if (suid || dev || nointr) {
				char *tmp = alloca(len + suid + dev + nointr + 1);
				if (!tmp) {
					error(ap->logopt, MODPREFIX
					      "alloca failed for options");
					if (nonstrict)
						return -1;
					return 1;
				}

				strcpy(tmp, options);
				if (suid)
					strcat(tmp, ",nosuid");
				if (dev)
					strcat(tmp, ",nodev");
				if (nointr)
					strcat(tmp, ",intr");
				options = tmp;
			}
		} else {
			char *tmp = alloca(18);
			if (!tmp) {
				error(ap->logopt,
				      MODPREFIX "alloca failed for options");
				if (nonstrict)
					return -1;
				return 1;
			}
			strcpy(tmp, "nosuid,nodev,intr");
			options = tmp;
		}
	}

	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cur_state);
	if (!strcmp(fstype, "nfs") || !strcmp(fstype, "nfs4")) {
		what = alloca(loclen + 1);
		memcpy(what, loc, loclen);
		what[loclen] = '\0';

		/* Add back "[no-]use-weight-only" for NFS mounts only */
		if (use_weight_only) {
			char *tmp;
			int len;

			if (options && *options != '\0') {
				len = strlen(options) + 19;
				tmp = alloca(len);
				strcpy(tmp, options);
				strcat(tmp, ",");
				if (use_weight_only == MOUNT_FLAG_USE_WEIGHT_ONLY)
					strcat(tmp, "use-weight-only");
				else
					strcat(tmp, "no-use-weight-only");
			} else {
				tmp = alloca(19);
				if (use_weight_only == MOUNT_FLAG_USE_WEIGHT_ONLY)
					strcpy(tmp, "use-weight-only");
				else
					strcpy(tmp, "no-use-weight-only");
			}
			options = tmp;
		}

		debug(ap->logopt, MODPREFIX
		      "mounting root %s, mountpoint %s, "
		      "what %s, fstype %s, options %s",
		      root, mountpoint, what, fstype, options);

		rv = mount_nfs->mount_mount(ap, root, mountpoint, strlen(mountpoint),
					    what, fstype, options, mount_nfs->context);
	} else {
		if (!loclen)
			what = NULL;
		else {
			what = alloca(loclen + 1);
			if (*loc == ':') {
				loclen--;
				memcpy(what, loc + 1, loclen);
				what[loclen] = '\0';
			} else {
				memcpy(what, loc, loclen);
				what[loclen] = '\0';
			}
		}

		debug(ap->logopt, MODPREFIX
		      "mounting root %s, mountpoint %s, "
		      "what %s, fstype %s, options %s",
		      root, mountpoint, what, fstype, options);

		/* Generic mount routine */
		rv = do_mount(ap, root, mountpoint, strlen(mountpoint), what, fstype,
			      options);
	}
	pthread_setcancelstate(cur_state, NULL);

	if (nonstrict && rv)
		return -rv;

	return rv;
}