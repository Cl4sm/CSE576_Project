struct MaskItem *
find_conf_by_address(const char *name, struct irc_ssaddr *addr, unsigned int type,
                     int fam, const char *username, const char *password, int do_match)
{
  unsigned int hprecv = 0;
  dlink_node *ptr = NULL;
  struct MaskItem *hprec = NULL;
  struct AddressRec *arec = NULL;
  int b;
  int (*cmpfunc)(const char *, const char *) = do_match ? match : irccmp;

  if (addr)
  {
    /* Check for IPV6 matches... */
    if (fam == AF_INET6)
    {
      for (b = 128; b >= 0; b -= 16)
      {
        DLINK_FOREACH(ptr, atable[hash_ipv6(addr, b)].head)
        {
          arec = ptr->data;

          if ((arec->type == type) &&
              arec->precedence > hprecv &&
              arec->masktype == HM_IPV6 &&
              match_ipv6(addr, &arec->Mask.ipa.addr,
                         arec->Mask.ipa.bits) &&
              (!username || !cmpfunc(arec->username, username)) &&
              (IsNeedPassword(arec->conf) || arec->conf->passwd == NULL ||
               match_conf_password(password, arec->conf)))
          {
            hprecv = arec->precedence;
            hprec = arec->conf;
          }
        }
      }
    }
    else if (fam == AF_INET)
    {
      for (b = 32; b >= 0; b -= 8)
      {
        DLINK_FOREACH(ptr, atable[hash_ipv4(addr, b)].head)
        {
          arec = ptr->data;

          if ((arec->type == type) &&
              arec->precedence > hprecv &&
              arec->masktype == HM_IPV4 &&
              match_ipv4(addr, &arec->Mask.ipa.addr,
                         arec->Mask.ipa.bits) &&
              (!username || !cmpfunc(arec->username, username)) &&
              (IsNeedPassword(arec->conf) || arec->conf->passwd == NULL ||
               match_conf_password(password, arec->conf)))
          {
            hprecv = arec->precedence;
            hprec = arec->conf;
          }
        }
      }
    }
  }

  if (name)
  {
    const char *p = name;

    while (1)
    {
        DLINK_FOREACH(ptr, atable[hash_text(p)].head)
        {
          arec = ptr->data;
          if ((arec->type == type) &&
            arec->precedence > hprecv &&
            (arec->masktype == HM_HOST) &&
            !cmpfunc(arec->Mask.hostname, name) &&
            (!username || !cmpfunc(arec->username, username)) &&
            (IsNeedPassword(arec->conf) || arec->conf->passwd == NULL ||
             match_conf_password(password, arec->conf)))
        {
          hprecv = arec->precedence;
          hprec = arec->conf;
        }
      }

      if ((p = strchr(p, '.')) == NULL)
        break;
      ++p;
    }

    DLINK_FOREACH(ptr, atable[0].head)
    {
      arec = ptr->data;

      if (arec->type == type &&
          arec->precedence > hprecv &&
          arec->masktype == HM_HOST &&
          !cmpfunc(arec->Mask.hostname, name) &&
          (!username || !cmpfunc(arec->username, username)) &&
          (IsNeedPassword(arec->conf) || arec->conf->passwd == NULL ||
           match_conf_password(password, arec->conf)))
      {
        hprecv = arec->precedence;
        hprec = arec->conf;
      }
    }
  }

  return hprec;
}
