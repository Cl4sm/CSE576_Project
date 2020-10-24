int
opd_init (struct prelink_info *info)
{
  int i, j, nrefent = 0;
  struct opd_lib *l;
  struct opd_refent refent, *r, **refarr, **a;
  struct opd_tabent tabent, *t;
  void **tabslot;
  htab_t tabent_htab = NULL, refent_htab = NULL;

  l = calloc (sizeof (struct opd_lib), 1);
  if (l == NULL)
    goto error_mem;
  l->nrefs = (info->symtab_end - info->symtab_start) / info->symtab_entsize;
  if (l->nrefs)
    {
      l->u.refp = calloc (l->nrefs, sizeof (struct opd_ref *));
      if (l->u.refp == NULL)
	goto error_mem;
    }
  else
    l->u.refp = NULL;
  tabent_htab = htab_try_create (100, opd_tabent_hash, opd_tabent_eq, opd_del);
  refent_htab = htab_try_create (100, opd_refent_hash, opd_refent_eq, opd_del);
  l->htab = htab_try_create (100, opd_refent_hash, opd_refent_eq, opd_del);
  if (tabent_htab == NULL || refent_htab == NULL || l->htab == NULL)
    goto error_mem;

  for (i = 0; i < info->ent->ndepends; ++i)
    {
      struct prelink_entry *ent;
      struct prelink_conflict *conflict;
      struct opd_lib *ol;
      size_t maxidx = 1;

      ent = info->ent->depends[i];
      ol = ent->opd;
      if (info->conflicts[i + 1].hash != &info->conflicts[i + 1].first)
	maxidx = 251;
      for (j = 0; j < ol->nrefs; ++j)
	{
	  GElf_Addr symoff = ol->u.refs[j].symoff;
	  refent.val = ol->u.refs[j].ent->val;
	  refent.gp = ol->u.refs[j].ent->gp;
	  for (conflict = info->conflicts[i + 1].hash[symoff % maxidx]; conflict;
	       conflict = conflict->next)
	    {
	      if (conflict->symoff == symoff
		  && conflict->reloc_class != RTYPE_CLASS_COPY
		  && conflict->reloc_class != RTYPE_CLASS_TLS)
		break;
	    }

	  if (conflict)
	    {
	      if (refent.val
		  != conflict->conflict.ent->base + conflict->conflictval
		  || refent.gp != conflict->conflict.ent->pltgot)
		{
		  error (0, 0, "%s: OPD value changed during prelinking",
			 info->ent->filename);
		  goto error_out;
		}

	      refent.val = conflict->lookup.ent->base + conflict->lookupval;
	      refent.gp = conflict->lookup.ent->pltgot;
	    }

	  if (ol->u.refs[j].ent->opd & OPD_ENT_PLT)
	    {
	      struct opd_ent_plt *entp
		= (struct opd_ent_plt *) ol->u.refs[j].ent;
	      int k;
	      size_t idx = 0;

	      for (k = 0; k < info->ent->ndepends; ++k)
		if (info->ent->depends[k] == entp->lib)
		  break;

	      assert (k < info->ent->ndepends);

	      if (info->conflicts[k + 1].hash != &info->conflicts[k + 1].first)
		idx = entp->symoff % 251;
	      for (conflict = info->conflicts[k + 1].hash[idx]; conflict;
		   conflict = conflict->next)
		{
		  if (conflict->symoff == entp->symoff
		      && conflict->reloc_class == RTYPE_CLASS_PLT)
		    break;
		}

	      if (conflict)
		{
		  if (ol->u.refs[j].ent->val
		      != conflict->conflict.ent->base + conflict->conflictval
		      || ol->u.refs[j].ent->gp
			 != conflict->conflict.ent->pltgot)
		    {
		      error (0, 0, "%s: OPD value changed during prelinking",
			     info->ent->filename);
		      goto error_out;
		    }

		  /* FPTR originally pointed into .plt, but since they
		     now resolve to different values, this cannot be used.  */
		  if (refent.val
		      != conflict->lookup.ent->base + conflict->lookupval
		      || refent.gp != conflict->lookup.ent->pltgot)
		    continue;
		}
	      else if (refent.val != ol->u.refs[j].ent->val
		       || refent.gp != ol->u.refs[j].ent->gp)
		continue;
	    }

	  tabslot = htab_find_slot (refent_htab, &refent, INSERT);
	  if (tabslot == NULL)
	    goto error_mem;

	  if (*tabslot != NULL)
	    {
	      for (r = (struct opd_refent *) *tabslot; r; r = r->next)
		if (r->tabent->ent == ol->u.refs[j].ent)
		  {
		    r->refcnt += ol->u.refs[j].refcnt;
		    break;
		  }

	      if (r)
		continue;
	    }

	  r = (struct opd_refent *) calloc (sizeof (struct opd_refent), 1);
	  if (r == NULL)
	    goto error_mem;

	  ++nrefent;
	  r->next = (struct opd_refent *) *tabslot;
	  *tabslot = r;
	  r->val = refent.val;
	  r->gp = refent.gp;
	  r->refcnt = ol->u.refs[j].refcnt;

	  tabent.ent = ol->u.refs[j].ent;

	  tabslot = htab_find_slot (tabent_htab, &tabent, INSERT);
	  if (tabslot == NULL)
	    goto error_mem;

	  if (*tabslot != NULL)
	    {
	      t = (struct opd_tabent *) *tabslot;
	      t->ref->nextref = r;
	      r->nextref = t->ref;
	    }
	  else
	    {
	      t = (struct opd_tabent *) calloc (sizeof (struct opd_tabent), 1);
	      if (t == NULL)
		goto error_mem;
	      t->ent = ol->u.refs[j].ent;
	      *tabslot = t;
	      r->nextref = r;
	      t->ref = r;
	    }

	  r->tabent = t;
	}
    }

  refarr = alloca (nrefent * sizeof (struct opd_refent *));
  a = refarr;
  htab_traverse (refent_htab, opd_gather_refent, &a);
  assert (a == refarr + nrefent);
  qsort (refarr, nrefent, sizeof (struct opd_refent *), opd_refent_cmp);
  for (i = 0; i < nrefent; ++i)
    {
      struct opd_fptr *f;

      if (refarr[i]->tabent == NULL)
	continue;

      f = (struct opd_fptr *) calloc (sizeof (struct opd_fptr), 1);
      if (f == NULL)
	goto error_mem;

      f->val = refarr[i]->val;
      f->gp = refarr[i]->gp;
      f->ent = refarr[i]->tabent->ent;
      tabslot = htab_find_slot (l->htab, f, INSERT);
      if (tabslot == NULL)
	goto error_mem;

      *tabslot = f;
      r = refarr[i]->tabent->ref;
      do
	{
	  if (r != refarr[i])
	    r->tabent = NULL;
	  r = r->nextref;
	}
      while (r != refarr[i]->tabent->ref);

      for (r = refarr[i]->first; r; r = r->next)
	r->tabent = NULL;
    }

  htab_delete (tabent_htab);
  htab_delete (refent_htab);
  info->ent->opd = l;
  return 0;

error_mem:
  error (0, ENOMEM, "%s: Could not create OPD table",
	 info->ent->filename);
error_out:
  if (tabent_htab)
    htab_delete (tabent_htab);
  if (refent_htab)
    htab_delete (refent_htab);
  if (l && l->htab)
    htab_delete (l->htab);
  free (l);
  return 1;
}
