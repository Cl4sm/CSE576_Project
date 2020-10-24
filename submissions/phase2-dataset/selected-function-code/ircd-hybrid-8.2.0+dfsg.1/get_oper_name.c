const char *
get_oper_name(const struct Client *client_p)
{
  const dlink_node *cnode = NULL;
  /* +5 for !,@,{,} and null */
  static char buffer[NICKLEN + USERLEN + HOSTLEN + HOSTLEN + 5];

  if (MyConnect(client_p))
  {
    if ((cnode = client_p->localClient->confs.head))
    {
      const struct MaskItem *conf = cnode->data;

      if (IsConfOperator(conf))
      {
        snprintf(buffer, sizeof(buffer), "%s!%s@%s{%s}", client_p->name,
                 client_p->username, client_p->host, conf->name);
        return buffer;
      }
    }

    /*
     * Probably should assert here for now. If there is an oper out there
     * with no operator {} conf attached, it would be good for us to know...
     */
    assert(0);  /* Oper without oper conf! */
  }

  snprintf(buffer, sizeof(buffer), "%s!%s@%s{%s}", client_p->name,
           client_p->username, client_p->host, client_p->servptr->name);
  return buffer;
}
