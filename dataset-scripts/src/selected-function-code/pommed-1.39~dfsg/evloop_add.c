int
evloop_add(int fd, uint32_t events, pommed_event_cb cb)
{
  int ret;

  struct epoll_event epoll_ev;
  struct pommed_event *pommed_ev;

  pommed_ev = (struct pommed_event *)malloc(sizeof(*pommed_ev));

  if (pommed_ev == NULL)
    {
      logmsg(LOG_ERR, "Could not allocate memory for new source");

      return -1;
    }

  pommed_ev->fd = fd;
  pommed_ev->cb = cb;
  pommed_ev->next = sources;

  epoll_ev.events = events;
  epoll_ev.data.ptr = pommed_ev;

  ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &epoll_ev);

  if (ret < 0)
    {
      logmsg(LOG_ERR, "Could not add source to epoll: %s", strerror(errno));

      free(pommed_ev);
      return -1;
    }

  sources = pommed_ev;

  return 0;
}