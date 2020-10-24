bgp_do_rx_update(struct bgp_conn *conn,
		 byte *withdrawn, int withdrawn_len,
		 byte *nlri, int nlri_len,
		 byte *attrs, int attr_len)
{
  struct bgp_proto *p = conn->bgp;
  struct rte_src *src = p->p.main_source;
  rta *a0, *a = NULL;
  ip_addr prefix;
  int pxlen, err = 0;
  u32 path_id = 0;
  u32 last_id = 0;

  /* Check for End-of-RIB marker */
  if (!withdrawn_len && !attr_len && !nlri_len)
    {
      bgp_rx_end_mark(p);
      return;
    }

  /* Withdraw routes */
  while (withdrawn_len)
    {
      DECODE_PREFIX(withdrawn, withdrawn_len);
      DBG("Withdraw %I/%d\n", prefix, pxlen);

      bgp_rte_withdraw(p, prefix, pxlen, path_id, &last_id, &src);
    }

  if (!attr_len && !nlri_len)		/* shortcut */
    return;

  a0 = bgp_decode_attrs(conn, attrs, attr_len, bgp_linpool, nlri_len);

  if (conn->state != BS_ESTABLISHED)	/* fatal error during decoding */
    return;

  if (a0 && nlri_len && !bgp_set_next_hop(p, a0))
    a0 = NULL;

  last_id = 0;
  src = p->p.main_source;

  while (nlri_len)
    {
      DECODE_PREFIX(nlri, nlri_len);
      DBG("Add %I/%d\n", prefix, pxlen);

      if (a0)
	bgp_rte_update(p, prefix, pxlen, path_id, &last_id, &src, a0, &a);
      else /* Forced withdraw as a result of soft error */
	bgp_rte_withdraw(p, prefix, pxlen, path_id, &last_id, &src);
    }

 done:
  if (a)
    rta_free(a);

  if (err)
    bgp_error(conn, 3, err, NULL, 0);

  return;
}
