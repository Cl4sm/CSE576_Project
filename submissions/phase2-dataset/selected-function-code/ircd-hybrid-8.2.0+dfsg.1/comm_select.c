void
comm_select(void)
{
  int num, i;
  struct pollfd pollfds[128];
  struct dvpoll dopoll;
  void (*hdl)(fde_t *, void *);
  fde_t *F;

  dopoll.dp_timeout = SELECT_DELAY;
  dopoll.dp_nfds = 128;
  dopoll.dp_fds = &pollfds[0];
  num = ioctl(dpfd.fd, DP_POLL, &dopoll);

  set_time();

  if (num < 0)
  {
#ifdef HAVE_USLEEP
    usleep(50000);  /* avoid 99% CPU in comm_select */
#endif
    return;
  }

  for (i = 0; i < num; i++)
  {
    F = lookup_fd(dopoll.dp_fds[i].fd);
    if (F == NULL || !F->flags.open)
      continue;

    if ((dopoll.dp_fds[i].revents & POLLIN))
    {
      if ((hdl = F->read_handler) != NULL)
      {
        F->read_handler = NULL;
        hdl(F, F->read_data);
        if (!F->flags.open)
          continue;
      }
    }

    if ((dopoll.dp_fds[i].revents & POLLOUT))
    {
      if ((hdl = F->write_handler) != NULL)
      {
        F->write_handler = NULL;
        hdl(F, F->write_data);
        if (!F->flags.open)
          continue;
      }
    }

    comm_setselect(F, 0, NULL, NULL, 0);
  }
}
