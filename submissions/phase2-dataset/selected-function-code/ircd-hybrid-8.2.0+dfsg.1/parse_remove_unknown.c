parse_remove_unknown(struct Client *client_p, const char *lsender, char *lbuffer)
{
  /*
   * Do kill if it came from a server because it means there is a ghost
   * user on the other server which needs to be removed. -avalon
   * Tell opers about this. -Taner
   */
  /*
   * '[0-9]something'  is an ID      (KILL/SQUIT depending on its length)
   * 'nodots'          is a nickname (KILL)
   * 'no.dot.at.start' is a server   (SQUIT)
   */
  if ((IsDigit(*lsender) && strlen(lsender) <= IRC_MAXSID) || strchr(lsender, '.'))
  {
    sendto_realops_flags(UMODE_DEBUG, L_ADMIN, SEND_NOTICE,
                         "Unknown prefix (%s) from %s, Squitting %s",
                         lbuffer, get_client_name(client_p, SHOW_IP), lsender);
    sendto_realops_flags(UMODE_DEBUG, L_OPER, SEND_NOTICE,
                         "Unknown prefix (%s) from %s, Squitting %s",
                         lbuffer, client_p->name, lsender);
    sendto_one(client_p, ":%s SQUIT %s :(Unknown prefix (%s) from %s)",
               me.id, lsender, lbuffer, client_p->name);
  }
  else
    sendto_one(client_p, ":%s KILL %s :%s (Unknown Client)",
               me.id, lsender, me.name);
}
