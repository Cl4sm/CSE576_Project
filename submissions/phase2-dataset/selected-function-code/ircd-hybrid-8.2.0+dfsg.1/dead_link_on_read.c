dead_link_on_read(struct Client *client_p, int error)
{
  char errmsg[IRCD_BUFSIZE];
  int current_error;

  if (IsDefunct(client_p))
    return;

  dbuf_clear(&client_p->localClient->buf_recvq);
  dbuf_clear(&client_p->localClient->buf_sendq);

  current_error = get_sockerr(client_p->localClient->fd.fd);

  if (IsServer(client_p) || IsHandshake(client_p))
  {
    int connected = CurrentTime - client_p->localClient->firsttime;

    if (error == 0)
    {
      /* Admins get the real IP */
      sendto_realops_flags(UMODE_ALL, L_ADMIN, SEND_NOTICE,
                           "Server %s closed the connection",
                           get_client_name(client_p, SHOW_IP));

      /* Opers get a masked IP */
      sendto_realops_flags(UMODE_ALL, L_OPER, SEND_NOTICE,
                           "Server %s closed the connection",
                           get_client_name(client_p, MASK_IP));

      ilog(LOG_TYPE_IRCD, "Server %s closed the connection",
           get_client_name(client_p, SHOW_IP));
    }
    else
    {
      report_error(L_ADMIN, "Lost connection to %s: %s",
                   get_client_name(client_p, SHOW_IP), current_error);
      report_error(L_OPER, "Lost connection to %s: %s",
                   get_client_name(client_p, MASK_IP), current_error);
    }

    sendto_realops_flags(UMODE_ALL, L_ALL, SEND_NOTICE,
                         "%s had been connected for %d day%s, %2d:%02d:%02d",
                         client_p->name, connected/86400,
                         (connected/86400 == 1) ? "" : "s",
                         (connected % 86400) / 3600, (connected % 3600) / 60,
                         connected % 60);
  }

  if (error == 0)
    strlcpy(errmsg, "Remote host closed the connection",
            sizeof(errmsg));
  else
    snprintf(errmsg, sizeof(errmsg), "Read error: %s",
             strerror(current_error));

  exit_client(client_p, errmsg);
}
