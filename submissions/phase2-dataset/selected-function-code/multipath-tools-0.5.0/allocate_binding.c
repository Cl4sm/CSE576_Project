allocate_binding(int fd, char *wwid, int id, char *prefix)
{
	char buf[LINE_MAX];
	off_t offset;
	char *alias, *c;
	int i;

	if (id < 0) {
		condlog(0, "Bindings file full. Cannot allocate new binding");
		return NULL;
	}

	i = format_devname(buf, id, LINE_MAX, prefix);
	c = buf + i;
	snprintf(c,LINE_MAX - i, " %s\n", wwid);
	buf[LINE_MAX - 1] = '\0';

	offset = lseek(fd, 0, SEEK_END);
	if (offset < 0){
		condlog(0, "Cannot seek to end of bindings file : %s",
			strerror(errno));
		return NULL;
	}
	if (write_all(fd, buf, strlen(buf)) != strlen(buf)){
		condlog(0, "Cannot write binding to bindings file : %s",
			strerror(errno));
		/* clear partial write */
		if (ftruncate(fd, offset))
			condlog(0, "Cannot truncate the header : %s",
				strerror(errno));
		return NULL;
	}
	c = strchr(buf, ' ');
	*c = '\0';
	alias = strdup(buf);
	if (alias == NULL)
		condlog(0, "cannot copy new alias from bindings file : %s",
			strerror(errno));
	else
		condlog(3, "Created new binding [%s] for WWID [%s]", alias,
			wwid);
	return alias;
}
