static void
update_client_exit_stats(struct Client *client_p)
{
  if (IsClient(client_p))
  {
    assert(Count.total > 0);
    --Count.total;
    if (HasUMode(client_p, UMODE_OPER))
      --Count.oper;
    if (HasUMode(client_p, UMODE_INVISIBLE))
      --Count.invisi;
  }
  else if (IsServer(client_p))
    sendto_realops_flags(UMODE_EXTERNAL, L_ALL, SEND_NOTICE,
                         "Server %s split from %s",
                         client_p->name, client_p->servptr->name);

  if (splitchecking && !splitmode)
    check_splitmode(NULL);
}
