sh_prelink_rela (struct prelink_info *info, GElf_Rela *rela,
		 GElf_Addr relaaddr)
{
  DSO *dso;
  GElf_Addr value;

  dso = info->dso;
  if (GELF_R_TYPE (rela->r_info) == R_SH_NONE)
    /* Fast path: nothing to do.  */
    return 0;
  else if (GELF_R_TYPE (rela->r_info) == R_SH_RELATIVE)
    {
      if (rela->r_addend)
	write_ne32 (dso, rela->r_offset, rela->r_addend);
      return 0;
    }
  value = info->resolve (info, GELF_R_SYM (rela->r_info),
			 GELF_R_TYPE (rela->r_info));
  value += rela->r_addend;
  switch (GELF_R_TYPE (rela->r_info))
    {
    case R_SH_GLOB_DAT:
    case R_SH_JMP_SLOT:
    case R_SH_DIR32:
      write_ne32 (dso, rela->r_offset, value);
      break;
    case R_SH_REL32:
      write_ne32 (dso, rela->r_offset, value - rela->r_addend);
      break;
    case R_SH_COPY:
      if (dso->ehdr.e_type == ET_EXEC)
	/* COPY relocs are handled specially in generic code.  */
	return 0;
      error (0, 0, "%s: R_SH_COPY reloc in shared library?", dso->filename);
      return 1;
    default:
      error (0, 0, "%s: Unknown sh relocation type %d", dso->filename,
	     (int) GELF_R_TYPE (rela->r_info));
      return 1;
    }
  return 0;
}
