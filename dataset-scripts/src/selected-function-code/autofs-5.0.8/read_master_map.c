static int read_master_map(struct master *master, char *type, time_t age)
{
	unsigned int logopt = master->logopt;
	char *path, *save_name;
	int result;

	if (strcasecmp(type, "files")) {
		return do_read_master(master, type, age);
	}

	/* 
	 * This is a special case as we need to append the
	 * normal location to the map name.
	 * note: It's invalid to specify a relative path.
	 */

	if (strchr(master->name, '/')) {
		error(logopt, "relative path invalid in files map name");
		return NSS_STATUS_NOTFOUND;
	}

	path = malloc(strlen(AUTOFS_MAP_DIR) + strlen(master->name) + 2);
	if (!path)
		return NSS_STATUS_UNKNOWN;

	strcpy(path, AUTOFS_MAP_DIR);
	strcat(path, "/");
	strcat(path, master->name);

	save_name = master->name;
	master->name = path;

	result = do_read_master(master, type, age);

	master->name = save_name;
	free(path);

	return result;
}