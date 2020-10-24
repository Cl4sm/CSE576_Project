prelink_conflict_rel (DSO *dso, int n, struct prelink_info *info)
{
  Elf_Data *data = NULL;
  Elf_Scn *scn = dso->scn[n];
  GElf_Rel rel;
  int sec, ndx, maxndx;

  while ((data = elf_getdata (scn, data)) != NULL)
    {
      GElf_Addr addr = dso->shdr[n].sh_addr + data->d_off;

      maxndx = data->d_size / dso->shdr[n].sh_entsize;
      for (ndx = 0; ndx < maxndx;
	   ++ndx, addr += dso->shdr[n].sh_entsize)
	{
	  gelfx_getrel (dso->elf, data, ndx, &rel);
	  sec = addr_to_sec (dso, rel.r_offset);
	  if (sec == -1)
	    continue;

	  if (dso->arch->prelink_conflict_rel (dso, info, &rel, addr))
	    return 1;
	}
    }
  return 0;
}
