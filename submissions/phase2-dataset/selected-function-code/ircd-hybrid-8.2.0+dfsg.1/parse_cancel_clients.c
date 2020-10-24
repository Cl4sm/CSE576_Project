static void
parse_cancel_clients(struct Client *client_p, struct Client *source_p, char *cmd)
{
  if (IsServer(source_p) || IsMe(source_p))
  {
    sendto_realops_flags(UMODE_DEBUG, L_ADMIN, SEND_NOTICE,
                         "Message for %s[%s] from %s",
                         source_p->name, source_p->from->name,
                         get_client_name(client_p, SHOW_IP));
    sendto_realops_flags(UMODE_DEBUG, L_OPER, SEND_NOTICE,
                         "Message for %s[%s] from %s",
                         source_p->name, source_p->from->name,
                         get_client_name(client_p, MASK_IP));
    sendto_realops_flags(UMODE_DEBUG, L_ALL, SEND_NOTICE,
                         "Not dropping server %s (%s) for Fake Direction",
                         client_p->name, source_p->name);
    return;
  }

  sendto_realops_flags(UMODE_DEBUG, L_ADMIN, SEND_NOTICE,
                       "Message for %s[%s@%s!%s] from %s (TS, ignored)",
                       source_p->name, source_p->username, source_p->host,
                       source_p->from->name, get_client_name(client_p, SHOW_IP));
  sendto_realops_flags(UMODE_DEBUG, L_OPER, SEND_NOTICE,
                       "Message for %s[%s@%s!%s] from %s (TS, ignored)",
                       source_p->name, source_p->username, source_p->host,
                       source_p->from->name, get_client_name(client_p, MASK_IP));
}
