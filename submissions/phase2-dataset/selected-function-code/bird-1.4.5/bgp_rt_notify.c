void
bgp_rt_notify(struct proto *P, rtable *tbl UNUSED, net *n, rte *new, rte *old UNUSED, ea_list *attrs)
{
  struct bgp_proto *p = (struct bgp_proto *) P;
  struct bgp_bucket *buck;
  struct bgp_prefix *px;
  rte *key;
  u32 path_id;

  DBG("BGP: Got route %I/%d %s\n", n->n.prefix, n->n.pxlen, new ? "up" : "down");

  if (new)
    {
      key = new;
      buck = bgp_get_bucket(p, n, attrs, new->attrs->source != RTS_BGP);
      if (!buck)			/* Inconsistent attribute list */
	return;
    }
  else
    {
      key = old;
      if (!(buck = p->withdraw_bucket))
	{
	  buck = p->withdraw_bucket = mb_alloc(P->pool, sizeof(struct bgp_bucket));
	  init_list(&buck->prefixes);
	}
    }
  path_id = p->add_path_tx ? key->attrs->src->global_id : 0;
  px = bgp_get_prefix(p, n->n.prefix, n->n.pxlen, path_id);
  if (px->bucket_node.next)
    {
      DBG("\tRemoving old entry.\n");
      rem_node(&px->bucket_node);
    }
  add_tail(&buck->prefixes, &px->bucket_node);
  bgp_schedule_packet(p->conn, PKT_UPDATE);
}
