static int
mips_prelink_conflict_reloc (DSO *dso, struct prelink_info *info,
			     GElf_Addr r_offset, GElf_Xword r_info,
			     GElf_Rela *rela)
{
  GElf_Addr value;
  struct prelink_conflict *conflict;
  struct prelink_tls *tls = NULL;
  GElf_Rela *entry;

  if (info->dso == dso)
    return 0;

  conflict = prelink_conflict (info, GELF_R_SYM (r_info),
			       GELF_R_TYPE (r_info));
  if (conflict == NULL)
    {
      switch (GELF_R_TYPE (r_info))
	{
	case R_MIPS_TLS_DTPMOD32:
	case R_MIPS_TLS_TPREL32:
	  tls = info->curtls;
	  if (tls == NULL)
	    return 0;
	  /* A relocation against symbol 0.  A shared library cannot
	     know what the final module IDs or TP-relative offsets are,
	     so the executable must always have a conflict for them.  */
	  value = 0;
	  break;
	default:
	  return 0;
	}
    }
  else if (conflict->ifunc)
    {
      error (0, 0, "%s: STT_GNU_IFUNC not handled on MIPS yet",
	     dso->filename);
      return 1;
    }
  else
    {
      /* DTPREL32 relocations just involve the symbol value; no other
	 TLS information is needed.  Ignore conflicts created from a
	 lookup of type RTYPE_CLASS_TLS if no real conflict exists.  */
      if (GELF_R_TYPE (r_info) == R_MIPS_TLS_DTPREL32
	  && conflict->lookup.tls == conflict->conflict.tls
	  && conflict->lookupval == conflict->conflictval)
	return 0;

      value = conflict_lookup_value (conflict);
    }
  /* VALUE now contains the final symbol value.  Change it to the
     value we want to store at R_OFFSET.  */
  switch (GELF_R_TYPE (r_info))
    {
    case R_MIPS_REL32:
      value += mips_read_addend (dso, r_offset, rela);
      break;

    case R_MIPS_GLOB_DAT:
      break;

    case R_MIPS_TLS_DTPMOD32:
      if (conflict != NULL && mips_get_tls (dso, conflict, &tls) == 1)
	return 1;
      value = tls->modid;
      break;

    case R_MIPS_TLS_DTPREL32:
      value += mips_read_addend (dso, r_offset, rela) - TLS_DTV_OFFSET;
      break;

    case R_MIPS_TLS_TPREL32:
      if (conflict != NULL && mips_get_tls (dso, conflict, &tls) == 1)
	return 1;
      value += (mips_read_addend (dso, r_offset, rela)
		+ tls->offset - TLS_TP_OFFSET);
      break;

    default:
      error (0, 0, "%s: Unknown MIPS relocation type %d", dso->filename,
	     (int) GELF_R_TYPE (r_info));
      return 1;
    }
  /* Create and initialize a conflict entry.  */
  entry = prelink_conflict_add_rela (info);
  if (entry == NULL)
    return 1;
  entry->r_addend = (int32_t) value;
  entry->r_offset = r_offset;
  entry->r_info = GELF_R_INFO (0, R_MIPS_REL32);
  return 0;
}
