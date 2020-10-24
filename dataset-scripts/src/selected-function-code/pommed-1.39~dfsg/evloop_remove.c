int
evloop_remove(int fd)
{
  int ret;

  struct pommed_event *p;
  struct pommed_event *e;

  ret = epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);

  if (ret < 0)
    {
      logmsg(LOG_ERR, "Could not remove source from epoll: %s", strerror(errno));

      return -1;
    }

  for (p = NULL, e = sources; e != NULL; p = e, e = e->next)
    {
      if (e->fd != fd)
	continue;

      if (p != NULL)
	p->next = e->next;
      else
	sources = e->next;

      free(e);

      break;
    }

  return 0;
}