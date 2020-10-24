rt_examine(rtable *t, ip_addr prefix, int pxlen, struct proto *p, struct filter *filter)
{
  net *n = net_find(t, prefix, pxlen);
  rte *rt = n ? n->routes : NULL;

  if (!rte_is_valid(rt))
    return 0;

  rte_update_lock();

  /* Rest is stripped down export_filter() */
  ea_list *tmpa = make_tmp_attrs(rt, rte_update_pool);
  int v = p->import_control ? p->import_control(p, &rt, &tmpa, rte_update_pool) : 0;
  if (v == RIC_PROCESS)
    v = (f_run(filter, &rt, &tmpa, rte_update_pool, FF_FORCE_TMPATTR) <= F_ACCEPT);

   /* Discard temporary rte */
  if (rt != n->routes)
    rte_free(rt);

  rte_update_unlock();

  return v > 0;
}
