recompute_nonalloc_offsets (DSO *dso)
{
  int i, first_nonalloc, sec_before_shoff = 0;
  GElf_Addr last_offset = 0;
  GElf_Addr shdralign = gelf_fsize (dso->elf, ELF_T_ADDR, 1, EV_CURRENT);
  GElf_Addr shdrsize = gelf_fsize (dso->elf, ELF_T_SHDR, 1, EV_CURRENT)
		       * dso->ehdr.e_shnum;

  for (i = 1; i < dso->ehdr.e_shnum; ++i)
    if (RELOCATE_SCN (dso->shdr[i].sh_flags))
      {
	if (dso->shdr[i].sh_type == SHT_NOBITS)
	  last_offset = dso->shdr[i].sh_offset;
	else
	  last_offset = dso->shdr[i].sh_offset + dso->shdr[i].sh_size;
      }
    else
      break;

  first_nonalloc = i;
  if (dso->ehdr.e_shoff < dso->shdr[i].sh_offset)
    {
      dso->ehdr.e_shoff = (last_offset + shdralign - 1) & ~(shdralign - 1);
      last_offset = dso->ehdr.e_shoff + shdrsize;
    }
  else
    for (; i < dso->ehdr.e_shnum; ++i)
      if (dso->shdr[i].sh_offset < dso->ehdr.e_shoff
	  && (i == dso->ehdr.e_shnum - 1
	      || dso->shdr[i + 1].sh_offset > dso->ehdr.e_shoff))
	{
	  sec_before_shoff = i;
	  break;
	}

  for (i = first_nonalloc; i < dso->ehdr.e_shnum; ++i)
    {
      assert (!RELOCATE_SCN (dso->shdr[i].sh_flags));
      assert (dso->shdr[i].sh_type != SHT_NOBITS);
      dso->shdr[i].sh_offset = (last_offset + dso->shdr[i].sh_addralign - 1)
			       & ~(dso->shdr[i].sh_addralign - 1);
      last_offset = dso->shdr[i].sh_offset + dso->shdr[i].sh_size;
      if (i == sec_before_shoff)
	{
	  dso->ehdr.e_shoff = (last_offset + shdralign - 1) & ~(shdralign - 1);
	  last_offset = dso->ehdr.e_shoff + shdrsize;
	}
    }

  return 0;
}
