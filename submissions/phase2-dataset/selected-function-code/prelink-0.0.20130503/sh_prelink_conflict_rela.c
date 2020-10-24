static int
sh_prelink_conflict_rela (DSO *dso, struct prelink_info *info,
			  GElf_Rela *rela, GElf_Addr relaaddr)
{
  GElf_Addr value;
  struct prelink_conflict *conflict;
  GElf_Rela *ret;

  if (GELF_R_TYPE (rela->r_info) == R_SH_RELATIVE
      || GELF_R_TYPE (rela->r_info) == R_SH_NONE
      || info->dso == dso)
    /* Fast path: nothing to do.  */
    return 0;
  conflict = prelink_conflict (info, GELF_R_SYM (rela->r_info),
			       GELF_R_TYPE (rela->r_info));
  if (conflict == NULL)
    return 0;
  else if (conflict->ifunc)
    {
      error (0, 0, "%s: STT_GNU_IFUNC not handled on SuperH yet",
	     dso->filename);
      return 1;
    }
  value = conflict_lookup_value (conflict);
  ret = prelink_conflict_add_rela (info);
  if (ret == NULL)
    return 1;
  ret->r_offset = rela->r_offset;
  ret->r_info = GELF_R_INFO (0, GELF_R_TYPE (rela->r_info));
  value += rela->r_addend;
  switch (GELF_R_TYPE (rela->r_info))
    {
    case R_SH_REL32:
      value -= rela->r_offset;
      ret->r_info = GELF_R_INFO (0, R_SH_DIR32);
      /* FALLTHROUGH */
    case R_SH_DIR32:
      if ((rela->r_offset & 3) == 0)
	ret->r_info = GELF_R_INFO (0, R_SH_GLOB_DAT);
      /* FALLTHROUGH */
    case R_SH_GLOB_DAT:
    case R_SH_JMP_SLOT:
      ret->r_addend = (Elf32_Sword) (value + rela->r_addend);
      break;
    case R_SH_COPY:
      error (0, 0, "R_SH_COPY should not be present in shared libraries");
      return 1;
    default:
      error (0, 0, "%s: Unknown sh relocation type %d", dso->filename,
	     (int) GELF_R_TYPE (rela->r_info));
      return 1;
    }
  return 0;
}
