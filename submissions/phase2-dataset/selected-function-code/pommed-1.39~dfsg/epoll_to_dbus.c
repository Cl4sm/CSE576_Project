static int
epoll_to_dbus(uint32_t events)
{
  int flags;

  flags = 0;

  if (events & EPOLLIN)
    flags |= DBUS_WATCH_READABLE;

  if (events & EPOLLOUT)
    flags |= DBUS_WATCH_WRITABLE;

  if (events & EPOLLHUP)
    flags |= DBUS_WATCH_HANGUP;

  if (events & EPOLLERR)
    flags |= DBUS_WATCH_ERROR;

  return flags;
}