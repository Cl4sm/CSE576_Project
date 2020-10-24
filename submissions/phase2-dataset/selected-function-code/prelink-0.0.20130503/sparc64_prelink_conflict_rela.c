sparc64_prelink_conflict_rela (DSO *dso, struct prelink_info *info,
			       GElf_Rela *rela, GElf_Addr relaaddr)
{
  GElf_Addr value;
  struct prelink_conflict *conflict;
  struct prelink_tls *tls;
  GElf_Rela *ret;
  int r_type;

  if (SPARC64_R_TYPE (rela->r_info) == R_SPARC_RELATIVE
      || SPARC64_R_TYPE (rela->r_info) == R_SPARC_NONE
      || info->dso == dso)
    /* Fast path: nothing to do.  */
    return 0;
  conflict = prelink_conflict (info, GELF_R_SYM (rela->r_info),
			       SPARC64_R_TYPE (rela->r_info));
  if (conflict == NULL)
    {
      if (info->curtls == NULL)
	return 0;
      switch (SPARC64_R_TYPE (rela->r_info))
	{
	/* Even local DTPMOD64 and TPOFF64 relocs need conflicts.  */
	case R_SPARC_TLS_DTPMOD64:
	case R_SPARC_TLS_TPOFF64:
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
      error (0, 0, "%s: STT_GNU_IFUNC not handled on SPARC64 yet",
	     dso->filename);
      return 1;
    }
  else
    {
      /* DTPOFF64 wants to see only real conflicts, not lookups
	 with reloc_class RTYPE_CLASS_TLS.  */
      if (SPARC64_R_TYPE (rela->r_info) == R_SPARC_TLS_DTPOFF64
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
  r_type = SPARC64_R_TYPE (rela->r_info);
  switch (r_type)
    {
    case R_SPARC_DISP64:
      value -= rela->r_offset;
    case R_SPARC_GLOB_DAT:
    case R_SPARC_64:
      r_type = R_SPARC_64;
      break;
    case R_SPARC_DISP32:
      value -= rela->r_offset;
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
    case R_SPARC_LM22:
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
    case R_SPARC_H44:
      value = (read_ube32 (dso, rela->r_offset) & 0xffc00000)
	      | ((value >> 22) & 0x3fffff);
      r_type = R_SPARC_32;
      break;
    case R_SPARC_M44:
      value = (read_ube32 (dso, rela->r_offset) & ~0x3ff)
	      | ((value >> 12) & 0x3ff);
      r_type = R_SPARC_32;
      break;
    case R_SPARC_L44:
      value = (read_ube32 (dso, rela->r_offset) & ~0xfff) | (value & 0xfff);
      r_type = R_SPARC_32;
      break;
    case R_SPARC_HH22:
      value = (read_ube32 (dso, rela->r_offset) & 0xffc00000)
	      | ((value >> 42) & 0x3fffff);
      r_type = R_SPARC_32;
      break;
    case R_SPARC_HM10:
      value = (read_ube32 (dso, rela->r_offset) & ~0x3ff)
	      | ((value >> 32) & 0x3ff);
      r_type = R_SPARC_32;
      break;
    case R_SPARC_OLO10:
      value = (read_ube32 (dso, rela->r_offset) & ~0x1fff)
	      | (((value & 0x3ff) + (GELF_R_TYPE (rela->r_info) >> 8)) & 0x1fff);
      r_type = R_SPARC_32;
      break;
    case R_SPARC_JMP_SLOT:
      if (rela->r_addend)
	r_type = R_SPARC_64;
      break;
    case R_SPARC_UA16:
    case R_SPARC_UA32:
    case R_SPARC_UA64:
      break;
    case R_SPARC_TLS_DTPMOD64:
    case R_SPARC_TLS_DTPOFF64:
    case R_SPARC_TLS_TPOFF64:
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
      tls = conflict ? conflict->lookup.tls : info->curtls;
      switch (r_type)
	{
	case R_SPARC_TLS_DTPMOD64:
	  r_type = R_SPARC_64;
	  value = tls->modid;
	  break;
	case R_SPARC_TLS_DTPOFF64:
	  r_type = R_SPARC_64;
	  break;
	case R_SPARC_TLS_TPOFF64:
	  r_type = R_SPARC_64;
	  value -= tls->offset;
	  break;
	case R_SPARC_TLS_LE_HIX22:
	  r_type = R_SPARC_32;
	  value -= tls->offset;
	  value = (read_ube32 (dso, rela->r_offset) & 0xffc00000)
		  | (((~value) >> 10) & 0x3fffff);
	  break;
	case R_SPARC_TLS_LE_LOX10:
	  r_type = R_SPARC_32;
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
  ret->r_addend = value;
  return 0;
}
