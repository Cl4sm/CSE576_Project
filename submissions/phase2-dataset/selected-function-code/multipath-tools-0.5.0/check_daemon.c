int check_daemon(void)
{
	int fd;
	char *reply;
	size_t len;
	int ret = 0;

	fd = ux_socket_connect(DEFAULT_SOCKET);
	if (fd == -1)
		return 0;

	if (send_packet(fd, "show daemon", 12) != 0)
		goto out;
	if (recv_packet(fd, &reply, &len) != 0)
		goto out;

	if (strstr(reply, "shutdown"))
		goto out_free;

	ret = 1;

out_free:
	FREE(reply);
out:
	close(fd);
	return ret;
}
