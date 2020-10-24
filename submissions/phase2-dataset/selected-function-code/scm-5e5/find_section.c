static int
find_section (name, section_names, file_name, old_file_h, old_section_h, noerror)
     char *name;
     char *section_names;
     char *file_name;
     Elf32_Ehdr *old_file_h;
     Elf32_Shdr *old_section_h;
     int noerror;
{
  int idx;

  for (idx = 1; idx < old_file_h->e_shnum; idx++)
    {
#ifdef DEBUG
      fprintf (stderr, "Looking for %s - found %s\n", name,
	       section_names + OLD_SECTION_H (idx).sh_name);
#endif
      if (!strcmp (section_names + OLD_SECTION_H (idx).sh_name,
		   name))
	break;
    }
  if (idx == old_file_h->e_shnum)
    {
      if (noerror)
	return -1;
      else
	fatal ("Can't find .bss in %s.\n", file_name, 0);
    }

  return idx;
}
