adjust_rel (DSO *dso, int n, GElf_Addr start, GElf_Addr adjust)
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

	  dso->arch->adjust_rel (dso, &rel, start, adjust);
	  addr_adjust (rel.r_offset, start, adjust);
	  gelfx_update_rel (dso->elf, data, ndx, &rel);
	}
    }

  elf_flagscn (scn, ELF_C_SET, ELF_F_DIRTY);
  return 0;
}
