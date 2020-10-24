int ux_socket_connect(const char *name)
{
	int fd, len;
	struct sockaddr_un addr;

	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_LOCAL;
	addr.sun_path[0] = '\0';
	len = strlen(name) + 1 + sizeof(sa_family_t);
	strncpy(&addr.sun_path[1], name, len);

	fd = socket(AF_LOCAL, SOCK_STREAM, 0);
	if (fd == -1) {
		condlog(3, "Couldn't create ux_socket, error %d", errno);
		return -1;
	}

	if (connect(fd, (struct sockaddr *)&addr, len) == -1) {
		condlog(3, "Couldn't connect to ux_socket, error %d", errno);
		close(fd);
		return -1;
	}

	return fd;
}
