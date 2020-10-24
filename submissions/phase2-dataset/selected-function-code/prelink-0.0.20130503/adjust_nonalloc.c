int
adjust_nonalloc (DSO *dso, GElf_Ehdr *ehdr, GElf_Shdr *shdr, int first,
		 GElf_Addr start, GElf_Addr adjust)
{
  int i;

  for (i = 1; i < ehdr->e_shnum; i++)
    {
      if (RELOCATE_SCN (shdr[i].sh_flags) || shdr[i].sh_type == SHT_NULL)
	continue;

      if ((shdr[i].sh_offset > start
	   || (shdr[i].sh_offset == start && i >= first))
	  && (adjust & (shdr[i].sh_addralign - 1)))
	adjust = (adjust + shdr[i].sh_addralign - 1)
		 & ~(shdr[i].sh_addralign - 1);
    }

  if (ehdr->e_shoff >= start)
    {
      GElf_Addr shdralign = gelf_fsize (dso->elf, ELF_T_ADDR, 1, EV_CURRENT);

      if (adjust & (shdralign - 1))
	adjust = (adjust + shdralign - 1) & ~(shdralign - 1);
      ehdr->e_shoff += adjust;
    }

  for (i = 1; i < ehdr->e_shnum; i++)
    {
      if (RELOCATE_SCN (shdr[i].sh_flags) || shdr[i].sh_type == SHT_NULL)
	continue;

      if (shdr[i].sh_offset > start
	  || (shdr[i].sh_offset == start && i >= first))
	shdr[i].sh_offset += adjust;
    }
  return 0;
}
