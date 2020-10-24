static void new_client(int ux_sock)
{
	struct client *c;
	struct sockaddr addr;
	socklen_t len = sizeof(addr);
	int fd;

	fd = accept(ux_sock, &addr, &len);

	if (fd == -1)
		return;

	/* put it in our linked list */
	c = (struct client *)MALLOC(sizeof(*c));
	memset(c, 0, sizeof(*c));
	c->fd = fd;
	c->next = clients;
	if (c->next) c->next->prev = c;
	clients = c;
	num_clients++;
}
