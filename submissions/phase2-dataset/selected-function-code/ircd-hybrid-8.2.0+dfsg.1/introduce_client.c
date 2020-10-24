static void
introduce_client(struct Client *source_p)
{
  dlink_node *ptr = NULL;
  char ubuf[IRCD_BUFSIZE] = "";

  if (MyClient(source_p))
    send_umode(source_p, source_p, 0, ubuf);
  else
    send_umode(NULL, source_p, 0, ubuf);

  watch_check_hash(source_p, RPL_LOGON);

  if (ubuf[0] == '\0')
  {
    ubuf[0] = '+';
    ubuf[1] = '\0';
  }

  DLINK_FOREACH(ptr, local_server_list.head)
  {
    struct Client *server = ptr->data;

    if (server == source_p->from)
      continue;

    if (IsCapable(server, CAP_SVS))
      sendto_one(server, ":%s UID %s %d %lu %s %s %s %s %s %s :%s",
                 source_p->servptr->id,
                 source_p->name, source_p->hopcount+1,
                 (unsigned long)source_p->tsinfo,
                 ubuf, source_p->username, source_p->host,
                 (MyClient(source_p) && IsIPSpoof(source_p)) ?
                 "0" : source_p->sockhost, source_p->id,
                 source_p->svid,
                 source_p->info);
    else
      sendto_one(server, ":%s UID %s %d %lu %s %s %s %s %s :%s",
                 source_p->servptr->id,
                 source_p->name, source_p->hopcount+1,
                 (unsigned long)source_p->tsinfo,
                 ubuf, source_p->username, source_p->host,
                 (MyClient(source_p) && IsIPSpoof(source_p)) ?
                 "0" : source_p->sockhost, source_p->id, source_p->info);

    if (!EmptyString(source_p->certfp))
      sendto_one(server, ":%s CERTFP %s", source_p->id, source_p->certfp);
  }
}
