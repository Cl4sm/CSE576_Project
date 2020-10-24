ospf_sh(struct proto *p)
{
  struct ospf_area *oa;
  struct proto_ospf *po = (struct proto_ospf *) p;
  struct ospf_iface *ifa;
  struct ospf_neighbor *n;
  int ifano, nno, adjno, firstfib;
  struct area_net *anet;

  if (p->proto_state != PS_UP)
  {
    cli_msg(-1014, "%s: is not up", p->name);
    cli_msg(0, "");
    return;
  }

  cli_msg(-1014, "%s:", p->name);
  cli_msg(-1014, "RFC1583 compatibility: %s", (po->rfc1583 ? "enable" : "disabled"));
  cli_msg(-1014, "Stub router: %s", (po->stub_router ? "Yes" : "No"));
  cli_msg(-1014, "RT scheduler tick: %d", po->tick);
  cli_msg(-1014, "Number of areas: %u", po->areano);
  cli_msg(-1014, "Number of LSAs in DB:\t%u", po->gr->hash_entries);

  WALK_LIST(oa, po->area_list)
  {
    cli_msg(-1014, "\tArea: %R (%u) %s", oa->areaid, oa->areaid,
	    oa->areaid == 0 ? "[BACKBONE]" : "");
    ifano = 0;
    nno = 0;
    adjno = 0;
    WALK_LIST(ifa, po->iface_list)
    {
      if (oa == ifa->oa)
      {
	ifano++;
        WALK_LIST(n, ifa->neigh_list)
        {
	  nno++;
	  if (n->state == NEIGHBOR_FULL)
	    adjno++;
        }
      }
    }

    cli_msg(-1014, "\t\tStub:\t%s", oa_is_stub(oa) ? "Yes" : "No");
    cli_msg(-1014, "\t\tNSSA:\t%s", oa_is_nssa(oa) ? "Yes" : "No");
    cli_msg(-1014, "\t\tTransit:\t%s", oa->trcap ? "Yes" : "No");

    if (oa_is_nssa(oa))
      cli_msg(-1014, "\t\tNSSA translation:\t%s%s", oa->translate ? "Yes" : "No",
	      oa->translate == TRANS_WAIT ? " (run down)" : "");
    cli_msg(-1014, "\t\tNumber of interfaces:\t%u", ifano);
    cli_msg(-1014, "\t\tNumber of neighbors:\t%u", nno);
    cli_msg(-1014, "\t\tNumber of adjacent neighbors:\t%u", adjno);

    firstfib = 1;
    FIB_WALK(&oa->net_fib, nftmp)
    {
      anet = (struct area_net *) nftmp;
      if(firstfib)
      {
        cli_msg(-1014, "\t\tArea networks:");
        firstfib = 0;
      }
      cli_msg(-1014, "\t\t\t%1I/%u\t%s\t%s", anet->fn.prefix, anet->fn.pxlen,
		anet->hidden ? "Hidden" : "Advertise", anet->active ? "Active" : "");
    }
    FIB_WALK_END;

    firstfib = 1;
    FIB_WALK(&oa->enet_fib, nftmp)
    {
      anet = (struct area_net *) nftmp;
      if(firstfib)
      {
        cli_msg(-1014, "\t\tArea external networks:");
        firstfib = 0;
      }
      cli_msg(-1014, "\t\t\t%1I/%u\t%s\t%s", anet->fn.prefix, anet->fn.pxlen,
		anet->hidden ? "Hidden" : "Advertise", anet->active ? "Active" : "");
    }
    FIB_WALK_END;

  }
  cli_msg(0, "");
}
