check_pings_list(dlink_list *list)
{
  char buf[IRCD_BUFSIZE] = "";
  int ping = 0;      /* ping time value from client */
  dlink_node *ptr = NULL, *ptr_next = NULL;

  DLINK_FOREACH_SAFE(ptr, ptr_next, list->head)
  {
    struct Client *client_p = ptr->data;

    if (IsDead(client_p))
      continue;  /* Ignore it, its been exited already */

    if (!IsRegistered(client_p))
      ping = CONNECTTIMEOUT;
    else
      ping = get_client_ping(&client_p->localClient->confs);

    if (ping < CurrentTime - client_p->localClient->lasttime)
    {
      if (!IsPingSent(client_p))
      {
        /*
         * If we haven't PINGed the connection and we haven't
         * heard from it in a while, PING it to make sure
         * it is still alive.
         */
        SetPingSent(client_p);
        client_p->localClient->lasttime = CurrentTime - ping;
        sendto_one(client_p, "PING :%s", ID_or_name(&me, client_p));
      }
      else
      {
        if (CurrentTime - client_p->localClient->lasttime >= 2 * ping)
        {
          /*
           * If the client/server hasn't talked to us in 2*ping seconds
           * and it has a ping time, then close its connection.
           */
          if (IsServer(client_p) || IsHandshake(client_p))
          {
            sendto_realops_flags(UMODE_ALL, L_ADMIN, SEND_NOTICE,
                                 "No response from %s, closing link",
                                 get_client_name(client_p, HIDE_IP));
            sendto_realops_flags(UMODE_ALL, L_OPER, SEND_NOTICE,
                                 "No response from %s, closing link",
                                 get_client_name(client_p, MASK_IP));
            ilog(LOG_TYPE_IRCD, "No response from %s, closing link",
                 get_client_name(client_p, HIDE_IP));
          }

          snprintf(buf, sizeof(buf), "Ping timeout: %d seconds",
                   (int)(CurrentTime - client_p->localClient->lasttime));
          exit_client(client_p, buf);
        }
      }
    }
  }
}
