static char *make_fullpath(const char *root, const char *key)
{
	int l;
	char *path;

	if (*key == '/') {
		l = strlen(key) + 1;
		if (l > KEY_MAX_LEN)
			return NULL;
		path = malloc(l);
		if (!path)
			return NULL;
		strcpy(path, key);
	} else {
		l = strlen(key) + 1 + strlen(root) + 1;
		if (l > KEY_MAX_LEN)
			return NULL;
		path = malloc(l);
		if (!path)
			return NULL;
		sprintf(path, "%s/%s", root, key);
	}
	return path;
}