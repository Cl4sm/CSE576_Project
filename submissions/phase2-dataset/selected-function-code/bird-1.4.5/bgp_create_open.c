static byte *
bgp_create_open(struct bgp_conn *conn, byte *buf)
{
  struct bgp_proto *p = conn->bgp;
  byte *cap;
  int cap_len;

  BGP_TRACE(D_PACKETS, "Sending OPEN(ver=%d,as=%d,hold=%d,id=%08x)",
	    BGP_VERSION, p->local_as, p->cf->hold_time, p->local_id);
  buf[0] = BGP_VERSION;
  put_u16(buf+1, (p->local_as < 0xFFFF) ? p->local_as : AS_TRANS);
  put_u16(buf+3, p->cf->hold_time);
  put_u32(buf+5, p->local_id);

  if (conn->start_state == BSS_CONNECT_NOCAP)
    {
      BGP_TRACE(D_PACKETS, "Skipping capabilities");
      buf[9] = 0;
      return buf + 10;
    }

  /* Skipped 3 B for length field and Capabilities parameter header */
  cap = buf + 12;

#ifndef IPV6
  if (p->cf->advertise_ipv4)
    cap = bgp_put_cap_ipv4(p, cap);
#endif

#ifdef IPV6
  cap = bgp_put_cap_ipv6(p, cap);
#endif

  if (p->cf->enable_refresh)
    cap = bgp_put_cap_rr(p, cap);

  if (p->cf->gr_mode == BGP_GR_ABLE)
    cap = bgp_put_cap_gr1(p, cap);
  else if (p->cf->gr_mode == BGP_GR_AWARE)
    cap = bgp_put_cap_gr2(p, cap);

  if (p->cf->enable_as4)
    cap = bgp_put_cap_as4(p, cap);

  if (p->cf->add_path)
    cap = bgp_put_cap_add_path(p, cap);

  cap_len = cap - buf - 12;
  if (cap_len > 0)
    {
      buf[9]  = cap_len + 2;	/* Optional params len */
      buf[10] = 2;		/* Option: Capability list */
      buf[11] = cap_len;	/* Option length */
      return cap;
    }
  else
    {
      buf[9] = 0;		/* No optional parameters */
      return buf + 10;
    }
}
