ppc_prelink_conflict_rela (DSO *dso, struct prelink_info *info,
			   GElf_Rela *rela, GElf_Addr relaaddr)
{
  GElf_Addr value;
  struct prelink_conflict *conflict;
  struct prelink_tls *tls;
  GElf_Rela *ret;
  int r_type;

  if (GELF_R_TYPE (rela->r_info) == R_PPC_RELATIVE
      || GELF_R_TYPE (rela->r_info) == R_PPC_NONE)
    /* Fast path: nothing to do.  */
    return 0;
  conflict = prelink_conflict (info, GELF_R_SYM (rela->r_info),
			       GELF_R_TYPE (rela->r_info));
  if (conflict == NULL)
    {
      switch (GELF_R_TYPE (rela->r_info))
	{
	/* Even local DTPMOD and TPREL relocs need conflicts.  */
	case R_PPC_DTPMOD32:
	case R_PPC_TPREL32:
	case R_PPC_TPREL16:
	case R_PPC_TPREL16_LO:
	case R_PPC_TPREL16_HI:
	case R_PPC_TPREL16_HA:
	  if (info->curtls == NULL || info->dso == dso)
	    return 0;
	  break;
	/* Similarly IRELATIVE relocations always need conflicts.  */
	case R_PPC_IRELATIVE:
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
      /* DTPREL wants to see only real conflicts, not lookups
	 with reloc_class RTYPE_CLASS_TLS.  */
      if (conflict->lookup.tls == conflict->conflict.tls
	  && conflict->lookupval == conflict->conflictval)
	switch (GELF_R_TYPE (rela->r_info))
	  {
	  case R_PPC_DTPREL32:
	  case R_PPC_DTPREL16:
	  case R_PPC_DTPREL16_LO:
	  case R_PPC_DTPREL16_HI:
	  case R_PPC_DTPREL16_HA:
	    return 0;
	  }

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
    case R_PPC_GLOB_DAT:
      r_type = R_PPC_ADDR32;
    case R_PPC_ADDR32:
    case R_PPC_UADDR32:
    case R_PPC_IRELATIVE:
      if (conflict != NULL && conflict->ifunc)
	r_type = R_PPC_IRELATIVE;
      break;
    case R_PPC_JMP_SLOT:
      if (dynamic_info_is_set (dso, DT_PPC_GOT_BIT))
	{
	  r_type = R_PPC_ADDR32;
	  if (conflict != NULL && conflict->ifunc)
	    r_type = R_PPC_IRELATIVE;
	}
      break;
    case R_PPC_ADDR16_HA:
      value += 0x8000;
      /* FALLTHROUGH  */
    case R_PPC_ADDR16_HI:
      value = value >> 16;
      /* FALLTHROUGH  */
    case R_PPC_ADDR16:
    case R_PPC_UADDR16:
    case R_PPC_ADDR16_LO:
      if (r_type != R_PPC_UADDR16)
	r_type = R_PPC_ADDR16;
      value = ((value & 0xffff) ^ 0x8000) - 0x8000;
      break;
    case R_PPC_ADDR24:
      r_type = R_PPC_ADDR32;
      value = (value & 0x03fffffc)
	      | (read_ube32 (dso, rela->r_offset) & 0xfc000003);
      break;
    case R_PPC_ADDR14:
      r_type = R_PPC_ADDR32;
      value = (value & 0xfffc)
	      | (read_ube32 (dso, rela->r_offset) & 0xffff0003);
      break;
    case R_PPC_ADDR14_BRTAKEN:
    case R_PPC_ADDR14_BRNTAKEN:
      r_type = R_PPC_ADDR32;
      value = (value & 0xfffc)
	      | (read_ube32 (dso, rela->r_offset) & 0xffdf0003)
	      | ((((r_type == R_PPC_ADDR14_BRTAKEN) << 21)
		  ^ (value >> 10)) & 0x00200000);
      break;
    case R_PPC_REL24:
      r_type = R_PPC_ADDR32;
      value = ((value - rela->r_offset) & 0x03fffffc)
	      | (read_ube32 (dso, rela->r_offset) & 0xfc000003);
      break;
    case R_PPC_REL32:
      r_type = R_PPC_ADDR32;
      value -= rela->r_offset;
      break;
    case R_PPC_DTPMOD32:
    case R_PPC_DTPREL32:
    case R_PPC_DTPREL16:
    case R_PPC_DTPREL16_LO:
    case R_PPC_DTPREL16_HI:
    case R_PPC_DTPREL16_HA:
    case R_PPC_TPREL32:
    case R_PPC_TPREL16:
    case R_PPC_TPREL16_LO:
    case R_PPC_TPREL16_HI:
    case R_PPC_TPREL16_HA:
      if (conflict != NULL
	  && (conflict->reloc_class != RTYPE_CLASS_TLS
	      || conflict->lookup.tls == NULL))
	{
	  error (0, 0, "%s: TLS reloc not resolving to STT_TLS symbol",
		 dso->filename);
	  return 1;
	}
      tls = conflict ? conflict->lookup.tls : info->curtls;
      r_type = R_PPC_ADDR16;
      switch (GELF_R_TYPE (rela->r_info))
	{
	case R_PPC_DTPMOD32:
	  r_type = R_PPC_ADDR32;
	  value = tls->modid;
	  break;
	case R_PPC_DTPREL32:
	  r_type = R_PPC_ADDR32;
	  value -= 0x8000;
	  break;
	case R_PPC_DTPREL16:
	case R_PPC_DTPREL16_LO:
	  value -= 0x8000;
	  break;
	case R_PPC_DTPREL16_HI:
	  value = (value - 0x8000) >> 16;
	  break;
	case R_PPC_DTPREL16_HA:
	  value >>= 16;
	  break;
	case R_PPC_TPREL32:
	  r_type = R_PPC_ADDR32;
	  value += tls->offset - 0x7000;
	  break;
	case R_PPC_TPREL16:
	case R_PPC_TPREL16_LO:
	  value += tls->offset - 0x7000;
	  break;
	case R_PPC_TPREL16_HI:
	  value = (value + tls->offset - 0x7000) >> 16;
	  break;
	case R_PPC_TPREL16_HA:
	  value = (value + tls->offset - 0x7000 + 0x8000) >> 16;
	  break;
	}
      if (r_type == R_PPC_ADDR16)
	value = ((value & 0xffff) ^ 0x8000) - 0x8000;
      break;
    default:
      error (0, 0, "%s: Unknown PowerPC relocation type %d", dso->filename,
	     r_type);
      return 1;
    }
  if (conflict != NULL && conflict->ifunc && r_type != R_PPC_IRELATIVE)
    {
      error (0, 0, "%s: relocation %d against IFUNC symbol", dso->filename,
	     (int) GELF_R_TYPE (rela->r_info));
      return 1;
    }
  ret->r_info = GELF_R_INFO (0, r_type);
  ret->r_addend = (Elf32_Sword) value;
  return 0;
}
