add_conf_by_address(const unsigned int type, struct MaskItem *conf)
{
  const char *hostname = conf->host;
  const char *username = conf->user;
  static unsigned int prec_value = 0xFFFFFFFF;
  int bits = 0;
  struct AddressRec *arec = NULL;

  assert(type && !EmptyString(hostname));

  arec = MyCalloc(sizeof(struct AddressRec));
  arec->masktype = parse_netmask(hostname, &arec->Mask.ipa.addr, &bits);
  arec->Mask.ipa.bits = bits;
  arec->username = username;
  arec->conf = conf;
  arec->precedence = prec_value--;
  arec->type = type;

  switch (arec->masktype)
  {
    case HM_IPV4:
      /* We have to do this, since we do not re-hash for every bit -A1kmm. */
      bits -= bits % 8;
      dlinkAdd(arec, &arec->node, &atable[hash_ipv4(&arec->Mask.ipa.addr, bits)]);
      break;
    case HM_IPV6:
      /* We have to do this, since we do not re-hash for every bit -A1kmm. */
      bits -= bits % 16;
      dlinkAdd(arec, &arec->node, &atable[hash_ipv6(&arec->Mask.ipa.addr, bits)]);
      break;
    default: /* HM_HOST */
      arec->Mask.hostname = hostname;
      dlinkAdd(arec, &arec->node, &atable[get_mask_hash(hostname)]);
      break;
  }

  return arec;
}
