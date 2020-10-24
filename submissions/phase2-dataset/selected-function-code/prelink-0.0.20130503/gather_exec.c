gather_exec (DSO *dso, const struct stat64 *st)
{
  int i, j;
  Elf_Data *data;
  const char *dl;
  struct prelink_entry *ent;

  if (verbose > 5)
    printf ("Checking executable %s\n", dso->filename);

  for (i = 0; i < dso->ehdr.e_phnum; ++i)
    if (dso->phdr[i].p_type == PT_INTERP)
      break;

  /* If there are no PT_INTERP segments, it is statically linked.  */
  if (i == dso->ehdr.e_phnum)
    {
make_unprelinkable:
      if (undo)
	goto error_out;

      ent = prelink_find_entry (dso->filename, st, 1);
      if (ent == NULL)
	goto error_out;

      assert (ent->type == ET_NONE);
      ent->type = ET_UNPRELINKABLE;
      if (dso)
	close_dso (dso);
      return 0;
    }

  j = addr_to_sec (dso, dso->phdr[i].p_vaddr);
  if (j == -1 || dso->shdr[j].sh_addr != dso->phdr[i].p_vaddr
      || dso->shdr[j].sh_type != SHT_PROGBITS)
    {
      error (0, 0, "%s: PT_INTERP segment not corresponding to .interp section",
	     dso->filename);
      goto make_unprelinkable;
    }

  data = elf_getdata (dso->scn[j], NULL);
  if (data == NULL)
    {
      error (0, 0, "%s: Could not read .interp section", dso->filename);
      goto error_out;
    }

  i = strnlen (data->d_buf, data->d_size);
  if (i == data->d_size)
    {
      error (0, 0, "%s: .interp section not zero terminated", dso->filename);
      goto error_out;
    }

  dl = dynamic_linker ?: dso->arch->dynamic_linker;
  if (strcmp (dl, data->d_buf) != 0)
    {
      error (0, 0, "%s: Using %s, not %s as dynamic linker", dso->filename,
	     (char *) data->d_buf, dl);
      goto error_out;
    }

  ent = prelink_find_entry (dso->filename, st, 1);
  if (ent == NULL)
    goto error_out;

  assert (ent->type == ET_NONE);
  ent->u.explicit = 1;

  if (gather_deps (dso, ent))
    return 0;

  for (i = 0; i < ent->ndepends; ++i)
    ++ent->depends[i]->refs;

  ent->type = ET_EXEC;
  return 0;

error_out:
  if (dso)
    close_dso (dso);
  return 0;
}
