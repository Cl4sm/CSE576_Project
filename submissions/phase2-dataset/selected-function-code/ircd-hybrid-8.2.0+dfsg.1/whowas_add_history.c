whowas_add_history(struct Client *client_p, const int online)
{
  static unsigned int whowas_next = 0;
  struct Whowas *const who = &WHOWAS[whowas_next];

  assert(IsClient(client_p));

  if (++whowas_next == NICKNAMEHISTORYLENGTH)
    whowas_next = 0;

  if (who->hashv != -1)
  {
    if (who->online)
      dlinkDelete(&who->cnode, &who->online->whowas);

    dlinkDelete(&who->tnode, &WHOWASHASH[who->hashv]);
  }

  who->hashv = strhash(client_p->name);
  who->shide = IsHidden(client_p->servptr) != 0;
  who->logoff = CurrentTime;

  strlcpy(who->name, client_p->name, sizeof(who->name));
  strlcpy(who->username, client_p->username, sizeof(who->username));
  strlcpy(who->hostname, client_p->host, sizeof(who->hostname));
  strlcpy(who->realname, client_p->info, sizeof(who->realname));
  strlcpy(who->servername, client_p->servptr->name, sizeof(who->servername));

  if (online)
  {
    who->online = client_p;
    dlinkAdd(who, &who->cnode, &client_p->whowas);
  }
  else
    who->online = NULL;

  dlinkAdd(who, &who->tnode, &WHOWASHASH[who->hashv]);
}
