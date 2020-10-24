sendto_match_butone(struct Client *one, struct Client *from, const char *mask,
                    int what, const char *pattern, ...)
{
  va_list alocal, aremote;
  dlink_node *ptr = NULL, *ptr_next = NULL;
  struct dbuf_block *local_buf, *remote_buf;

  local_buf = dbuf_alloc(), remote_buf = dbuf_alloc();

  dbuf_put_fmt(local_buf, ":%s!%s@%s ", from->name, from->username, from->host);
  dbuf_put_fmt(remote_buf, ":%s ", from->id);

  va_start(alocal, pattern);
  va_start(aremote, pattern);
  send_format(local_buf, pattern, alocal);
  send_format(remote_buf, pattern, aremote);
  va_end(aremote);
  va_end(alocal);

  /* scan the local clients */
  DLINK_FOREACH(ptr, local_client_list.head)
  {
    struct Client *client_p = ptr->data;

    if ((!one || client_p != one->from) && !IsDefunct(client_p) &&
        match_it(client_p, mask, what))
      send_message(client_p, local_buf);
  }

  /* Now scan servers */
  DLINK_FOREACH_SAFE(ptr, ptr_next, local_server_list.head)
  {
    struct Client *client_p = ptr->data;

    /*
     * The old code looped through every client on the
     * network for each server to check if the
     * server (client_p) has at least 1 client matching
     * the mask, using something like:
     *
     * for (target_p = GlobalClientList; target_p; target_p = target_p->next)
     *        if (IsRegisteredUser(target_p) &&
     *                        match_it(target_p, mask, what) &&
     *                        (target_p->from == client_p))
     *   vsendto_prefix_one(client_p, from, pattern, args);
     *
     * That way, we wouldn't send the message to
     * a server who didn't have a matching client.
     * However, on a network such as EFNet, that
     * code would have looped through about 50
     * servers, and in each loop, loop through
     * about 50k clients as well, calling match()
     * in each nested loop. That is a very bad
     * thing cpu wise - just send the message
     * to every connected server and let that
     * server deal with it.
     * -wnder
     */
    if ((!one || client_p != one->from) && !IsDefunct(client_p))
      send_message_remote(client_p, from, remote_buf);
  }

  dbuf_ref_free(local_buf);
  dbuf_ref_free(remote_buf);
}
