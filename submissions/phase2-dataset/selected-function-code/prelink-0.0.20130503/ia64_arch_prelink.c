ia64_arch_prelink (struct prelink_info *info)
{
  DSO *dso;
  int plt = -1, got = -1, i;
  const char *name;

  /* Write address of .plt + 0x30 into .got[1].
     .plt + 0x30 is what .IA_64.pltoff[0] contains unless prelinking.  */

  dso = info->dso;
  for (i = 1; i < dso->ehdr.e_shnum; i++)
    if (dso->shdr[i].sh_type == SHT_PROGBITS)
      {
	name = strptr (dso, dso->ehdr.e_shstrndx, dso->shdr[i].sh_name);
	if (! strcmp (name, ".got"))
	  {
	    if (got != -1)
	      {
		error (0, 0, "%s: More than one .got section", dso->filename);
		return 1;
	      }
	    got = i;
	  }
	else if (! strcmp (name, ".plt"))
	  {
	    if (plt != -1)
	      {
		error (0, 0, "%s: More than one .plt section", dso->filename);
		return 1;
	      }
	    plt = i;
	  }
      }

  if (plt == -1)
    return 0;

  if (got == -1)
    {
      error (0, 0, "%s: Has .plt section but not .got section", dso->filename);
      return 1;
    }

  write_le64 (dso, dso->shdr[got].sh_addr + 8, dso->shdr[plt].sh_addr + 0x30);
  return 0;
}
