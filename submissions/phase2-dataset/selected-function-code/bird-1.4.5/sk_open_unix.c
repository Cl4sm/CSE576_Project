int
sk_open_unix(sock *s, char *name)
{
  struct sockaddr_un sa;
  int fd;

  /* We are sloppy during error (leak fd and not set s->err), but we die anyway */

  fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (fd < 0)
    return -1;

  if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
    return -1;

  /* Path length checked in test_old_bird() */
  sa.sun_family = AF_UNIX;
  strcpy(sa.sun_path, name);

  if (bind(fd, (struct sockaddr *) &sa, SUN_LEN(&sa)) < 0)
    return -1;

  if (listen(fd, 8) < 0)
    return -1;

  s->fd = fd;
  sk_insert(s);
  return 0;
}
