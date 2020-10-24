ospf_check_vlinks(struct proto_ospf *po)
{
  struct proto *p = &po->proto;

  struct ospf_iface *ifa;
  WALK_LIST(ifa, po->iface_list)
  {
    if (ifa->type == OSPF_IT_VLINK)
    {
      struct top_hash_entry *tmp;
      tmp = ospf_hash_find_rt(po->gr, ifa->voa->areaid, ifa->vid);

      if (tmp && (tmp->color == INSPF) && ipa_nonzero(tmp->lb) && tmp->nhs)
      {
	struct ospf_iface *nhi = ospf_iface_find(po, tmp->nhs->iface);

        if ((ifa->state != OSPF_IS_PTP)
	    || (ifa->vifa != nhi)
	    || !ipa_equal(ifa->vip, tmp->lb))
        {
          OSPF_TRACE(D_EVENTS, "Vlink peer %R found", tmp->lsa.id);
          ospf_iface_sm(ifa, ISM_DOWN);
	  ifa->vifa = nhi;
	  ifa->addr = nhi->addr;
	  ifa->cost = tmp->dist;
          ifa->vip = tmp->lb;
          ospf_iface_sm(ifa, ISM_UP);
        }
	else if ((ifa->state == OSPF_IS_PTP) && (ifa->cost != tmp->dist))
	{
	  ifa->cost = tmp->dist;
	  schedule_rt_lsa(po->backbone);
	}
      }
      else
      {
        if (ifa->state > OSPF_IS_DOWN)
        {
          OSPF_TRACE(D_EVENTS, "Vlink peer %R lost", ifa->vid);
	  ospf_iface_sm(ifa, ISM_DOWN);
        }
      }
    }
  }
}
