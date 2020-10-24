void
bgp_parse_capabilities(struct bgp_conn *conn, byte *opt, int len)
{
  // struct bgp_proto *p = conn->bgp;
  int i, cl;

  while (len > 0)
    {
      if (len < 2 || len < 2 + opt[1])
	goto err;
      
      cl = opt[1];

      switch (opt[0])
	{
	case 2:	/* Route refresh capability, RFC 2918 */
	  if (cl != 0)
	    goto err;
	  conn->peer_refresh_support = 1;
	  break;

	case 64: /* Graceful restart capability, RFC 4724 */
	  if (cl % 4 != 2)
	    goto err;
	  conn->peer_gr_aware = 1;
	  conn->peer_gr_able = 0;
	  conn->peer_gr_time = get_u16(opt + 2) & 0x0fff;
	  conn->peer_gr_flags = opt[2] & 0xf0;
	  conn->peer_gr_aflags = 0;
	  for (i = 2; i < cl; i += 4)
	    if (opt[2+i+0] == 0 && opt[2+i+1] == BGP_AF && opt[2+i+2] == 1) /* Match AFI/SAFI */
	      {
		conn->peer_gr_able = 1;
		conn->peer_gr_aflags = opt[2+i+3];
	      }
	  break;

	case 65: /* AS4 capability, RFC 4893 */
	  if (cl != 4)
	    goto err;
	  conn->peer_as4_support = 1;
	  if (conn->bgp->cf->enable_as4)
	    conn->advertised_as = get_u32(opt + 2);
	  break;

	case 69: /* ADD-PATH capability, draft */
	  if (cl % 4)
	    goto err;
	  for (i = 0; i < cl; i += 4)
	    if (opt[2+i+0] == 0 && opt[2+i+1] == BGP_AF && opt[2+i+2] == 1) /* Match AFI/SAFI */
	      conn->peer_add_path = opt[2+i+3];
	  if (conn->peer_add_path > ADD_PATH_FULL)
	    goto err;

	  break;

	  /* We can safely ignore all other capabilities */
	}
      len -= 2 + cl;
      opt += 2 + cl;
    }
  return;

 err:
  bgp_error(conn, 2, 0, NULL, 0);
  return;
}
