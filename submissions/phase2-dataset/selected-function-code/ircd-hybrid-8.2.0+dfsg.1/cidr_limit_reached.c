cidr_limit_reached(int over_rule, struct irc_ssaddr *ip, struct ClassItem *class)
{
  dlink_node *ptr = NULL;
  struct CidrItem *cidr = NULL;

  if (class->number_per_cidr == 0)
    return 0;

  if (ip->ss.ss_family == AF_INET)
  {
    if (class->cidr_bitlen_ipv4 == 0)
      return 0;

    DLINK_FOREACH(ptr, class->list_ipv4.head)
    {
      cidr = ptr->data;

      if (match_ipv4(ip, &cidr->mask, class->cidr_bitlen_ipv4))
      {
        if (!over_rule && (cidr->number_on_this_cidr >= class->number_per_cidr))
          return -1;

        cidr->number_on_this_cidr++;
        return 0;
      }
    }

    cidr = MyCalloc(sizeof(struct CidrItem));
    cidr->number_on_this_cidr = 1;
    cidr->mask = *ip;
    mask_addr(&cidr->mask, class->cidr_bitlen_ipv4);
    dlinkAdd(cidr, &cidr->node, &class->list_ipv4);
  }
  else if (class->cidr_bitlen_ipv6 > 0)
  {
    DLINK_FOREACH(ptr, class->list_ipv6.head)
    {
      cidr = ptr->data;

      if (match_ipv6(ip, &cidr->mask, class->cidr_bitlen_ipv6))
      {
        if (!over_rule && (cidr->number_on_this_cidr >= class->number_per_cidr))
          return -1;

        cidr->number_on_this_cidr++;
        return 0;
      }
    }

    cidr = MyCalloc(sizeof(struct CidrItem));
    cidr->number_on_this_cidr = 1;
    cidr->mask = *ip;
    mask_addr(&cidr->mask, class->cidr_bitlen_ipv6);
    dlinkAdd(cidr, &cidr->node, &class->list_ipv6);
  }

  return 0;
}
