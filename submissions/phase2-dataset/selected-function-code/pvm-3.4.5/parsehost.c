int
parsehost(buf, hp)
	char *buf;
	struct hostd *hp;
{
	char *av[10];		/* parsed words */
	int ac;
	int err = 0;

	ac = sizeof(av)/sizeof(av[0]);
	if (acav(buf, &ac, av)) {
		pvmlogprintf("parsehost(): line too long\n");
		goto bad;
	}
	if (!ac)
		goto bad;

	/* add options to host descriptor */

	while (--ac > 0) {
		if (!strncmp(av[ac], "lo=", 3)) {
			if (hp->hd_login)
				PVM_FREE(hp->hd_login);
			hp->hd_login = STRALLOC(av[ac] + 3);
			continue;
		}
		if (!strncmp(av[ac], "dx=", 3)) {
			if (hp->hd_dpath)
				PVM_FREE(hp->hd_dpath);
			hp->hd_dpath = STRALLOC(av[ac] + 3);
			continue;
		}
		if (!strncmp(av[ac], "ep=", 3)) {
			if (hp->hd_epath)
				PVM_FREE(hp->hd_epath);
			hp->hd_epath = STRALLOC(av[ac] + 3);
			continue;
		}
		if (!strncmp(av[ac], "sp=", 3)) {
			hp->hd_speed = atoi(av[ac] + 3);
			hp->hd_flag |= HF_SPEED;
			continue;
		}
		if (!strncmp(av[ac], "bx=", 3)) {
			if (hp->hd_bpath)
				PVM_FREE(hp->hd_bpath);
			hp->hd_bpath = STRALLOC(av[ac] + 3);
			continue;
		}
		if (!strncmp(av[ac], "wd=", 3)) {
			if (hp->hd_wdir)
				PVM_FREE(hp->hd_wdir);
			hp->hd_wdir = STRALLOC(av[ac] + 3);
			continue;
		}
		if (!strncmp(av[ac], "so=", 3)) {
			if (hp->hd_sopts)
				PVM_FREE(hp->hd_sopts);
			hp->hd_sopts = STRALLOC(av[ac] + 3);
			continue;
		}
		if (!strncmp(av[ac], "ip=", 3)) {
			if (hp->hd_aname)
				PVM_FREE(hp->hd_aname);
			hp->hd_aname = STRALLOC(av[ac] + 3);
			continue;
		}
		if (!strncmp(av[ac], "id=", 3)) {
			if (hp->hd_vmid)
				PVM_FREE(hp->hd_vmid);
			hp->hd_vmid = STRALLOC(av[ac] + 3);
			continue;
		}
		pvmlogprintf("parsehost(): unknown option \"%s\"\n", av[ac]);
		err++;
	}
	if (err)
		goto bad;

	if (hp->hd_name)
		PVM_FREE(hp->hd_name);
	hp->hd_name = STRALLOC(av[0]);
	return 0;

bad:
	return -1;
}
