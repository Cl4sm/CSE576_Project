static void
sockets_fire(struct birdloop *loop)
{
  struct pollfd *pfd = loop->poll_fd.data;
  sock **psk = loop->poll_sk.data;
  int poll_num = loop->poll_fd.used - 1;

  times_update(loop);

  /* Last fd is internal wakeup fd */
  if (pfd[loop->sock_num].revents & POLLIN)
    wakeup_drain(loop);

  int i;
  for (i = 0; i < poll_num; pfd++, psk++, i++)
  {
    int e = 1;

    if (! pfd->revents)
      continue;

    if (pfd->revents & POLLNVAL)
      die("poll: invalid fd %d", pfd->fd);

    if (pfd->revents & POLLIN)
      while (e && *psk && (*psk)->rx_hook)
	e = sk_read(*psk);

    e = 1;
    if (pfd->revents & POLLOUT)
      while (e && *psk)
	e = sk_write(*psk);
  }
}
