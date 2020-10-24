prelink_find_copy_rel (DSO *dso, int n, struct copy_relocs *cr)
{
  Elf_Data *data = NULL;
  Elf_Scn *scn = dso->scn[n];
  GElf_Rel rel;
  int sec, ndx, maxndx;

  while ((data = elf_getdata (scn, data)) != NULL)
    {
      maxndx = data->d_size / dso->shdr[n].sh_entsize;
      for (ndx = 0; ndx < maxndx; ++ndx)
	{
	  gelfx_getrel (dso->elf, data, ndx, &rel);
	  sec = addr_to_sec (dso, rel.r_offset);
	  if (sec == -1)
	    continue;

	  if (GELF_R_TYPE (rel.r_info) == dso->arch->R_COPY
	      && prelink_add_copy_rel (dso, n, &rel, cr))
	    return 1;
	}
    }
  return 0;
}
