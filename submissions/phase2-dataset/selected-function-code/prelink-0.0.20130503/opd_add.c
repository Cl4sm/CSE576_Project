int
opd_add (struct prelink_info *info, GElf_Word r_sym, int reloc_type)
{
  struct opd_fptr *f, fp;
  void **tabslot;
  struct opd_lib *l = info->ent->opd;

  if (l->u.refp[r_sym] != NULL)
    {
      ++l->u.refp[r_sym]->refcnt;
      return 0;
    }

  if (ELF64_ST_BIND (info->symtab [r_sym].st_info)
      == STB_LOCAL)
    {
      fp.val = info->symtab [r_sym].st_value;
      fp.gp = info->ent->pltgot;
    }
  else
    {
      fp.val = info->resolve (info, r_sym, reloc_type);
      if (info->resolveent == NULL)
	return 0;
      fp.gp = info->resolveent->pltgot;
    }

  l->u.refp[r_sym] = malloc (sizeof (struct opd_ref));
  if (l->u.refp[r_sym] == NULL)
    goto error_mem;
  l->u.refp[r_sym]->symoff = r_sym;
  l->u.refp[r_sym]->refcnt = 1;
  l->u.refp[r_sym]->ent = NULL;

  tabslot = htab_find_slot (l->htab, &fp, INSERT);
  if (tabslot == NULL)
    goto error_mem;

  if (*tabslot == NULL)
    {
      f = calloc (sizeof (struct opd_fptr), 1);
      if (f == NULL)
	goto error_mem;
      f->val = fp.val;
      f->gp = fp.gp;
      *tabslot = f;
    }

  l->u.refp[r_sym]->ent = *tabslot;
  return 0;

error_mem:
  error (0, ENOMEM, "%s: Could not create OPD table",
	 info->ent->filename);
  return 1;
}
