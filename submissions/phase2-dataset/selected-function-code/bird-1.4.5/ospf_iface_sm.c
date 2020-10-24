void
ospf_iface_sm(struct ospf_iface *ifa, int event)
{
  DBG("SM on iface %s. Event is '%s'\n", ifa->ifname, ospf_ism[event]);

  switch (event)
  {
  case ISM_UP:
    if (ifa->state <= OSPF_IS_LOOP)
    {
      /* Now, nothing should be adjacent */
      if ((ifa->type == OSPF_IT_PTP) || (ifa->type == OSPF_IT_PTMP) || (ifa->type == OSPF_IT_VLINK))
      {
	ospf_iface_chstate(ifa, OSPF_IS_PTP);
      }
      else
      {
	if (ifa->priority == 0)
	  ospf_iface_chstate(ifa, OSPF_IS_DROTHER);
	else
	{
	  ospf_iface_chstate(ifa, OSPF_IS_WAITING);
	  if (ifa->wait_timer)
	    tm_start(ifa->wait_timer, ifa->waitint);
	}
      }

      if (ifa->hello_timer)
	tm_start(ifa->hello_timer, ifa->helloint);

      if (ifa->poll_timer)
	tm_start(ifa->poll_timer, ifa->pollint);

      ospf_hello_send(ifa, OHS_HELLO, NULL);
      schedule_link_lsa(ifa);
    }
    break;

  case ISM_BACKS:
  case ISM_WAITF:
    if (ifa->state == OSPF_IS_WAITING)
    {
      bdr_election(ifa);
    }
    break;

  case ISM_NEICH:
    if ((ifa->state == OSPF_IS_DROTHER) || (ifa->state == OSPF_IS_DR) ||
	(ifa->state == OSPF_IS_BACKUP))
    {
      bdr_election(ifa);
      schedule_rt_lsa(ifa->oa);
    }
    break;

  case ISM_LOOP:
    if ((ifa->state > OSPF_IS_LOOP) && ifa->check_link)
      ospf_iface_chstate(ifa, OSPF_IS_LOOP);
    break;

  case ISM_UNLOOP:
    /* Immediate go UP */
    if (ifa->state == OSPF_IS_LOOP)
      ospf_iface_sm(ifa, ISM_UP);
    break;

  case ISM_DOWN:
    ospf_iface_chstate(ifa, OSPF_IS_DOWN);
    break;

  default:
    bug("OSPF_I_SM - Unknown event?");
    break;
  }

}
