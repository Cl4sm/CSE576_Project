static void
sockets_prepare(struct birdloop *loop)
{
  BUFFER_SET(loop->poll_sk, loop->sock_num + 1);
  BUFFER_SET(loop->poll_fd, loop->sock_num + 1);

  struct pollfd *pfd = loop->poll_fd.data;
  sock **psk = loop->poll_sk.data;
  int i = 0;
  node *n;

  WALK_LIST(n, loop->sock_list)
  {
    sock *s = SKIP_BACK(sock, n, n);

    ASSERT(i < loop->sock_num);

    s->index = i;
    *psk = s;
    pfd->fd = s->fd;
    pfd->events = sk_want_events(s);
    pfd->revents = 0;

    pfd++;
    psk++;
    i++;
  }

  ASSERT(i == loop->sock_num);

  /* Add internal wakeup fd */
  *psk = NULL;
  pfd->fd = loop->wakeup_fds[0];
  pfd->events = POLLIN;
  pfd->revents = 0;

  loop->poll_changed = 0;
}
