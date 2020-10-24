mrt_put_bgp4_hdr(byte *buf, struct bgp_conn *conn, int as4)
{
  struct bgp_proto *p = conn->bgp;

  if (as4)
    {
      put_u32(buf+0, p->remote_as);
      put_u32(buf+4, p->local_as);
      buf+=8;
    }
  else
    {
      put_u16(buf+0, (p->remote_as <= 0xFFFF) ? p->remote_as : AS_TRANS);
      put_u16(buf+2, (p->local_as <= 0xFFFF)  ? p->local_as  : AS_TRANS);
      buf+=4;
    }

  put_u16(buf+0, (p->neigh && p->neigh->iface) ? p->neigh->iface->index : 0);
  put_u16(buf+2, BGP_AF);
  buf+=4;
  buf = ipa_put_addr(buf, conn->sk ? conn->sk->daddr : IPA_NONE);
  buf = ipa_put_addr(buf, conn->sk ? conn->sk->saddr : IPA_NONE);

  return buf;
}
