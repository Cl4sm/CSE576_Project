make_client(struct Client *from)
{
  struct Client *client_p = mp_pool_get(client_pool);

  if (!from)
  {
    client_p->from                      = client_p; /* 'from' of local client is self! */
    client_p->localClient               = mp_pool_get(lclient_pool);
    client_p->localClient->since        = CurrentTime;
    client_p->localClient->lasttime     = CurrentTime;
    client_p->localClient->firsttime    = CurrentTime;
    client_p->localClient->registration = REG_INIT;

    /* as good a place as any... */
    dlinkAdd(client_p, &client_p->localClient->lclient_node, &unknown_list);
  }
  else
    client_p->from = from; /* 'from' of local client is self! */

  client_p->idhnext = client_p;
  client_p->hnext  = client_p;
  SetUnknown(client_p);
  strcpy(client_p->username, "unknown");
  strcpy(client_p->svid, "0");

  return client_p;
}
