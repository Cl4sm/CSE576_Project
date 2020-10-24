struct master *master_new(const char *name, unsigned int timeout, unsigned int ghost)
{
	struct master *master;
	char *tmp;

	master = malloc(sizeof(struct master));
	if (!master)
		return NULL;

	if (!name)
		tmp = (char *) defaults_get_master_map();
	else
		tmp = strdup(name);

	if (!tmp) {
		free(master);
		return NULL;
	}

	master->name = tmp;
	master->nc = NULL;

	master->recurse = 0;
	master->depth = 0;
	master->reading = 0;
	master->read_fail = 0;
	master->default_ghost = ghost;
	master->default_timeout = timeout;
	master->default_logging = defaults_get_logging();
	master->logopt = master->default_logging;

	INIT_LIST_HEAD(&master->mounts);
	INIT_LIST_HEAD(&master->completed);

	return master;
}