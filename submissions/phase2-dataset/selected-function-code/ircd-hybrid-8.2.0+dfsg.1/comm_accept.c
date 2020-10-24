int
comm_accept(struct Listener *lptr, struct irc_ssaddr *addr)
{
  int newfd;
  socklen_t addrlen = sizeof(struct irc_ssaddr);

  if (number_fd >= hard_fdlimit)
  {
    errno = ENFILE;
    return -1;
  }

  memset(addr, 0, sizeof(struct irc_ssaddr));

  /*
   * Next, do the accept(). if we get an error, we should drop the
   * reserved fd limit, but we can deal with that when comm_open()
   * also does it. XXX -- adrian
   */
  newfd = accept(lptr->fd.fd, (struct sockaddr *)addr, &addrlen);
  if (newfd < 0)
    return -1;

  remove_ipv6_mapping(addr);

  setup_socket(newfd);

  /* .. and return */
  return newfd;
}
