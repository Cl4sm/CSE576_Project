ospf_sh_lsadb(struct lsadb_show_data *ld)
{
  struct proto *p = proto_get_named(ld->name, &proto_ospf);
  struct proto_ospf *po = (struct proto_ospf *) p;
  int num = po->gr->hash_entries;
  unsigned int i, j;
  int last_dscope = -1;
  u32 last_domain = 0;

  if (p->proto_state != PS_UP)
  {
    cli_msg(-1017, "%s: is not up", p->name);
    cli_msg(0, "");
    return;
  }

  if (ld->router == SH_ROUTER_SELF)
    ld->router = po->router_id;

  struct top_hash_entry *hea[num];
  struct top_hash_entry *he;

  j = 0;
  WALK_SLIST(he, po->lsal)
    hea[j++] = he;

  if (j != num)
    die("Fatal mismatch");

  qsort(hea, j, sizeof(struct top_hash_entry *), lsa_compare_for_lsadb);

  for (i = 0; i < j; i++)
  {
    struct ospf_lsa_header *lsa = &(hea[i]->lsa);
    int dscope = LSA_SCOPE(lsa);

    if (ld->scope && (dscope != (ld->scope & 0xf000)))
      continue;

    if ((ld->scope == LSA_SCOPE_AREA) && (hea[i]->domain != ld->area))
      continue;

    /* Ignore high nibble */
    if (ld->type && ((lsa->type & 0x0fff) != (ld->type & 0x0fff)))
      continue;

    if (ld->lsid && (lsa->id != ld->lsid))
      continue;

    if (ld->router && (lsa->rt != ld->router))
      continue;
    
    if ((dscope != last_dscope) || (hea[i]->domain != last_domain))
    {
      cli_msg(-1017, "");
      switch (dscope)
      {
	case LSA_SCOPE_AS:
	  cli_msg(-1017, "Global");
	  break;
	case LSA_SCOPE_AREA:
	  cli_msg(-1017, "Area %R", hea[i]->domain);
	  break;
#ifdef OSPFv3
	case LSA_SCOPE_LINK:
	  {
	    struct iface *ifa = if_find_by_index(hea[i]->domain);
	    cli_msg(-1017, "Link %s", (ifa != NULL) ? ifa->name : "?");
	  }
	  break;
#endif
      }
      cli_msg(-1017, "");
      cli_msg(-1017," Type   LS ID           Router           Age  Sequence  Checksum");

      last_dscope = dscope;
      last_domain = hea[i]->domain;
    }


    cli_msg(-1017," %04x  %-15R %-15R %5u  %08x    %04x",
	    lsa->type, lsa->id, lsa->rt, lsa->age, lsa->sn, lsa->checksum);
  }
  cli_msg(0, "");
}
