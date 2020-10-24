remove_dynamic_prelink_tags (DSO *dso)
{
  Elf_Data *data;
  Elf_Scn *scn;
  GElf_Dyn dyn;
  int ndx;

  assert (dso->shdr[dso->dynamic].sh_type == SHT_DYNAMIC);
  scn = dso->scn[dso->dynamic];
  data = elf_getdata (scn, NULL);
  assert (elf_getdata (scn, data) == NULL);
  ndx = data->d_size / dso->shdr[dso->dynamic].sh_entsize;
  while (--ndx >= 0)
    {
      gelfx_getdyn (dso->elf, data, ndx, &dyn);
      switch (dyn.d_tag)
	{
	case DT_NULL:
	  continue;
	case DT_CHECKSUM:
	case DT_GNU_PRELINKED:
	case DT_GNU_LIBLIST:
	case DT_GNU_LIBLISTSZ:
	case DT_GNU_CONFLICT:
	case DT_GNU_CONFLICTSZ:
	  dyn.d_tag = DT_NULL;
	  dyn.d_un.d_val = 0;
	  gelfx_update_dyn (dso->elf, data, ndx, &dyn);
	  elf_flagscn (scn, ELF_C_SET, ELF_F_DIRTY);
	  break;
	default:
	  ndx = 0;
	  break;
	}
    }
  return 0;
}
