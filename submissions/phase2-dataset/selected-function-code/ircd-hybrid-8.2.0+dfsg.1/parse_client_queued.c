static void
parse_client_queued(struct Client *client_p)
{
  int dolen = 0;
  int checkflood = 1;
  struct LocalUser *lclient_p = client_p->localClient;

  if (IsUnknown(client_p))
  {
    int i = 0;

    while (1)
    {
      if (IsDefunct(client_p))
        return;

      /* rate unknown clients at MAX_FLOOD per loop */
      if (i >= MAX_FLOOD)
        break;

      dolen = extract_one_line(&lclient_p->buf_recvq, readBuf);
      if (dolen == 0)
        break;

      client_dopacket(client_p, readBuf, dolen);
      i++;

      /* if they've dropped out of the unknown state, break and move
       * to the parsing for their appropriate status.  --fl
       */
      if (!IsUnknown(client_p))
        break;
    }
  }

  if (IsServer(client_p) || IsConnecting(client_p) || IsHandshake(client_p))
  {
    while (1)
    {
      if (IsDefunct(client_p))
        return;
      if ((dolen = extract_one_line(&lclient_p->buf_recvq, readBuf)) == 0)
        break;

      client_dopacket(client_p, readBuf, dolen);
    }
  }
  else if (IsClient(client_p))
  {
    if (ConfigGeneral.no_oper_flood && (HasUMode(client_p, UMODE_OPER) || IsCanFlood(client_p)))
    {
      if (ConfigGeneral.true_no_oper_flood)
        checkflood = -1;
      else
        checkflood = 0;
    }

    /*
     * Handle flood protection here - if we exceed our flood limit on
     * messages in this loop, we simply drop out of the loop prematurely.
     *   -- adrian
     */
    while (1)
    {
      if (IsDefunct(client_p))
        break;

      /* This flood protection works as follows:
       *
       * A client is given allow_read lines to send to the server.  Every
       * time a line is parsed, sent_parsed is increased.  sent_parsed
       * is decreased by 1 every time flood_recalc is called.
       *
       * Thus a client can 'burst' allow_read lines to the server, any
       * excess lines will be parsed one per flood_recalc() call.
       *
       * Therefore a client will be penalised more if they keep flooding,
       * as sent_parsed will always hover around the allow_read limit
       * and no 'bursts' will be permitted.
       */
      if (checkflood > 0)
      {
        if (lclient_p->sent_parsed >= lclient_p->allow_read)
          break;
      }

      /* allow opers 4 times the amount of messages as users. why 4?
       * why not. :) --fl_
       */
      else if (lclient_p->sent_parsed >= (4 * lclient_p->allow_read) && checkflood != -1)
        break;

      dolen = extract_one_line(&lclient_p->buf_recvq, readBuf);
      if (dolen == 0)
        break;

      client_dopacket(client_p, readBuf, dolen);
      lclient_p->sent_parsed++;
    }
  }
}
