int
convert_rela_to_rel (DSO *dso, int i)
{
  Elf_Data d1, d2, *d;
  Elf_Scn *scn;
  GElf_Rel rel;
  GElf_Rela rela;
  int ndx, maxndx;

  scn = dso->scn[i];
  d = elf_getdata (scn, NULL);
  assert (elf_getdata (scn, d) == NULL);
  assert (d->d_off == 0);
  assert (d->d_size == dso->shdr[i].sh_size);
  d1 = *d;
  d2 = *d;
  assert (sizeof (Elf32_Rel) * 3 == sizeof (Elf32_Rela) * 2);
  assert (sizeof (Elf64_Rel) * 3 == sizeof (Elf64_Rela) * 2);
  d1.d_size = d->d_size / 3 * 2;
  d1.d_buf = malloc (d1.d_size);
  d1.d_type = ELF_T_REL;
  if (d1.d_buf == NULL)
    {
      error (0, ENOMEM, "Cannot convert RELA section to REL");
      return 1;
    }

  maxndx = d->d_size / dso->shdr[i].sh_entsize;
  for (ndx = 0; ndx < maxndx; ndx++)
    {
      if (gelfx_getrela (dso->elf, d, ndx, &rela) == 0
	  || dso->arch->rela_to_rel (dso, &rela, &rel))
	{
	  free (d1.d_buf);
	  return 1;
	}
      /* gelf_update_rela etc. should have Elf * argument, so that
	 we don't have to do this crap.  */
      *d = d1;
      if (gelfx_update_rel (dso->elf, d, ndx, &rel) == 0)
	{
	  *d = d2;
	  free (d1.d_buf);
	  return 1;
	}
      *d = d2;
    }

  free (d2.d_buf);
  *d = d1;
  dso->shdr[i].sh_entsize
    = gelf_fsize (dso->elf, ELF_T_REL, 1, EV_CURRENT);
  dso->shdr[i].sh_type = SHT_REL;
  return 0;
}
