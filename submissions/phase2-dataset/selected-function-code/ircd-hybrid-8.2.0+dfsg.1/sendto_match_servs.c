sendto_match_servs(struct Client *source_p, const char *mask, unsigned int cap,
                   const char *pattern, ...)
{
  va_list args;
  dlink_node *ptr = NULL, *ptr_next = NULL;
  struct dbuf_block *buffer;

  buffer = dbuf_alloc();

  dbuf_put_fmt(buffer, ":%s ", source_p->id);
  va_start(args, pattern);
  send_format(buffer, pattern, args);
  va_end(args);

  ++current_serial;

  DLINK_FOREACH_SAFE(ptr, ptr_next, global_server_list.head)
  {
    struct Client *target_p = ptr->data;

    /* Do not attempt to send to ourselves, or the source */
    if (IsMe(target_p) || target_p->from == source_p->from)
      continue;

    if (target_p->from->localClient->serial == current_serial)
      continue;

    if (!match(mask, target_p->name))
    {
      /*
       * if we set the serial here, then we'll never do a
       * match() again, if !IsCapable()
       */
      target_p->from->localClient->serial = current_serial;

      if (!IsCapable(target_p->from, cap))
        continue;

      send_message_remote(target_p->from, source_p, buffer);
    }
  }

  dbuf_ref_free(buffer);
}
