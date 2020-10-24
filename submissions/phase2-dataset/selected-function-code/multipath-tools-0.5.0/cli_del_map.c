int
cli_del_map (void * v, char ** reply, int * len, void * data)
{
	struct vectors * vecs = (struct vectors *)data;
	char * param = get_keyparam(v, MAP);
	int major, minor;
	char dev_path[PATH_SIZE];
	char *alias;
	int rc;

	param = convert_dev(param, 0);
	condlog(2, "%s: remove map (operator)", param);
	minor = dm_get_minor(param);
	if (minor < 0) {
		condlog(2, "%s: not a device mapper table", param);
		return 0;
	}
	major = dm_get_major(param);
	if (major < 0) {
		condlog(2, "%s: not a device mapper table", param);
		return 0;
	}
	sprintf(dev_path,"dm-%d", minor);
	alias = dm_mapname(major, minor);
	if (!alias) {
		condlog(2, "%s: mapname not found for %d:%d",
			param, major, minor);
		return 0;
	}
	rc = ev_remove_map(param, alias, minor, vecs);
	FREE(alias);
	return rc;
}
