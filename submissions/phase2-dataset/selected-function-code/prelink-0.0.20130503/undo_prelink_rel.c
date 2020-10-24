undo_prelink_rel (DSO *dso, int n)
{
  Elf_Data *data = NULL;
  Elf_Scn *scn = dso->scn[n];
  GElf_Rel rel;
  int sec;

  if (dso->arch->undo_prelink_rel == NULL)
    return 0;
  while ((data = elf_getdata (scn, data)) != NULL)
    {
      int ndx, maxndx;
      GElf_Addr addr = dso->shdr[n].sh_addr + data->d_off;

      maxndx = data->d_size / dso->shdr[n].sh_entsize;
      for (ndx = 0; ndx < maxndx;
	   ++ndx, addr += dso->shdr[n].sh_entsize)
	{
	  gelfx_getrel (dso->elf, data, ndx, &rel);
	  sec = addr_to_sec (dso, rel.r_offset);
	  if (sec == -1)
	    continue;

	  switch (dso->arch->undo_prelink_rel (dso, &rel, addr))
	    {
	    case 2:
	      gelfx_update_rel (dso->elf, data, ndx, &rel);
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
