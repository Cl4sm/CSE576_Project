arm_prelink_conflict_rela (DSO *dso, struct prelink_info *info,
			   GElf_Rela *rela, GElf_Addr relaaddr)
{
  GElf_Addr value;
  struct prelink_conflict *conflict;
  struct prelink_tls *tls;
  GElf_Rela *ret;
  Elf32_Sword val;

  if (GELF_R_TYPE (rela->r_info) == R_ARM_RELATIVE
      || GELF_R_TYPE (rela->r_info) == R_ARM_NONE
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
      if (GELF_R_TYPE (rela->r_info) == R_ARM_TLS_DTPOFF32
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
    case R_ARM_GLOB_DAT:
    case R_ARM_JUMP_SLOT:
    case R_ARM_ABS32:
      ret->r_addend = (Elf32_Sword) (value + rela->r_addend);
      break;
    case R_ARM_PC24:
      val = value + rela->r_addend - rela->r_offset;
      val >>= 2;
      if ((Elf32_Word) val + 0x800000 >= 0x1000000)
	{
	  error (0, 0, "%s: R_ARM_PC24 overflow", dso->filename);
	  return 1;
	}
      value = read_une32 (dso, rela->r_offset) & 0xff000000;
      ret->r_addend = (Elf32_Sword) (value | (val & 0xffffff));
      ret->r_info = GELF_R_INFO (0, R_ARM_ABS32);
      break;
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
      switch (GELF_R_TYPE (rela->r_info))
	{
	case R_ARM_TLS_DTPMOD32:
	  ret->r_addend = tls->modid;
	  break;
	case R_ARM_TLS_DTPOFF32:
	  ret->r_addend = value;
	  break;
	case R_ARM_TLS_TPOFF32:
	  ret->r_addend = value + rela->r_addend + tls->offset;
	  break;
	}
      break;
    default:
      error (0, 0, "%s: Unknown arm relocation type %d", dso->filename,
	     (int) GELF_R_TYPE (rela->r_info));
      return 1;
    }
  return 0;
}
