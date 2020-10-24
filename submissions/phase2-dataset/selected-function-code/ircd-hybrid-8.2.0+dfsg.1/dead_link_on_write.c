void
dead_link_on_write(struct Client *client_p, int ierrno)
{
  dlink_node *ptr;

  if (IsDefunct(client_p))
    return;

  dbuf_clear(&client_p->localClient->buf_recvq);
  dbuf_clear(&client_p->localClient->buf_sendq);

  assert(dlinkFind(&abort_list, client_p) == NULL);
  ptr = make_dlink_node();
  /* don't let exit_aborted_clients() finish yet */
  dlinkAddTail(client_p, ptr, &abort_list);

  if (eac_next == NULL)
    eac_next = ptr;

  SetDead(client_p); /* You are dead my friend */
}
