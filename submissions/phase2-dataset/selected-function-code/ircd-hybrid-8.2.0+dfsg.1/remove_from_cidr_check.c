void
remove_from_cidr_check(struct irc_ssaddr *ip, struct ClassItem *aclass)
{
  dlink_node *ptr = NULL;
  dlink_node *next_ptr = NULL;
  struct CidrItem *cidr;

  if (aclass->number_per_cidr == 0)
    return;

  if (ip->ss.ss_family == AF_INET)
  {
    if (aclass->cidr_bitlen_ipv4 == 0)
      return;

    DLINK_FOREACH_SAFE(ptr, next_ptr, aclass->list_ipv4.head)
    {
      cidr = ptr->data;

      if (match_ipv4(ip, &cidr->mask, aclass->cidr_bitlen_ipv4))
      {
        cidr->number_on_this_cidr--;

        if (cidr->number_on_this_cidr == 0)
        {
          dlinkDelete(ptr, &aclass->list_ipv4);
          MyFree(cidr);
          return;
        }
      }
    }
  }
  else if (aclass->cidr_bitlen_ipv6 > 0)
  {
    DLINK_FOREACH_SAFE(ptr, next_ptr, aclass->list_ipv6.head)
    {
      cidr = ptr->data;

      if (match_ipv6(ip, &cidr->mask, aclass->cidr_bitlen_ipv6))
      {
        cidr->number_on_this_cidr--;

        if (cidr->number_on_this_cidr == 0)
        {
          dlinkDelete(ptr, &aclass->list_ipv6);
          MyFree(cidr);
          return;
        }
      }
    }
  }
}
