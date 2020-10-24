struct ip_entry *
ipcache_find_or_add_address(struct irc_ssaddr *addr)
{
  dlink_node *ptr = NULL;
  struct ip_entry *iptr = NULL;
  uint32_t hash_index = ipcache_hash_address(addr);
  struct sockaddr_in *v4 = (struct sockaddr_in *)addr, *ptr_v4;
  struct sockaddr_in6 *v6 = (struct sockaddr_in6 *)addr, *ptr_v6;

  DLINK_FOREACH(ptr, ip_hash_table[hash_index].head)
  {
    iptr = ptr->data;

    if (iptr->ip.ss.ss_family != addr->ss.ss_family)
      continue;

    if (addr->ss.ss_family == AF_INET6)
    {
      ptr_v6 = (struct sockaddr_in6 *)&iptr->ip;
      if (!memcmp(&v6->sin6_addr, &ptr_v6->sin6_addr, sizeof(struct in6_addr)))
        return iptr;  /* Found entry already in hash, return it. */
    }
    else
    {
      ptr_v4 = (struct sockaddr_in *)&iptr->ip;
      if (!memcmp(&v4->sin_addr, &ptr_v4->sin_addr, sizeof(struct in_addr)))
        return iptr;  /* Found entry already in hash, return it. */
    }
  }

  iptr = mp_pool_get(ip_entry_pool);
  memcpy(&iptr->ip, addr, sizeof(struct irc_ssaddr));

  dlinkAdd(iptr, &iptr->node, &ip_hash_table[hash_index]);

  return iptr;
}
