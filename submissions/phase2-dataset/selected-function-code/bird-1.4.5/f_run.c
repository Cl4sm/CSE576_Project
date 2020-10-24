int
f_run(struct filter *filter, struct rte **rte, struct ea_list **tmp_attrs, struct linpool *tmp_pool, int flags)
{
  if (filter == FILTER_ACCEPT)
    return F_ACCEPT;

  if (filter == FILTER_REJECT)
    return F_REJECT;

  int rte_cow = ((*rte)->flags & REF_COW);
  DBG( "Running filter `%s'...", filter->name );

  f_rte = rte;
  f_old_rta = NULL;
  f_tmp_attrs = tmp_attrs;
  f_pool = tmp_pool;
  f_flags = flags;

  LOG_BUFFER_INIT(f_buf);

  struct f_val res = interpret(filter->root);

  if (f_old_rta) {
    /*
     * Cached rta was modified and f_rte contains now an uncached one,
     * sharing some part with the cached one. The cached rta should
     * be freed (if rte was originally COW, f_old_rta is a clone
     * obtained during rte_cow()).
     *
     * This also implements the exception mentioned in f_run()
     * description. The reason for this is that rta reuses parts of
     * f_old_rta, and these may be freed during rta_free(f_old_rta).
     * This is not the problem if rte was COW, because original rte
     * also holds the same rta.
     */
    if (!rte_cow)
      (*f_rte)->attrs = rta_lookup((*f_rte)->attrs);

    rta_free(f_old_rta);
  }


  if (res.type != T_RETURN) {
    log_rl(&rl_runtime_err, L_ERR "Filter %s did not return accept nor reject. Make up your mind", filter->name);
    return F_ERROR;
  }
  DBG( "done (%u)\n", res.val.i );
  return res.val.i;
}
