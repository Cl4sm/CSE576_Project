void
delete_one_address_conf(const char *address, struct MaskItem *conf)
{
  int bits = 0;
  uint32_t hv = 0;
  dlink_node *ptr = NULL;
  struct irc_ssaddr addr;

  switch (parse_netmask(address, &addr, &bits))
  {
    case HM_IPV4:
      /* We have to do this, since we do not re-hash for every bit -A1kmm. */
      bits -= bits % 8;
      hv = hash_ipv4(&addr, bits);
      break;
    case HM_IPV6:
      /* We have to do this, since we do not re-hash for every bit -A1kmm. */
      bits -= bits % 16;
      hv = hash_ipv6(&addr, bits);
      break;
    default: /* HM_HOST */
      hv = get_mask_hash(address);
      break;
  }

  DLINK_FOREACH(ptr, atable[hv].head)
  {
    struct AddressRec *arec = ptr->data;

    if (arec->conf == conf)
    {
      dlinkDelete(&arec->node, &atable[hv]);

      if (!conf->ref_count)
        conf_free(conf);

      MyFree(arec);
      return;
    }
  }
}
