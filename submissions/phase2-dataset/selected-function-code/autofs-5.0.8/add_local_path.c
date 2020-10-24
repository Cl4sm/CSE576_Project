static int add_local_path(struct host **hosts, const char *path)
{
	struct host *new;
	char *tmp;

	tmp = strdup(path);
	if (!tmp)
		return 0;

	new = malloc(sizeof(struct host));
	if (!new) {
		free(tmp);
		return 0;
	}

	memset(new, 0, sizeof(struct host));

	new->path = tmp;
	new->proximity = PROXIMITY_LOCAL;
	new->version = NFS_VERS_MASK;
	new->name = NULL;
	new->addr = NULL;
	new->weight = new->cost = 0;

	add_host(hosts, new);

	return 1;
}