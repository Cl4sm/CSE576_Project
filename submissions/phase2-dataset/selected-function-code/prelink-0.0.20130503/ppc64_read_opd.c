ppc64_read_opd (DSO *dso, struct prelink_entry *ent)
{
  int opd;
  GElf_Addr n, s;

  free (ent->opd);
  ent->opd = NULL;
  for (opd = 1; opd < dso->ehdr.e_shnum; ++opd)
    if (dso->shdr[opd].sh_type == SHT_PROGBITS
	&& ! strcmp (strptr (dso, dso->ehdr.e_shstrndx,
			     dso->shdr[opd].sh_name), ".opd"))
      break;
  if (opd == dso->ehdr.e_shnum)
    return 0;
  ent->opd = malloc (sizeof (struct opd_lib) + dso->shdr[opd].sh_size);
  /* The error will happen only when we'll need the opd.  */
  if (ent->opd == NULL)
    return 0;
  s = dso->shdr[opd].sh_addr;
  for (n = 0; n < dso->shdr[opd].sh_size / 8; ++n, s += 8)
    ent->opd->table[n] = read_ube64 (dso, s);
  ent->opd->start = dso->shdr[opd].sh_addr;
  ent->opd->size = dso->shdr[opd].sh_size;
  return 0;
}
