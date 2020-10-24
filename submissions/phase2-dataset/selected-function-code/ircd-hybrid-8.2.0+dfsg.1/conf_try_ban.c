conf_try_ban(struct Client *client_p, struct MaskItem *conf)
{
  const char *user_reason = NULL;  /* What is sent to user */
  const char *type_string = NULL;
  const char dline_string[] = "D-line";
  const char kline_string[] = "K-line";
  const char gline_string[] = "G-line";
  const char xline_string[] = "X-line";

  switch (conf->type)
  {
    case CONF_KLINE:
      if (IsExemptKline(client_p))
      {
        sendto_realops_flags(UMODE_ALL, L_ALL, SEND_NOTICE,
                             "KLINE over-ruled for %s, client is kline_exempt",
                             get_client_name(client_p, HIDE_IP));
        return;
      }

      type_string = kline_string;
      break;
    case CONF_DLINE:
      if (find_conf_by_address(NULL, &client_p->localClient->ip, CONF_EXEMPT,
                               client_p->localClient->aftype, NULL, NULL, 1))
        return;
      type_string = dline_string;
      break;
    case CONF_GLINE:
      if (IsExemptKline(client_p) ||
          IsExemptGline(client_p))
      {
        sendto_realops_flags(UMODE_ALL, L_ALL, SEND_NOTICE,
                             "GLINE over-ruled for %s, client is %sline_exempt",
                             get_client_name(client_p, HIDE_IP), IsExemptKline(client_p) ? "k" : "g");
        return;
      }

      type_string = gline_string;
      break;
    case CONF_XLINE:
      type_string = xline_string;
      ++conf->count;
      break;
    default:
      assert(0);
      break;
  }

  user_reason = conf->reason ? conf->reason : type_string;

  sendto_realops_flags(UMODE_ALL, L_ALL, SEND_NOTICE, "%s active for %s",
                       type_string, get_client_name(client_p, HIDE_IP));

  if (IsClient(client_p))
    sendto_one_numeric(client_p, &me, ERR_YOUREBANNEDCREEP, user_reason);

  exit_client(client_p, user_reason);
}
