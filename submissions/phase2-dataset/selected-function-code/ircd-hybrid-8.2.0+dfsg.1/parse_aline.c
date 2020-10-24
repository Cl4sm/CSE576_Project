parse_aline(const char *cmd, struct Client *source_p,
            int parc, char **parv,
            int parse_flags, char **up_p, char **h_p, time_t *tkline_time,
            char **target_server, char **reason)
{
  int found_tkline_time=0;
  static char def_reason[] = CONF_NOREASON;
  static char user[USERLEN*4+1];
  static char host[HOSTLEN*4+1];

  parv++;
  parc--;

  found_tkline_time = valid_tkline(*parv, TK_MINUTES);

  if (found_tkline_time != 0)
  {
    parv++;
    parc--;

    if (tkline_time != NULL)
      *tkline_time = found_tkline_time;
    else
    {
      sendto_one_notice(source_p, &me, ":temp_line not supported by %s", cmd);
      return -1;
    }
  }

  if (parc == 0)
  {
    sendto_one_numeric(source_p, &me, ERR_NEEDMOREPARAMS, cmd);
    return -1;
  }

  if (h_p == NULL)
    *up_p = *parv;
  else
  {
    if (find_user_host(source_p, *parv, user, host, parse_flags) == 0)
      return -1;

    *up_p = user;
    *h_p = host;
  }

  parc--;
  parv++;

  if (parc != 0)
  {
    if (irccmp(*parv, "ON") == 0)
    {
      parc--;
      parv++;

      if (target_server == NULL)
      {
        sendto_one_notice(source_p, &me, ":ON server not supported by %s", cmd);
        return -1;
      }

      if (!HasOFlag(source_p, OPER_FLAG_REMOTEBAN))
      {
        sendto_one_numeric(source_p, &me, ERR_NOPRIVS, "remoteban");
        return -1;
      }

      if (parc == 0 || EmptyString(*parv))
      {
        sendto_one_numeric(source_p, &me, ERR_NEEDMOREPARAMS, cmd);
        return -1;
      }

      *target_server = *parv;
      parc--;
      parv++;
    }
    else
    {
      /* Make sure target_server *is* NULL if no ON server found
       * caller probably NULL'd it first, but no harm to do it again -db
       */
      if (target_server != NULL)
        *target_server = NULL;
    }
  }

  if (h_p != NULL)
  {
    if (strchr(user, '!') != NULL)
    {
      sendto_one_notice(source_p, &me, ":Invalid character '!' in kline");
      return -1;
    }

    if ((parse_flags & AWILD) && !valid_wild_card(source_p, 1, 2, *up_p, *h_p))
      return -1;
  }
  else
    if ((parse_flags & AWILD) && !valid_wild_card(source_p, 1, 1, *up_p))
      return -1;

  if (reason != NULL)
  {
    if (parc != 0 && !EmptyString(*parv))
    {
      *reason = *parv;
      if (!valid_comment(source_p, *reason, 1))
        return -1;
    }
    else
      *reason = def_reason;
  }

  return 1;
}
