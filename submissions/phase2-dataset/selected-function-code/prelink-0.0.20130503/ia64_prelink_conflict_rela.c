ia64_prelink_conflict_rela (DSO *dso, struct prelink_info *info,
			    GElf_Rela *rela, GElf_Addr relaaddr)
{
  GElf_Addr value;
  struct prelink_conflict *conflict;
  GElf_Rela *ret;

  if ((GELF_R_TYPE (rela->r_info) & ~3) == R_IA64_REL32MSB
      || GELF_R_TYPE (rela->r_info) == R_IA64_NONE
      || info->dso == dso)
    /* Fast path: nothing to do.  */
    return 0;
  conflict = prelink_conflict (info, GELF_R_SYM (rela->r_info),
			       GELF_R_TYPE (rela->r_info));
  if (conflict == NULL)
    return 0;
  else if (conflict->ifunc)
    {
      error (0, 0, "%s: STT_GNU_IFUNC not handled on IA-64 yet",
	     dso->filename);
      return 1;
    }
  value = conflict_lookup_value (conflict);
  ret = prelink_conflict_add_rela (info);
  if (ret == NULL)
    return 1;
  ret->r_offset = rela->r_offset;
  ret->r_info = GELF_R_INFO (0, GELF_R_TYPE (rela->r_info));
  if ((GELF_R_TYPE (rela->r_info) & ~3) == R_IA64_DIR32MSB
      || (GELF_R_TYPE (rela->r_info) & ~1) == R_IA64_IPLTMSB)
    {
      ret->r_addend = value + rela->r_addend;
      return 0;
    }
  else if ((GELF_R_TYPE (rela->r_info) & ~3) == R_IA64_PCREL32MSB)
    {
      ret->r_addend = value + rela->r_addend - (rela->r_offset & -16);
      ret->r_info = GELF_R_INFO (0, GELF_R_TYPE (rela->r_info)
				    + R_IA64_DIR32MSB - R_IA64_PCREL32MSB);
      return 0;
    }
  else if (GELF_R_TYPE (rela->r_info) == R_IA64_COPY)
    {
      error (0, 0, "R_IA64_COPY should not be present in shared libraries");
      return 1;
    }
  error (0, 0, "%s: Unknown ia64 relocation type %d", dso->filename,
	 (int) GELF_R_TYPE (rela->r_info));
  return 1;
}
