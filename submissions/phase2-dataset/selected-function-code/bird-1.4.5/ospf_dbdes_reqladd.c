static void
ospf_dbdes_reqladd(struct ospf_dbdes_packet *ps, struct ospf_neighbor *n)
{
  struct ospf_lsa_header *plsa, lsa;
  struct top_hash_entry *he, *sn;
  struct ospf_area *oa = n->ifa->oa;
  struct top_graph *gr = oa->po->gr;
  struct ospf_packet *op;
  int i, j;

  op = (struct ospf_packet *) ps;

  plsa = (void *) (ps + 1);

  j = (ntohs(op->length) - sizeof(struct ospf_dbdes_packet)) /
    sizeof(struct ospf_lsa_header);

  for (i = 0; i < j; i++)
  {
    ntohlsah(plsa + i, &lsa);
    u32 dom = ospf_lsa_domain(lsa.type, n->ifa);
    if (((he = ospf_hash_find_header(gr, dom, &lsa)) == NULL) ||
	(lsa_comp(&lsa, &(he->lsa)) == 1))
    {
      /* Is this condition necessary? */
      if (ospf_hash_find_header(n->lsrqh, dom, &lsa) == NULL)
      {
	sn = ospf_hash_get_header(n->lsrqh, dom, &lsa);
	ntohlsah(plsa + i, &(sn->lsa));
	s_add_tail(&(n->lsrql), SNODE sn);
      }
    }
  }
}
