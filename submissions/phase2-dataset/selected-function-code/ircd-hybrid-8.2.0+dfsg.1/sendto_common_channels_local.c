void
sendto_common_channels_local(struct Client *user, int touser, unsigned int cap,
                             const char *pattern, ...)
{
  va_list args;
  dlink_node *uptr;
  dlink_node *cptr;
  struct Channel *chptr;
  struct Membership *ms;
  struct Client *target_p;
  struct dbuf_block *buffer;

  buffer = dbuf_alloc();

  va_start(args, pattern);
  send_format(buffer, pattern, args);
  va_end(args);

  ++current_serial;

  DLINK_FOREACH(cptr, user->channel.head)
  {
    chptr = ((struct Membership *)cptr->data)->chptr;

    DLINK_FOREACH(uptr, chptr->locmembers.head)
    {
      ms = uptr->data;
      target_p = ms->client_p;

      if (target_p == user || IsDefunct(target_p) ||
          target_p->localClient->serial == current_serial)
        continue;

      if (HasCap(target_p, cap) != cap)
        continue;

      target_p->localClient->serial = current_serial;
      send_message(target_p, buffer);
    }
  }

  if (touser && MyConnect(user) && !IsDead(user) &&
      user->localClient->serial != current_serial)
    if (HasCap(user, cap) == cap)
      send_message(user, buffer);

  dbuf_ref_free(buffer);
}
