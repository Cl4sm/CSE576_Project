void
roa_commit(struct config *new, struct config *old)
{
  struct roa_table_config *cf;
  struct roa_table *t;

  if (old)
    WALK_LIST(t, roa_table_list)
      {
	struct symbol *sym = cf_find_symbol(t->name);
	if (sym && sym->class == SYM_ROA)
	  {
	    /* Found old table in new config */
	    cf = sym->def;
	    cf->table = t;
	    t->name = cf->name;
	    t->cf = cf;

	    /* Reconfigure it */
	    roa_flush(t, ROA_SRC_CONFIG);
	    roa_populate(t);
	  }
	else
	  {
	    t->cf->table = NULL;

	    /* Free it now */
	    roa_flush(t, ROA_SRC_ANY);
	    rem_node(&t->n);
	    fib_free(&t->fib);
	    mb_free(t);
	  }
      }

  /* Add new tables */
  WALK_LIST(cf, new->roa_tables)
    if (! cf->table)
      roa_new_table(cf);

  roa_table_default = EMPTY_LIST(new->roa_tables) ? NULL :
    ((struct roa_table_config *) HEAD(new->roa_tables))->table;
}
