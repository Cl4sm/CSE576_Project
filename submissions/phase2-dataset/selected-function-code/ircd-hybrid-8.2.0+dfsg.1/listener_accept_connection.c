listener_accept_connection(fde_t *pfd, void *data)
{
  static time_t rate = 0;
  struct irc_ssaddr addr;
  int fd;
  int pe;
  struct Listener *listener = data;

  assert(listener);

  /* There may be many reasons for error return, but
   * in otherwise correctly working environment the
   * probable cause is running out of file descriptors
   * (EMFILE, ENFILE or others?). The man pages for
   * accept don't seem to list these as possible,
   * although it's obvious that it may happen here.
   * Thus no specific errors are tested at this
   * point, just assume that connections cannot
   * be accepted until some old is closed first.
   */
  while ((fd = comm_accept(listener, &addr)) != -1)
  {
    /*
     * check for connection limit
     */
    if (number_fd > hard_fdlimit - 10)
    {
      ++ServerStats.is_ref;
      sendto_realops_flags_ratelimited(&rate, "All connections in use. (%s)",
                                       get_listener_name(listener));

      if (!(listener->flags & LISTENER_SSL))
        send(fd, "ERROR :All connections in use\r\n", 32, 0);

      close(fd);
      break;    /* jump out and re-register a new io request */
    }

    /*
     * Do an initial check we aren't connecting too fast or with too many
     * from this IP...
     */
    if ((pe = conf_connect_allowed(&addr, addr.ss.ss_family)))
    {
      ++ServerStats.is_ref;

      if (!(listener->flags & LISTENER_SSL))
      {
        switch (pe)
        {
          case BANNED_CLIENT:
            send(fd, DLINE_WARNING, sizeof(DLINE_WARNING)-1, 0);
            break;
          case TOO_FAST:
            send(fd, TOOFAST_WARNING, sizeof(TOOFAST_WARNING)-1, 0);
            break;
        }
      }

      close(fd);
      continue;    /* drop the one and keep on clearing the queue */
    }

    ++ServerStats.is_ac;
    add_connection(listener, &addr, fd);
  }

  /* Re-register a new IO request for the next accept .. */
  comm_setselect(&listener->fd, COMM_SELECT_READ, listener_accept_connection,
                 listener, 0);
}
