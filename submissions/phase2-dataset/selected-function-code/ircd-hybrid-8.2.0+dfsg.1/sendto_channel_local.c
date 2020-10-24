void
sendto_channel_local(unsigned int type, int nodeaf, struct Channel *chptr,
                     const char *pattern, ...)
{
  va_list args;
  dlink_node *ptr = NULL;
  struct dbuf_block *buffer;

  buffer = dbuf_alloc();

  va_start(args, pattern);
  send_format(buffer, pattern, args);
  va_end(args);

  DLINK_FOREACH(ptr, chptr->locmembers.head)
  {
    struct Membership *ms = ptr->data;
    struct Client *target_p = ms->client_p;

    if (type && (ms->flags & type) == 0)
      continue;

    if (IsDefunct(target_p) ||
        (nodeaf && HasUMode(target_p, UMODE_DEAF)))
      continue;

    send_message(target_p, buffer);
  }

  dbuf_ref_free(buffer);
}
