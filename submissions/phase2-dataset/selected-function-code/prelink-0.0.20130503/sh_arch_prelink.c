sh_arch_prelink (struct prelink_info *info)
{
  DSO *dso;
  int i;

  dso = info->dso;
  if (dso->info[DT_PLTGOT])
    {
      /* Write address of .plt + 36 into got[1].
	 .plt + 36 is what got[3] contains unless prelinking.  */
      int sec = addr_to_sec (dso, dso->info[DT_PLTGOT]);
      Elf32_Addr data;

      if (sec == -1)
	return 1;

      for (i = 1; i < dso->ehdr.e_shnum; i++)
	if (dso->shdr[i].sh_type == SHT_PROGBITS
	    && ! strcmp (strptr (dso, dso->ehdr.e_shstrndx,
				 dso->shdr[i].sh_name),
			 ".plt"))
	break;

      if (i == dso->ehdr.e_shnum)
	return 0;
      data = dso->shdr[i].sh_addr + 36;
      write_ne32 (dso, dso->info[DT_PLTGOT] + 4, data);
    }

  return 0;
}
