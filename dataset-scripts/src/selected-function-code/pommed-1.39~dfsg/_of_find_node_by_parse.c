void _of_find_node_by_parse(char *path, const void *search, uint16_t type, int full)
{
	DIR *dir;
	struct dirent *tmp = NULL;
	char *directories[8192] = { NULL };
	char fullpath[PATH_MAX];
	int x = 0;
	struct stat fstats;
	struct device_node *node = NULL;
	
	lstat(path, &fstats);

	if (S_ISLNK(fstats.st_mode))
		return;

	if ((dir = opendir(path)) == NULL)
		return;

	while ((tmp = readdir(dir)) != NULL) {

		if ((strcmp(tmp->d_name, ".")) && (strcmp(tmp->d_name, ".."))) {

			if (!strcmp(path, "/"))
				strcat(strcpy(fullpath, "/"), tmp->d_name);
			else
				strcat(strcat(strcpy(fullpath, path), "/"),
				       tmp->d_name);

			if (type == OF_SEARCH_NAME) {
				if (!strcmp(tmp->d_name, "name")) {
					if ((node =
					     _of_get_name(fullpath,
							  search)) != NULL) {
						_n_array[++_n_idx] = node;
						_n_sem = 1;
						if(full==0)
							goto out;
					}
				}
			}

			if (type == OF_SEARCH_TYPE) {
				if (!strcmp(tmp->d_name, "device_type")) {
					if ((node =
					     _of_get_type(fullpath,
							  search)) != NULL) {
						_t_array[++_t_idx] = node;
						_t_sem = 1;
						if(full==0)
							goto out;
					}
				}
			}

			if (type == OF_SEARCH_PHDL) {
				if (!strcmp(tmp->d_name, "linux,phandle")) {
					if ((node =
					     _of_get_phandle(fullpath,
							     search)) != NULL) {
						_p_array[++_p_idx] = node;
						_p_sem = 1;
						goto out;
					}
				}
			}

			lstat(fullpath, &fstats);
			if (S_ISDIR(fstats.st_mode))
				directories[x++] = strdup(fullpath);
		}
	}

	x = 0;

	while (directories[x] != NULL) {
		_of_find_node_by_parse(directories[x], search, type, full);
	      out:
		free(directories[x++]);
	}

	closedir(dir);
}