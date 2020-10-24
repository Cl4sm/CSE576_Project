adjust_rela (DSO *dso, int n, GElf_Addr start, GElf_Addr adjust)
{
  Elf_Data *data = NULL;
  Elf_Scn *scn = dso->scn[n];
  GElf_Rela rela;
  int sec, ndx, maxndx;

  while ((data = elf_getdata (scn, data)) != NULL)
    {
      maxndx = data->d_size / dso->shdr[n].sh_entsize;
      for (ndx = 0; ndx < maxndx; ++ndx)
	{
	  gelfx_getrela (dso->elf, data, ndx, &rela);
	  sec = addr_to_sec (dso, rela.r_offset);
	  if (sec == -1)
	    continue;

	  dso->arch->adjust_rela (dso, &rela, start, adjust);
	  addr_adjust (rela.r_offset, start, adjust);
	  gelfx_update_rela (dso->elf, data, ndx, &rela);
	}
    }

  elf_flagscn (scn, ELF_C_SET, ELF_F_DIRTY);
  return 0;
}
