bool run_template(const char *template, const char *mount_point,
		  const char *root_name)
{
	int len = 0;
	int nargs = 1;
	int i;
	char *buf;
	char *p;
	char **args;
	char **arg;
	bool quote = false;
	pid_t pid;
	int status;

	// calculate length
	for (i = 0; template[i]; i++)
		if (template[i] == '%') {
			switch (template[i + 1]) {
			case 'm':
				len += strlen(mount_point);
				i++;
				break;
			case 'r':
				len += strlen(root_name);
				i++;
				break;
			case '%':
				len++;
				i++;
				break;
			}
		} else if (template[i] == ' ' && !quote) {
			len++;
			nargs++;
		} else if (template[i] == '"')
			quote = !quote;
		else if (template[i] == '\\' && template[i + 1])
			len++, i++;
		else
			len++;

	buf = my_malloc(len + 1);
	args = my_malloc((nargs + 1) * sizeof(*args));

	p = buf;
	arg = args;
	*arg++ = p;

	for (i = 0; template[i]; i++)
		if (template[i] == '%') {
			switch (template[i + 1]) {
			case 'm':
				strcpy(p, mount_point);
				p += strlen(mount_point);
				i++;
				break;
			case 'r':
				strcpy(p, root_name);
				p += strlen(root_name);
				i++;
				break;
			case '%':
				*p++ = '%';
				i++;
				break;
			}
		} else if (template[i] == ' ' && !quote) {
			*p++ = '\0';
			*arg++ = p;
		} else if (template[i] == '"')
			quote = !quote;
		else if (template[i] == '\\' && template[i + 1])
			*p++ = template[++i];
		else
			*p++ = template[i];

	*p = '\0';
	*arg = NULL;

	pid = fork();
	if (pid == -1) {
		fprintf(stderr, "Failed to fork (%s)\n", strerror(errno));
		free(args);
		free(buf);
		return false;
	}
	if (pid == 0) {
		execvp(args[0], args);
		abort();
	}
	pid = waitpid(pid, &status, 0);
	if (pid == -1) {
		fprintf(stderr, "Failed to waitpid (%s)\n", strerror(errno));
		free(args);
		free(buf);
		return false;
	}
	if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
		fprintf(stderr, "Failed to invoke command: %s\n", args[0]);
		free(args);
		free(buf);
		return false;
	}
	free(args);
	free(buf);
	return true;
}