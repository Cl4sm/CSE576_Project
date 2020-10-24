void
sendto_server(struct Client *one,
              const unsigned int caps,
              const unsigned int nocaps,
              const char *format, ...)
{
  va_list args;
  dlink_node *ptr = NULL;
  struct dbuf_block *buffer;

  buffer = dbuf_alloc();

  va_start(args, format);
  send_format(buffer, format, args);
  va_end(args);

  DLINK_FOREACH(ptr, local_server_list.head)
  {
    struct Client *client_p = ptr->data;

    /* If dead already skip */
    if (IsDead(client_p))
      continue;
    /* check against 'one' */
    if (one && (client_p == one->from))
      continue;
    /* check we have required capabs */
    if ((client_p->localClient->caps & caps) != caps)
      continue;
    /* check we don't have any forbidden capabs */
    if ((client_p->localClient->caps & nocaps))
      continue;

    send_message(client_p, buffer);
  }

  dbuf_ref_free(buffer);
}
