static int add_path(struct host *hosts, const char *path, int len)
{
	struct host *this;
	char *tmp, *tmp2;

	tmp = alloca(len + 1);
	if (!tmp)
		return 0;

	strncpy(tmp, path, len);
	tmp[len] = '\0';

	this = hosts;
	while (this) {
		if (!this->path) {
			tmp2 = strdup(tmp);
			if (!tmp2)
				return 0;
			this->path = tmp2;
		}
		this = this->next;
	}

	return 1;
}