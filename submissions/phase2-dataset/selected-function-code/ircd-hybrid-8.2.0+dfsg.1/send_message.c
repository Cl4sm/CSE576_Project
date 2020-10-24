static void
send_message(struct Client *to, struct dbuf_block *buf)
{
  assert(!IsMe(to));
  assert(to != &me);

  if (dbuf_length(&to->localClient->buf_sendq) + buf->size > get_sendq(&to->localClient->confs))
  {
    if (IsServer(to))
      sendto_realops_flags(UMODE_ALL, L_ALL, SEND_NOTICE,
                           "Max SendQ limit exceeded for %s: %lu > %u",
                           get_client_name(to, HIDE_IP),
                           (unsigned long)(dbuf_length(&to->localClient->buf_sendq) + buf->size),
                           get_sendq(&to->localClient->confs));
    if (IsClient(to))
      SetSendQExceeded(to);

    dead_link_on_write(to, 0);
    return;
  }

  dbuf_add(&to->localClient->buf_sendq, buf);

  /*
   * Update statistics. The following is slightly incorrect because
   * it counts messages even if queued, but bytes only really sent.
   * Queued bytes get updated in send_queued_write().
   */
  ++to->localClient->send.messages;
  ++me.localClient->send.messages;

  send_queued_write(to);
}
