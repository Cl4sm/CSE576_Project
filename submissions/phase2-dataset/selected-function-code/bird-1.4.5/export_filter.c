static rte *
export_filter(struct announce_hook *ah, rte *rt0, rte **rt_free, ea_list **tmpa, int silent)
{
  struct proto *p = ah->proto;
  struct filter *filter = ah->out_filter;
  struct proto_stats *stats = ah->stats;
  ea_list *tmpb = NULL;
  rte *rt;
  int v;

  rt = rt0;
  *rt_free = NULL;

  /* If called does not care for eattrs, we prepare one internally */
  if (!tmpa)
    {
      tmpb = make_tmp_attrs(rt, rte_update_pool);
      tmpa = &tmpb;
    }

  v = p->import_control ? p->import_control(p, &rt, tmpa, rte_update_pool) : 0;
  if (v < 0)
    {
      if (silent)
	goto reject;

      stats->exp_updates_rejected++;
      if (v == RIC_REJECT)
	rte_trace_out(D_FILTERS, p, rt, "rejected by protocol");
      goto reject;
    }
  if (v > 0)
    {
      if (!silent)
	rte_trace_out(D_FILTERS, p, rt, "forced accept by protocol");
      goto accept;
    }

  v = filter && ((filter == FILTER_REJECT) ||
		 (f_run(filter, &rt, tmpa, rte_update_pool, FF_FORCE_TMPATTR) > F_ACCEPT));
  if (v)
    {
      if (silent)
	goto reject;

      stats->exp_updates_filtered++;
      rte_trace_out(D_FILTERS, p, rt, "filtered out");
      goto reject;
    }

 accept:
  if (rt != rt0)
    *rt_free = rt;
  return rt;

 reject:
  /* Discard temporary rte */
  if (rt != rt0)
    rte_free(rt);
  return NULL;
}
