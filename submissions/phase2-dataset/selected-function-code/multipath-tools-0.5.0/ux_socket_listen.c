{
	int fd, len;
#ifdef USE_SYSTEMD
	int num;
#endif
	struct sockaddr_un addr;

#ifdef USE_SYSTEMD
	num = sd_listen_fds(0);
	if (num > 1) {
		condlog(3, "sd_listen_fds returned %d fds", num);
		return -1;
	} else if (num == 1) {
		fd = SD_LISTEN_FDS_START + 0;
		condlog(3, "using fd %d from sd_listen_fds", fd);
		return fd;
	}
#endif
	fd = socket(AF_LOCAL, SOCK_STREAM, 0);
	if (fd == -1) {
		condlog(3, "Couldn't create ux_socket, error %d", errno);
		return -1;
	}

	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_LOCAL;
	addr.sun_path[0] = '\0';
	len = strlen(name) + 1 + sizeof(sa_family_t);
	strncpy(&addr.sun_path[1], name, len);

	if (bind(fd, (struct sockaddr *)&addr, len) == -1) {
		condlog(3, "Couldn't bind to ux_socket, error %d", errno);
		close(fd);
		return -1;
	}

	if (listen(fd, 10) == -1) {
		condlog(3, "Couldn't listen to ux_socket, error %d", errno);
		close(fd);
		return -1;
	}
	return fd;
}
