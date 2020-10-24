readhostfile(fn)
	char *fn;
{
	struct htab *htp;
	struct hostd *hp;
	FILE *ff = 0;
	char buf[512];		/* line buffer */
	int lnum = 0;		/* line counter */
	int err = 0;		/* error count */
	struct hostent *he;
	char *p;
	struct in_addr *my_in_addrs;
	int num_addrs;
	int i;
	int maxhostid = tidhmask >> (ffs(tidhmask) - 1);
	struct hostd *defaults = 0;

	htp = ht_new(1);

	/*
	* get list of this host's interfaces so we can detect ourself in file
	*/

	if (iflist(&my_in_addrs, &num_addrs) == -1 || num_addrs < 1) {
		pvmlogprintf("readhostfile() iflist failed\n");
		goto bail;
	}

	if (!(ff = fopen(fn, "r"))) {
		pvmlogprintf("readhostfile() %s: can't read\n", fn);
		goto bail;
	}

	/* parse each line of host file */

	while (fgets(buf, sizeof(buf), ff)) {
		lnum++;
		for (p = buf; *p && isspace(*p); p++);

		if (!*p || *p == '#')	/* leading '#' is comment */
			continue;

		if (lnum > maxhostid) {
			pvmlogprintf("readhostfile() %s: too many lines\n", fn);
			err++;
			break;
		}
		hp = hd_new(lnum);

		while (*p == '&' || *p == '$') {
			if (*p == '&')		/* leading '&' is no-start */
				hp->hd_flag |= HF_NOSTART;
			if (*p == '$')		/* leading '$' allows overloading host */
				hp->hd_flag |= HF_OVERLOAD;
			p++;
		}

		if (parsehost(p, hp)) {
			pvmlogprintf("readhostfile() %s %d: errors\n", fn, lnum);
			err++;
			goto badline;
		}

	/*
	* if host == "*", change the default options
	* instead of adding a new host to the list
	*/

		if (!strcmp(hp->hd_name, "*")) {
			if (defaults)
				hd_unref(defaults);
			defaults = hp;
			continue;
		}

	/* Set unspecified fields of hp to defaults */

		if (defaults)
			applydefaults(hp, defaults);

	/* look up ip addr */

		if (!(he = gethostbyname(hp->hd_aname ? hp->hd_aname : hp->hd_name))) {
			pvmlogprintf("readhostfile() %s %d: %s: can't gethostbyname\n",
				fn, lnum, hp->hd_name);
			err++;
			goto badline;
		}
		BCOPY(he->h_addr_list[0], (char*)&hp->hd_sad.sin_addr,
			sizeof(struct in_addr));

	/* mark master host to not start */

		if (!(hp->hd_flag & HF_OVERLOAD))
			for (i = num_addrs; i-- > 0; ) {
				if (BCMP((char*)&my_in_addrs[i], (char*)&hp->hd_sad.sin_addr,
						sizeof(struct in_addr)) == 0) {
					hp->hd_flag |= HF_NOSTART;
					break;
				}
			}

		ht_insert(htp, hp);
		hd_unref(hp);
		continue;

badline:
		hd_unref(hp);
	}
	if (err) {
		pvmlogprintf("readhostfile() %s: %d errors in hostfile\n", fn, err);
	}

	if (defaults) {
		defaults->hd_hostpart = 0;
		ht_insert(htp, defaults);
	}
	fclose(ff);
	return htp;

bail:
	if (defaults)
		hd_unref(defaults);
	if (ff)
		fclose(ff);
	ht_free(htp);
	return (struct htab*)0;
}
