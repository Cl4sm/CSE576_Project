shstrtabadd (DSO *dso, const char *name)
{
  Elf_Scn *scn;
  Elf_Data *data;
  GElf_Addr adjust;
  const char *p, *q, *r;
  size_t len = strlen (name), align;
  int ret;

  scn = dso->scn[dso->ehdr.e_shstrndx];
  data = elf_getdata (scn, NULL);
  assert (elf_getdata (scn, data) == NULL);
  assert (data->d_off == 0);
  assert (data->d_size == dso->shdr[dso->ehdr.e_shstrndx].sh_size);
  q = data->d_buf + data->d_size;
  for (p = data->d_buf; p < q; p = r + 1)
    {
      r = strchr (p, '\0');
      if (r - p >= len && memcmp (r - len, name, len) == 0)
	return (r - (const char *) data->d_buf) - len;
    }

  data->d_buf = realloc (data->d_buf, data->d_size + len + 1);
  if (data->d_buf == NULL)
    {
      error (0, ENOMEM, "Cannot add new section name %s", name);
      return 0;
    }

  memcpy (data->d_buf + data->d_size, name, len + 1);
  ret = data->d_size;
  data->d_size += len + 1;
  align = gelf_fsize (dso->elf, ELF_T_ADDR, 1, EV_CURRENT);
  adjust = (len + 1 + align - 1) & ~(align - 1);
  if (adjust_dso_nonalloc (dso, 0,
			   dso->shdr[dso->ehdr.e_shstrndx].sh_offset
			   + dso->shdr[dso->ehdr.e_shstrndx].sh_size,
			   adjust))
    return 0;
  dso->shdr[dso->ehdr.e_shstrndx].sh_size += len + 1;
  return ret;
}
