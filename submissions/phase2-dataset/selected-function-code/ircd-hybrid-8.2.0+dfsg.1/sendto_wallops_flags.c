void
sendto_wallops_flags(unsigned int flags, struct Client *source_p,
                     const char *pattern, ...)
{
  dlink_node *ptr = NULL;
  va_list args;
  struct dbuf_block *buffer;

  buffer = dbuf_alloc();

  if (IsClient(source_p))
    dbuf_put_fmt(buffer, ":%s!%s@%s WALLOPS :", source_p->name, source_p->username, source_p->host);
  else
    dbuf_put_fmt(buffer, ":%s WALLOPS :", source_p->name);

  va_start(args, pattern);
  send_format(buffer, pattern, args);
  va_end(args);

  DLINK_FOREACH(ptr, oper_list.head)
  {
    struct Client *client_p = ptr->data;
    assert(client_p->umodes & UMODE_OPER);

    if (HasUMode(client_p, flags) && !IsDefunct(client_p))
      send_message(client_p, buffer);
  }

  dbuf_ref_free(buffer);
}
