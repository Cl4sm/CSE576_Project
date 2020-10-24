static int
i386_prelink_conflict_rel (DSO *dso, struct prelink_info *info, GElf_Rel *rel,
			   GElf_Addr reladdr)
{
  GElf_Addr value;
  struct prelink_conflict *conflict;
  struct prelink_tls *tls;
  GElf_Rela *ret;

  if (GELF_R_TYPE (rel->r_info) == R_386_RELATIVE
      || GELF_R_TYPE (rel->r_info) == R_386_NONE)
    /* Fast path: nothing to do.  */
    return 0;
  conflict = prelink_conflict (info, GELF_R_SYM (rel->r_info),
			       GELF_R_TYPE (rel->r_info));
  if (conflict == NULL)
    {
      switch (GELF_R_TYPE (rel->r_info))
	{
	/* Even local DTPMOD and TPOFF relocs need conflicts.  */
	case R_386_TLS_DTPMOD32:
	case R_386_TLS_TPOFF32:
	case R_386_TLS_TPOFF:
	  if (info->curtls == NULL || info->dso == dso)
	    return 0;
	  break;
	/* Similarly IRELATIVE relocations always need conflicts.  */
	case R_386_IRELATIVE:
	  break;
	default:
	  return 0;
	}
      value = 0;
    }
  else if (info->dso == dso && !conflict->ifunc)
    return 0;
  else
    {
      /* DTPOFF32 wants to see only real conflicts, not lookups
	 with reloc_class RTYPE_CLASS_TLS.  */
      if (GELF_R_TYPE (rel->r_info) == R_386_TLS_DTPOFF32
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
    case R_386_GLOB_DAT:
      ret->r_info = GELF_R_INFO (0, R_386_32);
      /* FALLTHROUGH */
    case R_386_JMP_SLOT:
      ret->r_addend = (Elf32_Sword) value;
      if (conflict != NULL && conflict->ifunc)
	ret->r_info = GELF_R_INFO (0, R_386_IRELATIVE);
      break;
    case R_386_IRELATIVE:
      ret->r_addend = (Elf32_Sword) read_ule32 (dso, rel->r_offset);
      break;
    case R_386_32:
    case R_386_PC32:
      error (0, 0, "%s: R_386_%s32 relocs should not be present in prelinked REL sections",
	     dso->filename, GELF_R_TYPE (rel->r_info) == R_386_32 ? "" : "PC");
      return 1;
    case R_386_TLS_DTPMOD32:
    case R_386_TLS_DTPOFF32:
    case R_386_TLS_TPOFF32:
    case R_386_TLS_TPOFF:
      if (conflict != NULL
	  && (conflict->reloc_class != RTYPE_CLASS_TLS
	      || conflict->lookup.tls == NULL))
	{
	  error (0, 0, "%s: R_386_TLS not resolving to STT_TLS symbol",
		 dso->filename);
	  return 1;
	}
      tls = conflict ? conflict->lookup.tls : info->curtls;
      ret->r_info = GELF_R_INFO (0, R_386_32);
      switch (GELF_R_TYPE (rel->r_info))
	{
	case R_386_TLS_DTPMOD32:
	  ret->r_addend = tls->modid;
	  break;
	case R_386_TLS_DTPOFF32:
	  ret->r_addend = value;
	  break;
	case R_386_TLS_TPOFF32:
	  ret->r_addend = -(value + read_ule32 (dso, rel->r_offset)
			    - tls->offset);
	  break;
	case R_386_TLS_TPOFF:
	  ret->r_addend = value + read_ule32 (dso, rel->r_offset)
			  - tls->offset;
	}
      break;
    case R_386_COPY:
      error (0, 0, "R_386_COPY should not be present in shared libraries");
      return 1;
    default:
      error (0, 0, "%s: Unknown i386 relocation type %d", dso->filename,
	     (int) GELF_R_TYPE (rel->r_info));
      return 1;
    }
  return 0;
}
