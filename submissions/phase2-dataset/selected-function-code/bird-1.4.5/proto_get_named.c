proto_get_named(struct symbol *sym, struct protocol *pr)
{
  struct proto *p, *q;

  if (sym)
    {
      if (sym->class != SYM_PROTO)
	cf_error("%s: Not a protocol", sym->name);
      p = ((struct proto_config *)sym->def)->proto;
      if (!p || p->proto != pr)
	cf_error("%s: Not a %s protocol", sym->name, pr->name);
    }
  else
    {
      p = NULL;
      WALK_LIST(q, active_proto_list)
	if (q->proto == pr)
	  {
	    if (p)
	      cf_error("There are multiple %s protocols running", pr->name);
	    p = q;
	  }
      if (!p)
	cf_error("There is no %s protocol running", pr->name);
    }
  return p;
}
