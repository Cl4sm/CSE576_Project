static int rmdir_path_offset(struct autofs_point *ap, struct mapent *oe)
{
	char *dir, *path;
	unsigned int split;
	int ret;

	if (ap->type == LKP_DIRECT)
		return rmdir_path(ap, oe->key, oe->multi->dev);

	dir = strdup(oe->key);

	if (ap->flags & MOUNT_FLAG_GHOST)
		split = strlen(ap->path) + strlen(oe->multi->key) + 1;
	else
		split = strlen(ap->path);

	dir[split] = '\0';
	path = &dir[split + 1];

	if (chdir(dir) == -1) {
		error(ap->logopt, "failed to chdir to %s", dir);
		free(dir);
		return -1;
	}

	ret = rmdir_path(ap, path, ap->dev);

	free(dir);

	if (chdir("/") == -1)
		error(ap->logopt, "failed to chdir to /");

	return ret;
}