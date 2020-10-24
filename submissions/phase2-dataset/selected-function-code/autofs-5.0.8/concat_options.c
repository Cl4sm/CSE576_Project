static char *concat_options(char *left, char *right)
{
	char buf[MAX_ERR_BUF];
	char *ret;

	if (left == NULL || *left == '\0') {
		ret = strdup(right);
		free(right);
		return ret;
	}

	if (right == NULL || *right == '\0') {
		ret = strdup(left);
		free(left);
		return ret;
	}

	ret = malloc(strlen(left) + strlen(right) + 2);

	if (ret == NULL) {
		char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
		logerr(MODPREFIX "malloc: %s", estr);
		return NULL;
	}

	strcpy(ret, left);
	strcat(ret, ",");
	strcat(ret, right);

	free(left);
	free(right);

	return ret;
}