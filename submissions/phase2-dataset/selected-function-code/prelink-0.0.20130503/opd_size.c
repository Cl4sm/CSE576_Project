GElf_Addr
opd_size (struct prelink_info *info, GElf_Word entsize)
{
  struct opd_lib *l = info->ent->opd;
  int i;
  GElf_Addr ret = 0;
  struct opd_ent *e;
  struct opd_fptr *f;

  for (i = 0; i < l->nrefs; ++i)
    if ((f = (struct opd_fptr *) l->u.refp[i]->ent)->ent == NULL)
      {
	e = calloc (sizeof (struct opd_ent), 1);
	if (e == NULL)
	  {
	    error (0, ENOMEM, "%s: Could not create OPD table",
		   info->ent->filename);
	    return -1;
	  }

	e->val = f->val;
	e->gp = f->gp;
	e->opd = ret | OPD_ENT_NEW;
	ret += entsize;
      }

  return ret;
}
