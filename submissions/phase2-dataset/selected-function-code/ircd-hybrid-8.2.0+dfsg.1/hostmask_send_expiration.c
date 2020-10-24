hostmask_send_expiration(struct AddressRec *arec)
{
  char ban_type = '\0';

  if (!ConfigGeneral.tkline_expire_notices)
    return;

  switch (arec->type)
  {
    case CONF_KLINE:
      ban_type = 'K';
      break;
    case CONF_DLINE:
      ban_type = 'D';
      break;
    case CONF_GLINE:
      ban_type = 'G';
      break;
    default: break;
  }

  sendto_realops_flags(UMODE_ALL, L_ALL, SEND_NOTICE,
                       "Temporary %c-line for [%s@%s] expired", ban_type,
                       (arec->conf->user) ? arec->conf->user : "*",
                       (arec->conf->host) ? arec->conf->host : "*");
}
