static int
x86_64_prelink_conflict_rela (DSO *dso, struct prelink_info *info,
			    GElf_Rela *rela, GElf_Addr relaaddr)
{
  GElf_Addr value;
  struct prelink_conflict *conflict;
  struct prelink_tls *tls;
  GElf_Rela *ret;

  if (GELF_R_TYPE (rela->r_info) == R_X86_64_RELATIVE
      || GELF_R_TYPE (rela->r_info) == R_X86_64_NONE)
    /* Fast path: nothing to do.  */
    return 0;
  conflict = prelink_conflict (info, GELF_R_SYM (rela->r_info),
			       GELF_R_TYPE (rela->r_info));
  if (conflict == NULL)
    {
      switch (GELF_R_TYPE (rela->r_info))
	{
	/* Even local DTPMOD and TPOFF relocs need conflicts.  */
	case R_X86_64_DTPMOD64:
	case R_X86_64_TPOFF64:
	  if (info->curtls == NULL || info->dso == dso)
	    return 0;
	  break;
	/* Similarly IRELATIVE relocations always need conflicts.  */
	case R_X86_64_IRELATIVE:
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
      /* DTPOFF wants to see only real conflicts, not lookups
	 with reloc_class RTYPE_CLASS_TLS.  */
      if (GELF_R_TYPE (rela->r_info) == R_X86_64_DTPOFF64
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
    case R_X86_64_GLOB_DAT:
      ret->r_info = GELF_R_INFO (0, R_X86_64_64);
      /* FALLTHROUGH */
    case R_X86_64_JUMP_SLOT:
    case R_X86_64_64:
    case R_X86_64_IRELATIVE:
      ret->r_addend = value + rela->r_addend;
      if (conflict != NULL && conflict->ifunc)
	ret->r_info = GELF_R_INFO (0, R_X86_64_IRELATIVE);
      break;
    case R_X86_64_32:
      value += rela->r_addend;
      ret->r_addend = value;
      break;
    case R_X86_64_PC32:
      ret->r_addend = value + rela->r_addend - rela->r_offset;
      ret->r_info = GELF_R_INFO (0, R_X86_64_32);
      break;
    case R_X86_64_COPY:
      error (0, 0, "R_X86_64_COPY should not be present in shared libraries");
      return 1;
    case R_X86_64_DTPMOD64:
    case R_X86_64_DTPOFF64:
    case R_X86_64_TPOFF64:
      if (conflict != NULL
	  && (conflict->reloc_class != RTYPE_CLASS_TLS
	      || conflict->lookup.tls == NULL))
	{
	  error (0, 0, "%s: TLS reloc not resolving to STT_TLS symbol",
		 dso->filename);
	  return 1;
	}
      tls = conflict ? conflict->lookup.tls : info->curtls;
      ret->r_info = GELF_R_INFO (0, R_X86_64_64);
      switch (GELF_R_TYPE (rela->r_info))
	{
	case R_X86_64_DTPMOD64:
	  ret->r_addend = tls->modid;
	  break;
	case R_X86_64_DTPOFF64:
	  ret->r_addend = value + rela->r_addend;
	  break;
	case R_X86_64_TPOFF64:
	  ret->r_addend = value + rela->r_addend - tls->offset;
	  break;
	}
      break;

    default:
      error (0, 0, "%s: Unknown X86-64 relocation type %d", dso->filename,
	     (int) GELF_R_TYPE (rela->r_info));
      return 1;
    }
  return 0;
}
