int
ospf_iface_assure_bufsize(struct ospf_iface *ifa, uint plen)
{
  plen += SIZE_OF_IP_HEADER;

#ifdef OSPFv2
  if (ifa->autype == OSPF_AUTH_CRYPT)
    plen += OSPF_AUTH_CRYPT_SIZE;
#endif

  if (plen <= ifa->sk->tbsize)
    return 0;

  if (ifa->cf->rx_buffer || (plen > 0xffff))
    return -1;

  plen = BIRD_ALIGN(plen, 1024);
  plen = MIN(plen, 0xffff);
  sk_set_tbsize(ifa->sk, plen);
  return 1;
}
