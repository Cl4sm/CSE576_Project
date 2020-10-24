void
sendto_channel_butone(struct Client *one, struct Client *from,
                      struct Channel *chptr, unsigned int type,
                      const char *pattern, ...)
{
  va_list alocal, aremote;
  struct dbuf_block *local_buf, *remote_buf;
  dlink_node *ptr = NULL, *ptr_next = NULL;

  local_buf = dbuf_alloc(), remote_buf = dbuf_alloc();

  if (IsClient(from))
    dbuf_put_fmt(local_buf, ":%s!%s@%s ", from->name, from->username, from->host);
  else
    dbuf_put_fmt(local_buf, ":%s ", from->name);

  dbuf_put_fmt(remote_buf, ":%s ", from->id);

  va_start(alocal, pattern);
  va_start(aremote, pattern);
  send_format(local_buf, pattern, alocal);
  send_format(remote_buf, pattern, aremote);

  va_end(aremote);
  va_end(alocal);

  ++current_serial;

  DLINK_FOREACH_SAFE(ptr, ptr_next, chptr->members.head)
  {
    struct Membership *ms = ptr->data;
    struct Client *target_p = ms->client_p;

    assert(IsClient(target_p));

    if (IsDefunct(target_p) || HasUMode(target_p, UMODE_DEAF) ||
        (one && target_p->from == one->from))
      continue;

    if (type && (ms->flags & type) == 0)
      continue;

    if (MyConnect(target_p))
      send_message(target_p, local_buf);
    else if (target_p->from->localClient->serial != current_serial)
      send_message_remote(target_p->from, from, remote_buf);
    target_p->from->localClient->serial = current_serial;
  }

  dbuf_ref_free(local_buf);
  dbuf_ref_free(remote_buf);
}
