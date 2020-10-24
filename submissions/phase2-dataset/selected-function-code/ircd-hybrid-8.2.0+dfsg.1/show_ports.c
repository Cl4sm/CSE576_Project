show_ports(struct Client *source_p)
{
  char buf[IRCD_BUFSIZE] = "";
  char *p = NULL;
  const dlink_node *ptr = NULL;

  DLINK_FOREACH(ptr, listener_list.head)
  {
    const struct Listener *listener = ptr->data;
    p = buf;

    if (listener->flags & LISTENER_HIDDEN)
    {
      if (!HasUMode(source_p, UMODE_ADMIN))
        continue;
      *p++ = 'H';
    }

    if (listener->flags & LISTENER_SERVER)
      *p++ = 'S';
    if (listener->flags & LISTENER_SSL)
      *p++ = 's';
    *p = '\0';


    if (HasUMode(source_p, UMODE_ADMIN) &&
        (MyClient(source_p) || !ConfigServerHide.hide_server_ips))
      sendto_one_numeric(source_p, &me, RPL_STATSPLINE, 'P', listener->port,
                         listener->name,
                         listener->ref_count, buf,
                         listener->active ? "active" : "disabled");
    else
      sendto_one_numeric(source_p, &me, RPL_STATSPLINE, 'P', listener->port,
                         me.name, listener->ref_count, buf,
                         listener->active ? "active" : "disabled");
  }
}
