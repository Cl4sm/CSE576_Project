int
comm_open(fde_t *F, int family, int sock_type, int proto, const char *note)
{
  int fd;

  /* First, make sure we aren't going to run out of file descriptors */
  if (number_fd >= hard_fdlimit)
  {
    errno = ENFILE;
    return -1;
  }

  /*
   * Next, we try to open the socket. We *should* drop the reserved FD
   * limit if/when we get an error, but we can deal with that later.
   * XXX !!! -- adrian
   */
  fd = socket(family, sock_type, proto);
  if (fd < 0)
    return -1; /* errno will be passed through, yay.. */

  setup_socket(fd);

  /* update things in our fd tracking */
  fd_open(F, fd, 1, note);
  return 0;
}
