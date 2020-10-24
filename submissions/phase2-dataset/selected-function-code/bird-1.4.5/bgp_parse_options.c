bgp_parse_options(struct bgp_conn *conn, byte *opt, int len)
{
  struct bgp_proto *p = conn->bgp;
  int ol;

  while (len > 0)
    {
      if (len < 2 || len < 2 + opt[1])
	{ bgp_error(conn, 2, 0, NULL, 0); return 0; }
#ifdef LOCAL_DEBUG
      {
	int i;
	DBG("\tOption %02x:", opt[0]);
	for(i=0; i<opt[1]; i++)
	  DBG(" %02x", opt[2+i]);
	DBG("\n");
      }
#endif

      ol = opt[1];
      switch (opt[0])
	{
	case 2:
	  if (conn->start_state == BSS_CONNECT_NOCAP)
	    BGP_TRACE(D_PACKETS, "Ignoring received capabilities");
	  else
	    bgp_parse_capabilities(conn, opt + 2, ol);
	  break;

	default:
	  /*
	   *  BGP specs don't tell us to send which option
	   *  we didn't recognize, but it's common practice
	   *  to do so. Also, capability negotiation with
	   *  Cisco routers doesn't work without that.
	   */
	  bgp_error(conn, 2, 4, opt, ol);
	  return 0;
	}
      len -= 2 + ol;
      opt += 2 + ol;
    }
  return 0;
}
