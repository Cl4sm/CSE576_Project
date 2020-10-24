int
check_dso (DSO *dso)
{
  int i, last = 1;

  /* FIXME: Several routines in prelink and in libelf-0.7.0 too
     rely on sh_offset's monotonically increasing.  */
  for (i = 2; i < dso->ehdr.e_shnum; ++i)
    {
      if (dso->shdr[last].sh_offset
	  + (dso->shdr[last].sh_type == SHT_NOBITS
	     ? 0 : dso->shdr[last].sh_size) > dso->shdr[i].sh_offset)
	{
	  if (!dso->permissive
	      || RELOCATE_SCN (dso->shdr[last].sh_flags)
	      || RELOCATE_SCN (dso->shdr[i].sh_flags))
	    {
	      error (0, 0, "%s: section file offsets not monotonically increasing",
		     dso->filename);
	      return 1;
	    }
	}
      if (!dso->permissive
	  || (dso->shdr[i].sh_type != SHT_NOBITS && dso->shdr[i].sh_size != 0))
	last = i;
    }
  return 0;
}
