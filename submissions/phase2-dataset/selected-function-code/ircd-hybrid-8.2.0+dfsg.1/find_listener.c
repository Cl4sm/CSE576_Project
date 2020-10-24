find_listener(int port, struct irc_ssaddr *addr)
{
  dlink_node *ptr;
  struct Listener *listener    = NULL;
  struct Listener *last_closed = NULL;

  DLINK_FOREACH(ptr, listener_list.head)
  {
    listener = ptr->data;

    if ((port == listener->port) &&
        (!memcmp(addr, &listener->addr, sizeof(struct irc_ssaddr))))
    {
      /* Try to return an open listener, otherwise reuse a closed one */
      if (!listener->fd.flags.open)
        last_closed = listener;
      else
        return (listener);
    }
  }

  return (last_closed);
}
