static int
ppc64_fixup_plt (struct prelink_info *info, GElf_Rela *rela, GElf_Addr value)
{
  DSO *dso = info->dso;
  int sec, i;
  size_t n;
  struct opd_rec rec;

  if (value == 0)
    {
      rec.fn = 0;
      rec.toc = 0;
      rec.chain = 0;
    }
  else if ((sec = addr_to_sec (dso, value)) != -1)
    {
      rec.fn = read_ube64 (dso, value);
      rec.toc = read_ube64 (dso, value + 8);
      rec.chain = read_ube64 (dso, value + 16);
    }
  else
    {
      for (i = 0; i < info->ent->ndepends; ++i)
	if (info->ent->depends[i]->opd
	    && info->ent->depends[i]->opd->start <= value
	    && (info->ent->depends[i]->opd->start
		+ info->ent->depends[i]->opd->size) > value)
	break;

      if (i == info->ent->ndepends)
	{
	  error (0, 0, "%s: R_PPC64_JMP_SLOT doesn't resolve to an .opd address",
		 dso->filename);
	  return 1;
	}
      if ((value - info->ent->depends[i]->opd->start) % 8)
	{
	  error (0, 0, "%s: R_PPC64_JMP_SLOT doesn't resolve to valid .opd section location",
		 dso->filename);
	  return 1;
	}
      n = (value - info->ent->depends[i]->opd->start) / 8;
      rec.fn = info->ent->depends[i]->opd->table[n];
      rec.toc = info->ent->depends[i]->opd->table[n + 1];
      rec.chain = info->ent->depends[i]->opd->table[n + 2];
    }
  write_be64 (dso, rela->r_offset, rec.fn);
  write_be64 (dso, rela->r_offset + 8, rec.toc);
  write_be64 (dso, rela->r_offset + 16, rec.chain);
  return 0;
}
