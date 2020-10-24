static int
sparc_prelink_conflict_rela (DSO *dso, struct prelink_info *info,
			     GElf_Rela *rela, GElf_Addr relaaddr)
{
  GElf_Addr value;
  struct prelink_conflict *conflict;
  struct prelink_tls *tls;
  GElf_Rela *ret;
  int r_type;

  if (GELF_R_TYPE (rela->r_info) == R_SPARC_RELATIVE
      || GELF_R_TYPE (rela->r_info) == R_SPARC_NONE
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
	/* Even local DTPMOD32 and TPOFF32 relocs need conflicts.  */
	case R_SPARC_TLS_DTPMOD32:
	case R_SPARC_TLS_TPOFF32:
	case R_SPARC_TLS_LE_HIX22:
	case R_SPARC_TLS_LE_LOX10:
	  break;
	default:
	  return 0;
	}
      value = 0;
    }
  else if (conflict->ifunc)
    {
      error (0, 0, "%s: STT_GNU_IFUNC not handled on SPARC yet",
	     dso->filename);
      return 1;
    }
  else
    {
      /* DTPOFF32 wants to see only real conflicts, not lookups
	 with reloc_class RTYPE_CLASS_TLS.  */
      if (GELF_R_TYPE (rela->r_info) == R_SPARC_TLS_DTPOFF32
	  && conflict->lookup.tls == conflict->conflict.tls
	  && conflict->lookupval == conflict->conflictval)
	return 0;

      value = conflict_lookup_value (conflict);
    }
  ret = prelink_conflict_add_rela (info);
  if (ret == NULL)
    return 1;
  ret->r_offset = rela->r_offset;
  value += rela->r_addend;
  r_type = GELF_R_TYPE (rela->r_info);
  switch (r_type)
    {
    case R_SPARC_DISP32:
      value -= rela->r_offset;
    case R_SPARC_GLOB_DAT:
    case R_SPARC_32:
      r_type = R_SPARC_32;
      break;
    case R_SPARC_DISP16:
      value -= rela->r_offset;
    case R_SPARC_16:
      r_type = R_SPARC_16;
      break;
    case R_SPARC_DISP8:
      value -= rela->r_offset;
    case R_SPARC_8:
      r_type = R_SPARC_8;
      break;
    /* Attempt to transform all reloc which read-modify-write into
       simple writes.  */
    case R_SPARC_LO10:
      value = (read_ube32 (dso, rela->r_offset) & ~0x3ff) | (value & 0x3ff);
      r_type = R_SPARC_32;
      break;
    case R_SPARC_HI22:
      value = (read_ube32 (dso, rela->r_offset) & 0xffc00000)
	      | ((value >> 10) & 0x3fffff);
      r_type = R_SPARC_32;
      break;
    case R_SPARC_WDISP30:
      value = (read_ube32 (dso, rela->r_offset) & 0xc0000000)
	      | (((value - rela->r_offset) >> 2) & 0x3fffffff);
      r_type = R_SPARC_32;
      break;
    case R_SPARC_UA16:
    case R_SPARC_UA32:
    case R_SPARC_JMP_SLOT:
      break;
    case R_SPARC_TLS_DTPMOD32:
    case R_SPARC_TLS_DTPOFF32:
    case R_SPARC_TLS_TPOFF32:
    case R_SPARC_TLS_LE_HIX22:
    case R_SPARC_TLS_LE_LOX10:
      if (conflict != NULL
	  && (conflict->reloc_class != RTYPE_CLASS_TLS
	      || conflict->lookup.tls == NULL))
	{
	  error (0, 0, "%s: TLS reloc not resolving to STT_TLS symbol",
		 dso->filename);
	  return 1;
	}
      r_type = R_SPARC_32;
      tls = conflict ? conflict->lookup.tls : info->curtls;
      switch (GELF_R_TYPE (rela->r_info))
	{
	case R_SPARC_TLS_DTPMOD32:
	  value = tls->modid;
	  break;
	case R_SPARC_TLS_DTPOFF32:
	  break;
	case R_SPARC_TLS_TPOFF32:
	  value -= tls->offset;
	  break;
	case R_SPARC_TLS_LE_HIX22:
	  value -= tls->offset;
	  value = (read_ube32 (dso, rela->r_offset) & 0xffc00000)
		  | (((~value) >> 10) & 0x3fffff);
	  break;
	case R_SPARC_TLS_LE_LOX10:
	  value -= tls->offset;
	  value = (read_ube32 (dso, rela->r_offset) & 0xffffe000) | 0x1c00
		  | (value & 0x3ff);
	  break;
	}
      break;
    default:
      error (0, 0, "%s: Unknown Sparc relocation type %d", dso->filename,
	     r_type);
      return 1;
    }
  ret->r_info = GELF_R_INFO (0, r_type);
  ret->r_addend = (Elf32_Sword) value;
  return 0;
}
