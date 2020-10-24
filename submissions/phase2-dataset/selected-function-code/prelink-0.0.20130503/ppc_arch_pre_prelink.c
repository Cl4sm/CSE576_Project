static int
ppc_arch_pre_prelink (DSO *dso)
{
  Elf_Data *data = NULL;
  Elf_Scn *scn;
  GElf_Dyn dyn;
  Elf32_Addr val;
  int i;

  if (!dynamic_info_is_set (dso, DT_PPC_GOT_BIT))
    return 0;

  assert (dso->shdr[dso->dynamic].sh_type == SHT_DYNAMIC);

  scn = dso->scn[dso->dynamic];
  while ((data = elf_getdata (scn, data)) != NULL)
    {
      int ndx, maxndx;

      maxndx = data->d_size / dso->shdr[dso->dynamic].sh_entsize;
      for (ndx = 0; ndx < maxndx; ++ndx)
	{
	  gelfx_getdyn (dso->elf, data, ndx, &dyn);
	  assert (dyn.d_tag != DT_NULL);
	  if (dyn.d_tag == DT_PPC_GOT)
	    break;
	}
      if (ndx < maxndx)
	break;
    }

  /* DT_PPC_GOT[1] should point to .glink in prelinked libs.  */
  val = read_ube32 (dso, dyn.d_un.d_ptr + 4);
  if (val)
    return 0;

  for (i = 1; i < dso->ehdr.e_shnum; ++i)
    if (! strcmp (strptr (dso, dso->ehdr.e_shstrndx,
			  dso->shdr[i].sh_name), ".plt"))
      break;

  if (i == dso->ehdr.e_shnum)
    return 0;

  val = read_ube32 (dso, dso->shdr[i].sh_addr);
  write_be32 (dso, dyn.d_un.d_ptr + 4, val);

  return 0;
}
