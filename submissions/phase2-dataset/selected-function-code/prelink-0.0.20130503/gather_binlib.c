static int
gather_binlib (const char *name, const struct stat64 *st)
{
  unsigned char e_ident [EI_NIDENT + 2];
  int fd, type;
  DSO *dso;
  struct prelink_entry *ent;

  if (! S_ISREG (st->st_mode))
    {
      error (0, 0, "%s is not a regular file", name);
      return 1;
    }

  ent = prelink_find_entry (name, st, 0);
  if (ent != NULL && ent->type == ET_UNPRELINKABLE)
    {
      free (ent->depends);
      ent->depends = NULL;
      ent->ndepends = 0;
      ent->type = ET_NONE;
    }
  if (ent != NULL && ent->type != ET_NONE)
    {
      ent->u.explicit = 1;
      return 0;
    }

  fd = open (name, O_RDONLY);
  if (fd == -1)
    {
      error (0, errno, "Could not open %s", name);
      return 1;
    }

  if (read (fd, e_ident, sizeof (e_ident)) != sizeof (e_ident))
    {
      error (0, errno, "Could not read ELF header from %s", name);
      close (fd);
      return 1;
    }

  /* Quickly find ET_EXEC/ET_DYN ELF binaries/libraries only.  */

  if (memcmp (e_ident, ELFMAG, SELFMAG) != 0)
    {
      error (0, 0, "%s is not an ELF object", name);
      close (fd);
      return 1;
    }

  switch (e_ident [EI_DATA])
    {
    case ELFDATA2LSB:
      if (e_ident [EI_NIDENT + 1] != 0)
	goto unsupported_type;
      type = e_ident [EI_NIDENT];
      break;
    case ELFDATA2MSB:
      if (e_ident [EI_NIDENT] != 0)
	goto unsupported_type;
      type = e_ident [EI_NIDENT + 1];
      break;
    default:
      goto unsupported_type;
    }

  if (type != ET_EXEC && type != ET_DYN)
    {
unsupported_type:
      error (0, 0, "%s is neither ELF executable nor ELF shared library", name);
      close (fd);
      return 1;
    }

  dso = fdopen_dso (fd, name);
  if (dso == NULL)
    return 0;

  if (type == ET_EXEC)
    {
      int i;

      for (i = 0; i < dso->ehdr.e_phnum; ++i)
	if (dso->phdr[i].p_type == PT_INTERP)
	  break;

      /* If there are no PT_INTERP segments, it is statically linked.  */
      if (i == dso->ehdr.e_phnum)
	{
	  error (0, 0, "%s is statically linked", name);
	  close_dso (dso);
	  return 1;
	}

      return gather_exec (dso, st);
    }

  ent = prelink_find_entry (name, st, 1);
  if (ent == NULL)
    {
      close_dso (dso);
      return 1;
    }

  assert (ent->type == ET_NONE);
  ent->type = ET_BAD;
  ent->u.explicit = 1;
  return gather_dso (dso, ent);
}
