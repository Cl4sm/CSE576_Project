check_client(struct Client *source_p)
{
  int i;

  if ((i = verify_access(source_p)))
    ilog(LOG_TYPE_IRCD, "Access denied: %s[%s]",
         source_p->name, source_p->sockhost);

  switch (i)
  {
    case TOO_MANY:
      sendto_realops_flags(UMODE_FULL, L_ALL, SEND_NOTICE,
                           "Too many on IP for %s (%s).",
                           get_client_name(source_p, SHOW_IP),
                           source_p->sockhost);
      ilog(LOG_TYPE_IRCD, "Too many connections on IP from %s.",
           get_client_name(source_p, SHOW_IP));
      ++ServerStats.is_ref;
      exit_client(source_p, "No more connections allowed on that IP");
      break;

    case I_LINE_FULL:
      sendto_realops_flags(UMODE_FULL, L_ALL, SEND_NOTICE,
                           "auth{} block is full for %s (%s).",
                           get_client_name(source_p, SHOW_IP),
                           source_p->sockhost);
      ilog(LOG_TYPE_IRCD, "Too many connections from %s.",
           get_client_name(source_p, SHOW_IP));
      ++ServerStats.is_ref;
      exit_client(source_p, "No more connections allowed in your connection class");
      break;

    case NOT_AUTHORIZED:
      ++ServerStats.is_ref;
      /* jdc - lists server name & port connections are on */
      /*       a purely cosmetical change */
      sendto_realops_flags(UMODE_UNAUTH, L_ALL, SEND_NOTICE,
                           "Unauthorized client connection from %s [%s] on [%s/%u].",
                           get_client_name(source_p, SHOW_IP),
                           source_p->sockhost,
                           source_p->localClient->listener->name,
                           source_p->localClient->listener->port);
      ilog(LOG_TYPE_IRCD,
           "Unauthorized client connection from %s on [%s/%u].",
           get_client_name(source_p, SHOW_IP),
           source_p->localClient->listener->name,
           source_p->localClient->listener->port);

      exit_client(source_p, "You are not authorized to use this server");
      break;

   case BANNED_CLIENT:
     exit_client(source_p, "Banned");
     ++ServerStats.is_ref;
     break;

   case 0:
   default:
     break;
  }

  return (i < 0 ? 0 : 1);
}
