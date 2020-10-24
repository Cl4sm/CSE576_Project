close_connection(struct Client *client_p)
{
  dlink_node *ptr = NULL;

  assert(client_p);

  if (!IsDead(client_p))
  {
    /* attempt to flush any pending dbufs. Evil, but .. -- adrian */
    /* there is still a chance that we might send data to this socket
     * even if it is marked as blocked (COMM_SELECT_READ handler is called
     * before COMM_SELECT_WRITE). Let's try, nothing to lose.. -adx
     */
    DelFlag(client_p, FLAGS_BLOCKED);
    send_queued_write(client_p);
  }

  if (IsClient(client_p))
  {
    ++ServerStats.is_cl;
    ServerStats.is_cbs += client_p->localClient->send.bytes;
    ServerStats.is_cbr += client_p->localClient->recv.bytes;
    ServerStats.is_cti += CurrentTime - client_p->localClient->firsttime;
  }
  else if (IsServer(client_p))
  {
    ++ServerStats.is_sv;
    ServerStats.is_sbs += client_p->localClient->send.bytes;
    ServerStats.is_sbr += client_p->localClient->recv.bytes;
    ServerStats.is_sti += CurrentTime - client_p->localClient->firsttime;

    DLINK_FOREACH(ptr, server_items.head)
    {
      struct MaskItem *conf = ptr->data;

      if (irccmp(conf->name, client_p->name))
        continue;

      /*
       * Reset next-connect cycle of all connect{} blocks that match
       * this servername.
       */
      conf->until = CurrentTime + conf->class->con_freq;
    }
  }
  else
    ++ServerStats.is_ni;

#ifdef HAVE_LIBCRYPTO
  if (client_p->localClient->fd.ssl)
  {
    SSL_set_shutdown(client_p->localClient->fd.ssl, SSL_RECEIVED_SHUTDOWN);

    if (!SSL_shutdown(client_p->localClient->fd.ssl))
      SSL_shutdown(client_p->localClient->fd.ssl);
  }
#endif
  if (client_p->localClient->fd.flags.open)
    fd_close(&client_p->localClient->fd);

  dbuf_clear(&client_p->localClient->buf_sendq);
  dbuf_clear(&client_p->localClient->buf_recvq);

  MyFree(client_p->localClient->password);
  client_p->localClient->password = NULL;

  detach_conf(client_p, CONF_CLIENT|CONF_OPER|CONF_SERVER);
}
