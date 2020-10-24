int
strtabfind (DSO *dso, int strndx, const char *name)
{
  Elf_Scn *scn;
  Elf_Data *data;
  const char *p, *q, *r;
  size_t len = strlen (name);

  if (dso->shdr[strndx].sh_type != SHT_STRTAB)
    return 0;

  scn = dso->scn[strndx];
  data = elf_getdata (scn, NULL);
  assert (elf_getdata (scn, data) == NULL);
  assert (data->d_off == 0);
  assert (data->d_size == dso->shdr[strndx].sh_size);
  q = data->d_buf + data->d_size;
  for (p = data->d_buf; p < q; p = r + 1)
    {
      r = strchr (p, '\0');
      if (r - p >= len && memcmp (r - len, name, len) == 0)
	return (r - (const char *) data->d_buf) - len;
    }

  return 0;
}
