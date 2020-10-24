static void
send_message_remote(struct Client *to, struct Client *from, struct dbuf_block *buf)
{
  to = to->from;

  if (!MyConnect(to))
  {
    sendto_realops_flags(UMODE_ALL, L_ALL, SEND_NOTICE,
                         "Server send message to %s [%s] dropped from %s(Not local server)",
                         to->name, to->from->name, from->name);
    return;
  }

  /* Optimize by checking if (from && to) before everything */
  /* we set to->from up there.. */

  if (!MyClient(from) && IsClient(to) && (to == from->from))
  {
    if (IsServer(from))
    {
      sendto_realops_flags(UMODE_ALL, L_ALL, SEND_NOTICE,
                           "Send message to %s [%s] dropped from %s(Fake Dir)",
                           to->name, to->from->name, from->name);
      return;
    }

    sendto_realops_flags(UMODE_ALL, L_ALL, SEND_NOTICE,
                         "Ghosted: %s[%s@%s] from %s[%s@%s] (%s)",
                         to->name, to->username, to->host,
                         from->name, from->username, from->host,
                         to->from->name);

    sendto_server(NULL, NOCAPS, NOCAPS,
                  ":%s KILL %s :%s (%s[%s@%s] Ghosted %s)",
                  me.id, to->id, me.name, to->name,
                  to->username, to->host, to->from->name);

    AddFlag(to, FLAGS_KILLED);

    if (IsClient(from))
      sendto_one_numeric(from, &me, ERR_GHOSTEDCLIENT, to->name,
                         to->username, to->host, to->from);

    exit_client(to, "Ghosted client");
    return;
  }

  send_message(to, buf);
}
