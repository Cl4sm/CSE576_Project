free_client(struct Client *client_p)
{
  assert(client_p);
  assert(client_p != &me);
  assert(client_p->hnext == client_p);
  assert(client_p->idhnext == client_p);
  assert(client_p->channel.head == NULL);
  assert(dlink_list_length(&client_p->channel) == 0);
  assert(dlink_list_length(&client_p->whowas) == 0);
  assert(!IsServer(client_p) || client_p->serv);

  MyFree(client_p->serv);
  MyFree(client_p->certfp);

  if (MyConnect(client_p))
  {
    assert(client_p->localClient->invited.head == NULL);
    assert(dlink_list_length(&client_p->localClient->invited) == 0);
    assert(dlink_list_length(&client_p->localClient->watches) == 0);
    assert(IsClosing(client_p) && IsDead(client_p));

    MyFree(client_p->localClient->challenge_response);
    MyFree(client_p->localClient->challenge_operator);
    client_p->localClient->challenge_response = NULL;
    client_p->localClient->challenge_operator = NULL;

    /*
     * clean up extra sockets from P-lines which have been discarded.
     */
    if (client_p->localClient->listener)
    {
      listener_release(client_p->localClient->listener);
      client_p->localClient->listener = NULL;
    }

    dbuf_clear(&client_p->localClient->buf_recvq);
    dbuf_clear(&client_p->localClient->buf_sendq);

    mp_pool_release(client_p->localClient);
  }

  mp_pool_release(client_p);
}
