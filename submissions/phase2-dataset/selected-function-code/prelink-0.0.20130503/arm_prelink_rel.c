arm_prelink_rel (struct prelink_info *info, GElf_Rel *rel, GElf_Addr reladdr)
{
  DSO *dso;
  GElf_Addr value;

  if (GELF_R_TYPE (rel->r_info) == R_ARM_RELATIVE
      || GELF_R_TYPE (rel->r_info) == R_ARM_NONE)
    /* Fast path: nothing to do.  */
    return 0;
  dso = info->dso;
  value = info->resolve (info, GELF_R_SYM (rel->r_info),
			 GELF_R_TYPE (rel->r_info));
  switch (GELF_R_TYPE (rel->r_info))
    {
    case R_ARM_GLOB_DAT:
    case R_ARM_JUMP_SLOT:
      write_ne32 (dso, rel->r_offset, value);
      break;
    case R_ARM_ABS32:
      {
	if (read_une32 (dso, rel->r_offset))
	  {
	    error (0, 0, "%s: R_ARM_ABS32 relocs with non-zero addend should not be present in prelinked REL sections",
		   dso->filename);
	    return 1;
	  }
	rel->r_info = GELF_R_INFO (GELF_R_SYM (rel->r_info), R_ARM_GLOB_DAT);
	write_ne32 (dso, rel->r_offset, value);
	/* Tell prelink_rel routine *rel has changed.  */
	return 2;
      }
    case R_ARM_PC24:
      error (0, 0, "%s: R_ARM_PC24 relocs with non-zero addend should not be present in prelinked REL sections",
	     dso->filename);
      return 1;
    case R_ARM_COPY:
      if (dso->ehdr.e_type == ET_EXEC)
	/* COPY relocs are handled specially in generic code.  */
	return 0;
      error (0, 0, "%s: R_ARM_COPY reloc in shared library?", dso->filename);
      return 1;
    case R_ARM_TLS_DTPOFF32:
      write_ne32 (dso, rel->r_offset, value);
      break;
    /* DTPMOD32 and TPOFF32 is impossible to predict in shared libraries
       unless prelink sets the rules.  */
    case R_ARM_TLS_DTPMOD32:
      if (dso->ehdr.e_type == ET_EXEC)
        {
          error (0, 0, "%s: R_ARM_TLS_DTPMOD32 reloc in executable?",
                 dso->filename);
          return 1;
        }
      break;
    case R_ARM_TLS_TPOFF32:
      if (dso->ehdr.e_type == ET_EXEC)
	error (0, 0, "%s: R_ARM_TLS_TPOFF32 relocs should not be present in "
	       "prelinked ET_EXEC REL sections",
	       dso->filename);
      break;
    default:
      error (0, 0, "%s: Unknown arm relocation type %d", dso->filename,
	     (int) GELF_R_TYPE (rel->r_info));
      return 1;
    }
  return 0;
}
