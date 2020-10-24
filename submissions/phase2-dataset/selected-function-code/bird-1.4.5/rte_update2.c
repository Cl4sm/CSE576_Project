rte_update2(struct announce_hook *ah, net *net, rte *new, struct rte_src *src)
{
  struct proto *p = ah->proto;
  struct proto_stats *stats = ah->stats;
  struct filter *filter = ah->in_filter;
  ea_list *tmpa = NULL;
  rte *dummy = NULL;

  rte_update_lock();
  if (new)
    {
      new->sender = ah;

      stats->imp_updates_received++;
      if (!rte_validate(new))
	{
	  rte_trace_in(D_FILTERS, p, new, "invalid");
	  stats->imp_updates_invalid++;
	  goto drop;
	}

      if (filter == FILTER_REJECT)
	{
	  stats->imp_updates_filtered++;
	  rte_trace_in(D_FILTERS, p, new, "filtered out");

	  if (! ah->in_keep_filtered)
	    goto drop;

	  /* new is a private copy, i could modify it */
	  new->flags |= REF_FILTERED;
	}
      else
	{
	  tmpa = make_tmp_attrs(new, rte_update_pool);
	  if (filter && (filter != FILTER_REJECT))
	    {
	      ea_list *old_tmpa = tmpa;
	      int fr = f_run(filter, &new, &tmpa, rte_update_pool, 0);
	      if (fr > F_ACCEPT)
		{
		  stats->imp_updates_filtered++;
		  rte_trace_in(D_FILTERS, p, new, "filtered out");

		  if (! ah->in_keep_filtered)
		    goto drop;

		  new->flags |= REF_FILTERED;
		}
	      if (tmpa != old_tmpa && src->proto->store_tmp_attrs)
		src->proto->store_tmp_attrs(new, tmpa);
	    }
	}
      if (!rta_is_cached(new->attrs)) /* Need to copy attributes */
	new->attrs = rta_lookup(new->attrs);
      new->flags |= REF_COW;
    }
  else
    {
      stats->imp_withdraws_received++;

      if (!net || !src)
	{
	  stats->imp_withdraws_ignored++;
	  rte_update_unlock();
	  return;
	}
    }

 recalc:
  rte_hide_dummy_routes(net, &dummy);
  rte_recalculate(ah, net, new, tmpa, src);
  rte_unhide_dummy_routes(net, &dummy);
  rte_update_unlock();
  return;

 drop:
  rte_free(new);
  new = NULL;
  tmpa = NULL;
  goto recalc;
}
