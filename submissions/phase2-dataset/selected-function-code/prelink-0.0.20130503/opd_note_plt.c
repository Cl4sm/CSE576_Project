void
opd_note_plt (struct prelink_info *info, GElf_Word r_sym, int reloc_type,
	      GElf_Addr r_offset)
{
  struct opd_fptr *f, fp;
  struct opd_lib *l = info->ent->opd;
  struct opd_ent_plt *entp;

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
	return;
      fp.gp = info->resolveent->pltgot;
    }

  f = (struct opd_fptr *) htab_find (l->htab, &fp);
  if (f == NULL || f->ent != NULL)
    return;

  entp = calloc (sizeof (struct opd_ent_plt), 1);
  if (entp == NULL)
    return;

  entp->v.val = fp.val;
  entp->v.gp = fp.gp;
  entp->v.opd = (r_offset - l->plt_start) | (OPD_ENT_PLT | OPD_ENT_NEW);
  entp->lib = info->ent;
  entp->symoff = r_sym;
  f->ent = &entp->v;
}
