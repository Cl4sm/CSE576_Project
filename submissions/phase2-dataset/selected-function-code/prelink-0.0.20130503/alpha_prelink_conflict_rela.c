alpha_prelink_conflict_rela (DSO *dso, struct prelink_info *info,
			     GElf_Rela *rela, GElf_Addr relaaddr)
{
  GElf_Addr value;
  struct prelink_conflict *conflict;
  struct prelink_tls *tls;
  GElf_Rela *ret;

  if (GELF_R_TYPE (rela->r_info) == R_ALPHA_RELATIVE
      || GELF_R_TYPE (rela->r_info) == R_ALPHA_NONE
      || info->dso == dso)
    /* Fast path: nothing to do.  */
    return 0;
  conflict = prelink_conflict (info, GELF_R_SYM (rela->r_info),
			       GELF_R_TYPE (rela->r_info));
  if (conflict == NULL)
    {
      if (info->curtls == NULL)
	return 0;
      switch (GELF_R_TYPE (rela->r_info))
	{
	/* Even local DTPMOD64 and TPREL64 relocs need conflicts.  */
	case R_ALPHA_DTPMOD64:
	case R_ALPHA_TPREL64:
	  break;
	default:
	  return 0;
	}
      value = 0;
    }
  else if (conflict->ifunc)
    {
      error (0, 0, "%s: STT_GNU_IFUNC not handled on Alpha yet",
	     dso->filename);
      return 1;
    }
  else
    {
      /* DTPREL64 wants to see only real conflicts, not lookups
	 with reloc_class RTYPE_CLASS_TLS.  */
      if (GELF_R_TYPE (rela->r_info) == R_ALPHA_DTPREL64
	  && conflict->lookup.tls == conflict->conflict.tls
	  && conflict->lookupval == conflict->conflictval)
	return 0;

      value = conflict_lookup_value (conflict);
    }
  ret = prelink_conflict_add_rela (info);
  if (ret == NULL)
    return 1;
  ret->r_offset = rela->r_offset;
  ret->r_info = GELF_R_INFO (0, GELF_R_TYPE (rela->r_info));
  switch (GELF_R_TYPE (rela->r_info))
    {
    case R_ALPHA_GLOB_DAT:
    case R_ALPHA_REFQUAD:
      ret->r_addend = value + rela->r_addend;
      break;
    case R_ALPHA_JMP_SLOT:
      ret->r_addend = value + rela->r_addend;
      if (alpha_is_indirect_plt (dso, rela, relaaddr))
	ret->r_info = GELF_R_INFO (0, R_ALPHA_GLOB_DAT);
      else
	{
	  relaaddr -= dso->info[DT_JMPREL];
	  relaaddr /= sizeof (Elf64_Rela);
	  if (relaaddr > 0xffffff)
	    {
	      error (0, 0, "%s: Cannot create R_ALPHA_JMP_SLOT conflict against .rel.plt with more than 16M entries",
		     dso->filename);
	      return 1;
	    }
	  ret->r_info = GELF_R_INFO (0, (relaaddr << 8) | R_ALPHA_JMP_SLOT);
	}
      break;
    case R_ALPHA_DTPMOD64:
    case R_ALPHA_DTPREL64:
    case R_ALPHA_TPREL64:
      if (conflict != NULL
	  && (conflict->reloc_class != RTYPE_CLASS_TLS
	      || conflict->lookup.tls == NULL))
	{
	  error (0, 0, "%s: TLS reloc not resolving to STT_TLS symbol",
		 dso->filename);
	  return 1;
	}
      tls = conflict ? conflict->lookup.tls : info->curtls;
      ret->r_info = GELF_R_INFO (0, R_ALPHA_GLOB_DAT);
      switch (GELF_R_TYPE (rela->r_info))
	{
	case R_ALPHA_DTPMOD64:
	  ret->r_addend = tls->modid;
	  break;
	case R_ALPHA_DTPREL64:
	  ret->r_addend = value + rela->r_addend;
	  break;
	case R_ALPHA_TPREL64:
	  ret->r_addend = value + rela->r_addend + tls->offset;
	  break;
	}
      break;
    default:
      error (0, 0, "%s: Unknown Alpha relocation type %d", dso->filename,
	     (int) GELF_R_TYPE (rela->r_info));
      return 1;
    }
  return 0;
}
