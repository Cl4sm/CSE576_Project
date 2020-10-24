int mount_autofs_indirect(struct autofs_point *ap, const char *root)
{
	time_t now = time(NULL);
	int status;
	int map;

	/* TODO: read map, determine map type is OK */
	if (lookup_nss_read_map(ap, NULL, now))
		lookup_prune_cache(ap, now);
	else {
		error(ap->logopt, "failed to read map for %s", ap->path);
		return -1;
	}

	status = do_mount_autofs_indirect(ap, root);
	if (status < 0)
		return -1;

	map = lookup_ghost(ap, root);
	if (map & LKP_FAIL) {
		if (map & LKP_DIRECT) {
			error(ap->logopt,
			      "bad map format,found direct, "
			      "expected indirect exiting");
		} else {
			error(ap->logopt, "failed to load map, exiting");
		}
		/* TODO: Process cleanup ?? */
		return -1;
	}

	if (map & LKP_NOTSUP)
		ap->flags &= ~MOUNT_FLAG_GHOST;

	return 0;
}