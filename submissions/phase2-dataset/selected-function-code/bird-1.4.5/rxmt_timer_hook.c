static void
rxmt_timer_hook(timer * timer)
{
  struct ospf_neighbor *n = (struct ospf_neighbor *) timer->data;
  // struct proto *p = &n->ifa->oa->po->proto;
  struct top_hash_entry *en;

  DBG("%s: RXMT timer fired on interface %s for neigh: %I.\n",
      p->name, n->ifa->ifname, n->ip);

  if(n->state < NEIGHBOR_EXSTART) return;

  if (n->state == NEIGHBOR_EXSTART)
  {
    ospf_dbdes_send(n, 1);
    return;
  }

  if ((n->state == NEIGHBOR_EXCHANGE) && n->myimms.bit.ms)	/* I'm master */
    ospf_dbdes_send(n, 0);


  if (n->state < NEIGHBOR_FULL)	
    ospf_lsreq_send(n);	/* EXCHANGE or LOADING */
  else
  {
    if (!EMPTY_SLIST(n->lsrtl))	/* FULL */
    {
      list uplist;
      slab *upslab;
      struct l_lsr_head *llsh;

      init_list(&uplist);
      upslab = sl_new(n->pool, sizeof(struct l_lsr_head));

      WALK_SLIST(en, n->lsrtl)
      {
	if ((SNODE en)->next == (SNODE en))
	  bug("RTList is cycled");
	llsh = sl_alloc(upslab);
	llsh->lsh.id = en->lsa.id;
	llsh->lsh.rt = en->lsa.rt;
	llsh->lsh.type = en->lsa.type;
	DBG("Working on ID: %R, RT: %R, Type: %u\n",
	    en->lsa.id, en->lsa.rt, en->lsa.type);
	add_tail(&uplist, NODE llsh);
      }
      ospf_lsupd_send_list(n, &uplist);
      rfree(upslab);
    }
  }
}
