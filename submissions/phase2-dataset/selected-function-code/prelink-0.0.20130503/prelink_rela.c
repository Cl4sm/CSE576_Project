prelink_rela (DSO *dso, int n, struct prelink_info *info)
{
  Elf_Data *data = NULL;
  Elf_Scn *scn = dso->scn[n];
  GElf_Rela rela;
  int sec;

  while ((data = elf_getdata (scn, data)) != NULL)
    {
      int ndx, maxndx;
      GElf_Addr addr = dso->shdr[n].sh_addr + data->d_off;

      maxndx = data->d_size / dso->shdr[n].sh_entsize;
      for (ndx = 0; ndx < maxndx;
	   ++ndx, addr += dso->shdr[n].sh_entsize)
	{
	  gelfx_getrela (dso->elf, data, ndx, &rela);
	  sec = addr_to_sec (dso, rela.r_offset);
	  if (sec == -1)
	    continue;

	  switch (dso->arch->prelink_rela (info, &rela, addr))
	    {
	    case 2:
	      gelfx_update_rela (dso->elf, data, ndx, &rela);
	      break;
	    case 0:
	      break;
	    default:
	      return 1;
	    }
	}
    }
  return 0;
}
