static void
close_client(int fd) {
	dprintf(stderr, "close_client: fd%d\n", fd);
	close(fd);
	FD_CLR(fd, &Clients);
	if (WhosOn[fd]) {
		if (WhosOn[fd]->who) {
			char buf[TP_MAXVAR];

			sprintf(buf, "%s disconnected\07", WhosOn[fd]->who);
			broadcast((u_char*)buf, strlen(buf), TP_NOTICE);
			free(WhosOn[fd]->who);
		}
		free(WhosOn[fd]->host);
		if (WhosOn[fd]->auth)
			free(WhosOn[fd]->auth);
		free((char *) WhosOn[fd]);
		WhosOn[fd] = (struct whoson *) NULL;
	}
}
