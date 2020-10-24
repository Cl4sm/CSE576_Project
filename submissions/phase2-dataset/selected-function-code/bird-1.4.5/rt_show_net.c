static void
rt_show_net(struct cli *c, net *n, struct rt_show_data *d)
{
  rte *e, *ee;
  byte ia[STD_ADDRESS_P_LENGTH+8];
  struct ea_list *tmpa;
  struct announce_hook *a = NULL;
  int first = 1;
  int pass = 0;

  bsprintf(ia, "%I/%d", n->n.prefix, n->n.pxlen);

  if (d->export_mode)
    {
      if (! d->export_protocol->rt_notify)
	return;

      a = proto_find_announce_hook(d->export_protocol, d->table);
      if (!a)
	return;
    }

  for (e = n->routes; e; e = e->next)
    {
      if (rte_is_filtered(e) != d->filtered)
	continue;

      d->rt_counter++;
      d->net_counter += first;
      first = 0;

      if (pass)
	continue;

      ee = e;
      rte_update_lock();		/* We use the update buffer for filtering */
      tmpa = make_tmp_attrs(e, rte_update_pool);

      if (d->export_mode)
	{
	  struct proto *ep = d->export_protocol;
	  int ic = ep->import_control ? ep->import_control(ep, &e, &tmpa, rte_update_pool) : 0;

	  if (ep->accept_ra_types == RA_OPTIMAL)
	    pass = 1;

	  if (ic < 0)
	    goto skip;

	  if (d->export_mode > RSEM_PREEXPORT)
	    {
	      /*
	       * FIXME - This shows what should be exported according to current
	       * filters, but not what was really exported. 'configure soft'
	       * command may change the export filter and do not update routes.
	       */
	      int do_export = (ic > 0) ||
		(f_run(a->out_filter, &e, &tmpa, rte_update_pool, FF_FORCE_TMPATTR) <= F_ACCEPT);

	      if (do_export != (d->export_mode == RSEM_EXPORT))
		goto skip;

	      if ((d->export_mode == RSEM_EXPORT) && (ep->accept_ra_types == RA_ACCEPTED))
		pass = 1;
	    }
	}

      if (d->show_protocol && (d->show_protocol != e->attrs->src->proto))
	goto skip;

      if (f_run(d->filter, &e, &tmpa, rte_update_pool, FF_FORCE_TMPATTR) > F_ACCEPT)
	goto skip;

      d->show_counter++;
      if (d->stats < 2)
	rt_show_rte(c, ia, e, d, tmpa);
      ia[0] = 0;

    skip:
      if (e != ee)
      {
	rte_free(e);
	e = ee;
      }
      rte_update_unlock();

      if (d->primary_only)
	break;
    }
}
