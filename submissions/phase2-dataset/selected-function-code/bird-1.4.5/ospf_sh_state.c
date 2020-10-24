void
ospf_sh_state(struct proto *p, int verbose, int reachable)
{
  struct proto_ospf *po = (struct proto_ospf *) p;
  struct ospf_lsa_header *cnode = NULL;
  unsigned int i, ix, j1, j2, jx;
  u32 last_area = 0xFFFFFFFF;

  if (p->proto_state != PS_UP)
  {
    cli_msg(-1016, "%s: is not up", p->name);
    cli_msg(0, "");
    return;
  }

  /* We store interesting area-scoped LSAs in array hea and 
     global-scoped (LSA_T_EXT) LSAs in array hex */

  int num = po->gr->hash_entries;
  struct top_hash_entry *hea[num];
  struct top_hash_entry *hex[verbose ? num : 0];
  struct top_hash_entry *he;

  j1 = j2 = jx = 0;
  WALK_SLIST(he, po->lsal)
  {
    int accept;

    switch (he->lsa.type)
      {
      case LSA_T_RT:
      case LSA_T_NET:
	accept = 1;
	break;

      case LSA_T_SUM_NET:
      case LSA_T_SUM_RT:
      case LSA_T_NSSA:
#ifdef OSPFv3
      case LSA_T_PREFIX:
#endif
	accept = verbose;
	break;

      case LSA_T_EXT:
	if (verbose)
	{
	  he->domain = 1; /* Abuse domain field to mark the LSA */
	  hex[jx++] = he;
	}
      default:
	accept = 0;
      }

    if (accept)
      hea[j1++] = he;
    else
      j2++;
  }

  if ((j1 + j2) != num)
    die("Fatal mismatch");

  qsort(hea, j1, sizeof(struct top_hash_entry *), lsa_compare_for_state);
  qsort(hex, jx, sizeof(struct top_hash_entry *), ext_compare_for_state);

  /*
   * This code is a bit tricky, we have a primary LSAs (router and
   * network) that are presented as a node, and secondary LSAs that
   * are presented as a part of a primary node. cnode represents an
   * currently opened node (whose header was presented). The LSAs are
   * sorted to get secondary LSAs just after related primary LSA (if
   * available). We present secondary LSAs only when related primary
   * LSA is opened.
   *
   * AS-external LSAs are stored separately as they might be presented
   * several times (for each area when related ASBR is opened). When
   * the node is closed, related external routes are presented. We
   * also have to take into account that in OSPFv3, there might be
   * more router-LSAs and only the first should be considered as a
   * primary. This is handled by not closing old router-LSA when next
   * one is processed (which is not opened because there is already
   * one opened).
   */

  ix = 0;
  for (i = 0; i < j1; i++)
  {
    he = hea[i];

    /* If there is no opened node, we open the LSA (if appropriate) or skip to the next one */
    if (!cnode)
    {
      if (((he->lsa.type == LSA_T_RT) || (he->lsa.type == LSA_T_NET))
	  && ((he->color == INSPF) || !reachable))
      {
	cnode = &(he->lsa);

	if (he->domain != last_area)
	{
	  cli_msg(-1016, "");
	  cli_msg(-1016, "area %R", he->domain);
	  last_area = he->domain;
	  ix = 0;
	}
      }
      else
	continue;
    }

    ASSERT(cnode && (he->domain == last_area) && (he->lsa.rt == cnode->rt));

    switch (he->lsa.type)
    {
      case LSA_T_RT:
	show_lsa_router(po, he, he->lsa.id == cnode->id, verbose);
	break;

      case LSA_T_NET:
	show_lsa_network(he);
	break;

      case LSA_T_SUM_NET:
	if (cnode->type == LSA_T_RT)
	  show_lsa_sum_net(he);
	break;

      case LSA_T_SUM_RT:
	if (cnode->type == LSA_T_RT)
	  show_lsa_sum_rt(he);
	break;

#ifdef OSPFv3
      case LSA_T_PREFIX:
	show_lsa_prefix(he, cnode);
	break;
#endif

      case LSA_T_EXT:
      case LSA_T_NSSA:
	show_lsa_external(he);
	break;
    }

    /* In these cases, we close the current node */
    if ((i+1 == j1)
	|| (hea[i+1]->domain != last_area)
	|| (hea[i+1]->lsa.rt != cnode->rt)
	|| (hea[i+1]->lsa.type == LSA_T_NET))
    {
      while ((ix < jx) && (hex[ix]->lsa.rt < cnode->rt))
	ix++;

      while ((ix < jx) && (hex[ix]->lsa.rt == cnode->rt))
	show_lsa_external(hex[ix++]);

      cnode = NULL;
    }
  }

  int hdr = 0;
  u32 last_rt = 0xFFFFFFFF;
  for (ix = 0; ix < jx; ix++)
  {
    he = hex[ix];

    /* If it is still marked, we show it now. */
    if (he->domain)
    {
      he->domain = 0;

      if ((he->color != INSPF) && reachable)
	continue;

      if (!hdr)
      {
	cli_msg(-1016, "");
	cli_msg(-1016, "other ASBRs");
	hdr = 1;
      }

      if (he->lsa.rt != last_rt)
      {
	cli_msg(-1016, "");
	cli_msg(-1016, "\trouter %R", he->lsa.rt);
	last_rt = he->lsa.rt;
      }

      show_lsa_external(he);
    }
  }

  cli_msg(0, "");
}
