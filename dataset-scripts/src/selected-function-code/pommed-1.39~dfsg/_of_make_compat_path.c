void _of_make_compat_path(const char *path, char *buf)
{
	size_t slen = strlen(path);
	int changed = 0;

	if (*path != '/')
		changed = 1;

	if(!strlen(path)) {
		snprintf(buf, PATH_MAX, "%s/", OF_ROOT);
		return;
	}

	if (path[slen - 1] != '/') {
		if (changed)
			snprintf(buf, PATH_MAX, "%s/%s/", OF_ROOT, path);
		else
			snprintf(buf, PATH_MAX, "%s%s/", OF_ROOT, path);
	} else 
		snprintf(buf, PATH_MAX, "%s%s", OF_ROOT, path);
}