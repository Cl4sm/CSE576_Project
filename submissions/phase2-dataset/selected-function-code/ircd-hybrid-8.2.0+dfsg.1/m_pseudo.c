static int
m_pseudo(struct Client *source_p, int parc, char *parv[])
{
  char buffer[IRCD_BUFSIZE] = "";
  const struct pseudo_cmd *pseudo = (const struct pseudo_cmd *)parv[1];
  struct Client *target_p = NULL;
  struct Client *server_p = NULL;
  const char *msg = parv[parc - 1];

  if (parc < 3 || EmptyString(msg))
  {
    sendto_one_numeric(source_p, &me, ERR_NOTEXTTOSEND);
    return 0;
  }

  if (!EmptyString(pseudo->prep))
  {
    snprintf(buffer, sizeof(buffer), "%s%s", pseudo->prep, msg);
    msg = buffer;
  }

  target_p = find_person(source_p, pseudo->nick);
  server_p = hash_find_server(pseudo->serv);

  if (target_p && server_p && (target_p->servptr == server_p) && !IsMe(server_p))
  {
    sendto_one(target_p, ":%s PRIVMSG %s :%s",
               source_p->id, target_p->id, msg);
    return 0;
  }

  sendto_one_numeric(source_p, &me, ERR_SERVICESDOWN, pseudo->name);
  return 0;
}
