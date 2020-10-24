static int include_file(struct master *master, time_t age, struct lookup_context* ctxt, struct dirent *e)
{
	unsigned int logopt = master->logopt;
	char included_path[PATH_MAX + 1];
	int included_path_len;
	char *save_name;
	int status;

	included_path_len = snprintf(included_path,
				     PATH_MAX + 1,
				     "%s/%s",
				     ctxt->mapname,
				     e->d_name);
	if (included_path_len > PATH_MAX)
		return NSS_STATUS_NOTFOUND;

	save_name = master->name;
	master->name = included_path;

	master->depth++;
	debug(logopt, MODPREFIX "include: %s", master->name);
	status = lookup_nss_read_master(master, age);
	if (!status) {
		warn(logopt,
		     MODPREFIX
		     "failed to read included master map %s",
		     master->name);
	}
	master->depth--;

	master->name = save_name;
	return NSS_STATUS_SUCCESS;
}