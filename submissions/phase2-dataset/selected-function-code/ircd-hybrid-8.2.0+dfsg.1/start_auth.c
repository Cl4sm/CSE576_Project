void
start_auth(struct Client *client_p)
{
  struct AuthRequest *auth = NULL;

  assert(client_p);

  auth = make_auth_request(client_p);
  SetInAuth(auth);
  dlinkAddTail(auth, &auth->node, &auth_pending_list);

  sendheader(client_p, REPORT_DO_DNS);

  SetDNSPending(auth);

  if (ConfigGeneral.disable_auth == 0)
  {
    SetDoingAuth(auth);
    start_auth_query(auth);
  }

  gethost_byaddr(auth_dns_callback, auth, &client_p->localClient->ip);
}
