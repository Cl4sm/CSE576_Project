int
evloop_iteration(void)
{
  int i;
  int nfds;

  struct epoll_event epoll_ev[MAX_EPOLL_EVENTS];
  struct pommed_event *pommed_ev;

  if (!running)
    return -1;

  nfds = epoll_wait(epfd, epoll_ev, MAX_EPOLL_EVENTS, -1);

  if (nfds < 0)
    {
      if (errno == EINTR)
	return 0; /* pommed.c will continue */
      else
	{
	  logmsg(LOG_ERR, "epoll_wait() error: %s", strerror(errno));

	  return -1; /* pommed.c will exit */
	}
    }

  for (i = 0; i < nfds; i++)
    {
      pommed_ev = epoll_ev[i].data.ptr;
      pommed_ev->cb(pommed_ev->fd, epoll_ev[i].events);
    }

  return nfds;
}