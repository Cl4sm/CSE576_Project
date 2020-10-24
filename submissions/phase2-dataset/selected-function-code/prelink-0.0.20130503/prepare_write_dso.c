prepare_write_dso (DSO *dso)
{
  int i;

  if (check_dso (dso)
      || (dso->mdebug_orig_offset && finalize_mdebug (dso)))
    return 1;

  gelf_update_ehdr (dso->elf, &dso->ehdr);
  for (i = 0; i < dso->ehdr.e_phnum; ++i)
    gelf_update_phdr (dso->elf, i, dso->phdr + i);
  for (i = 0; i < dso->ehdr.e_shnum; ++i)
    {
      gelfx_update_shdr (dso->elf, dso->scn[i], dso->shdr + i);
      if (dso->shdr[i].sh_type == SHT_SYMTAB
	  || dso->shdr[i].sh_type == SHT_DYNSYM)
	set_stt_section_values (dso, i);
    }
  return 0;
}
