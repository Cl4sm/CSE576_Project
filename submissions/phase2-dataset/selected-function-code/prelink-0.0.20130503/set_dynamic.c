set_dynamic (DSO *dso, GElf_Word tag, GElf_Addr value, int fatal)
{
  Elf_Data *data;
  Elf_Scn *scn;
  GElf_Dyn dyn;
  int ndx, maxndx;
  uint64_t mask = dso->info_set_mask;

  assert (dso->shdr[dso->dynamic].sh_type == SHT_DYNAMIC);

  scn = dso->scn[dso->dynamic];

  data = elf_getdata (scn, NULL);
  assert (elf_getdata (scn, data) == NULL);

  switch (tag)
    {
    case DT_CHECKSUM:
      mask |= (1ULL << DT_CHECKSUM_BIT); break;
    case DT_GNU_PRELINKED:
      mask |= (1ULL << DT_GNU_PRELINKED_BIT); break;
    case DT_VERDEF:
      mask |= (1ULL << DT_VERDEF_BIT); break;
    case DT_VERNEED:
      mask |= (1ULL << DT_VERNEED_BIT); break;
    case DT_VERSYM:
      mask |= (1ULL << DT_VERSYM_BIT); break;
    default:
      if (tag < DT_NUM && tag < 50)
	mask |= (1ULL << tag);
      break;
    }

  maxndx = data->d_size / dso->shdr[dso->dynamic].sh_entsize;
  for (ndx = 0; ndx < maxndx; ndx++)
    {
      gelfx_getdyn (dso->elf, data, ndx, &dyn);
      if (dyn.d_tag == DT_NULL)
	break;
      else if (dyn.d_tag == tag)
	{
	  if (dyn.d_un.d_ptr != value)
	    {
	      dyn.d_un.d_ptr = value;
	      gelfx_update_dyn (dso->elf, data, ndx, &dyn);
	      elf_flagscn (scn, ELF_C_SET, ELF_F_DIRTY);
	    }

	  return 0;
	}
    }
  assert (ndx < maxndx);

  if (ndx + 1 < maxndx)
    {
      /* DT_NULL is not the last dynamic entry.  */
      gelfx_update_dyn (dso->elf, data, ndx + 1, &dyn);
      dyn.d_tag = tag;
      dyn.d_un.d_ptr = value;
      gelfx_update_dyn (dso->elf, data, ndx, &dyn);
      dso->info_set_mask = mask;
      elf_flagscn (scn, ELF_C_SET, ELF_F_DIRTY);
      return 0;
    }

  if (fatal)
    error (0, 0, "%s: Not enough room to add .dynamic entry",
	   dso->filename);
  return 1;
}
