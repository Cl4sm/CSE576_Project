arm_prelink_conflict_rel (DSO *dso, struct prelink_info *info, GElf_Rel *rel,
			  GElf_Addr reladdr)
{
  GElf_Addr value;
  struct prelink_conflict *conflict;
  struct prelink_tls *tls;
  GElf_Rela *ret;

  if (GELF_R_TYPE (rel->r_info) == R_ARM_RELATIVE
      || GELF_R_TYPE (rel->r_info) == R_ARM_NONE
      || info->dso == dso)
    /* Fast path: nothing to do.  */
    return 0;
  conflict = prelink_conflict (info, GELF_R_SYM (rel->r_info),
			       GELF_R_TYPE (rel->r_info));
  if (conflict == NULL)
    {
      if (info->curtls == NULL)
	return 0;

      switch (GELF_R_TYPE (rel->r_info))
	{
	/* Even local DTPMOD and TPOFF relocs need conflicts.  */
	case R_ARM_TLS_DTPMOD32:
	case R_ARM_TLS_TPOFF32:
	  break;

	default:
	  return 0;
	}
      value = 0;
    }
  else if (conflict->ifunc)
    {
      error (0, 0, "%s: STT_GNU_IFUNC not handled on ARM yet",
	     dso->filename);
      return 1;
    }
  else
    {
      /* DTPOFF32 wants to see only real conflicts, not lookups
	 with reloc_class RTYPE_CLASS_TLS.  */
      if (GELF_R_TYPE (rel->r_info) == R_ARM_TLS_DTPOFF32
	  && conflict->lookup.tls == conflict->conflict.tls
	  && conflict->lookupval == conflict->conflictval)
        return 0;

      value = conflict_lookup_value (conflict);
    }

  ret = prelink_conflict_add_rela (info);
  if (ret == NULL)
    return 1;
  ret->r_offset = rel->r_offset;
  ret->r_info = GELF_R_INFO (0, GELF_R_TYPE (rel->r_info));
  switch (GELF_R_TYPE (rel->r_info))
    {
    case R_ARM_GLOB_DAT:
    case R_ARM_JUMP_SLOT:
      ret->r_addend = (Elf32_Sword) value;
      break;
    case R_ARM_ABS32:
    case R_ARM_PC24:
      error (0, 0, "%s: R_ARM_%s relocs should not be present in prelinked REL sections",
	     dso->filename, GELF_R_TYPE (rel->r_info) == R_ARM_ABS32 ? "ABS32" : "PC24");
      return 1;
    case R_ARM_COPY:
      error (0, 0, "R_ARM_COPY should not be present in shared libraries");
      return 1;
    case R_ARM_TLS_DTPMOD32:
    case R_ARM_TLS_DTPOFF32:
    case R_ARM_TLS_TPOFF32:
      if (conflict != NULL
	  && (conflict->reloc_class != RTYPE_CLASS_TLS
	      || conflict->lookup.tls == NULL))
	{
	  error (0, 0, "%s: TLS reloc not resolving to STT_TLS symbol",
		 dso->filename);
	  return 1;
	}
      tls = conflict ? conflict->lookup.tls : info->curtls;
      ret->r_info = GELF_R_INFO (0, R_ARM_ABS32);
      switch (GELF_R_TYPE (rel->r_info))
	{
	case R_ARM_TLS_DTPMOD32:
	  ret->r_addend = tls->modid;
	  break;
	case R_ARM_TLS_DTPOFF32:
	  ret->r_addend = value;
	  break;
	case R_ARM_TLS_TPOFF32:
	  ret->r_addend = (value + read_une32 (dso, rel->r_offset)
			   + tls->offset);
	  break;
	}
      break;
    default:
      error (0, 0, "%s: Unknown arm relocation type %d", dso->filename,
	     (int) GELF_R_TYPE (rel->r_info));
      return 1;
    }
  return 0;
}
