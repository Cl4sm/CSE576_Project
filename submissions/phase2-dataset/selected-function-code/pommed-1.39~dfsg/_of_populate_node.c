struct device_node *_of_populate_node(const char *path, const char *name)
{
	struct device_node *tmp = calloc(1, sizeof(struct device_node));
	char *p = strdup(path);

	_of_remove_filename(p);

	tmp->full_path = strdup(p);

	if (name) 
		tmp->name = strdup(name);
	else
		_of_read_name(path, tmp);

	_of_get_path(p);

	tmp->path = strdup(p);
	free(p);

	_of_read_linux_phandle(tmp->full_path, tmp);
	_of_read_type(tmp->full_path, tmp);

	return tmp;
}