static int
prelink_find_copy_rela (DSO *dso, int n, struct copy_relocs *cr)
{
  Elf_Data *data = NULL;
  Elf_Scn *scn = dso->scn[n];
  union {
    GElf_Rel rel;
    GElf_Rela rela;
  } u;
  int sec, ndx, maxndx;

  while ((data = elf_getdata (scn, data)) != NULL)
    {
      maxndx = data->d_size / dso->shdr[n].sh_entsize;
      for (ndx = 0; ndx < maxndx; ++ndx)
	{
	  gelfx_getrela (dso->elf, data, ndx, &u.rela);
	  sec = addr_to_sec (dso, u.rela.r_offset);
	  if (sec == -1)
	    continue;

	  if (GELF_R_TYPE (u.rela.r_info) == dso->arch->R_COPY)
	    {
	      if (u.rela.r_addend != 0)
		{
		  error (0, 0, "%s: COPY reloc with non-zero addend?",
			 dso->filename);
		  return 1;
		}
	      if (prelink_add_copy_rel (dso, n, &u.rel, cr))
		return 1;
	    }
	}
    }
  return 0;
}
