i386_prelink_rel (struct prelink_info *info, GElf_Rel *rel, GElf_Addr reladdr)
{
  DSO *dso;
  GElf_Addr value;

  if (GELF_R_TYPE (rel->r_info) == R_386_RELATIVE
      || GELF_R_TYPE (rel->r_info) == R_386_IRELATIVE
      || GELF_R_TYPE (rel->r_info) == R_386_NONE)
    /* Fast path: nothing to do.  */
    return 0;
  dso = info->dso;
  value = info->resolve (info, GELF_R_SYM (rel->r_info),
			 GELF_R_TYPE (rel->r_info));
  switch (GELF_R_TYPE (rel->r_info))
    {
    case R_386_GLOB_DAT:
    case R_386_JMP_SLOT:
      write_le32 (dso, rel->r_offset, value);
      break;
    case R_386_32:
      {
	if (read_ule32 (dso, rel->r_offset))
	  {
	    error (0, 0, "%s: R_386_32 relocs with non-zero addend should not be present in prelinked REL sections",
		   dso->filename);
	    return 1;
	  }
	rel->r_info = GELF_R_INFO (GELF_R_SYM (rel->r_info), R_386_GLOB_DAT);
	write_le32 (dso, rel->r_offset, value);
	/* Tell prelink_rel routine *rel has changed.  */
	return 2;
      }
    case R_386_PC32:
      error (0, 0, "%s: R_386_PC32 relocs should not be present in prelinked REL sections",
	     dso->filename);
      return 1;
    case R_386_TLS_DTPOFF32:
      write_le32 (dso, rel->r_offset, value);
      break;
    /* DTPMOD32 and TPOFF{32,} is impossible to predict unless prelink
       sets the rules.  Also for TPOFF{32,} there is REL->RELA problem.  */
    case R_386_TLS_DTPMOD32:
      if (dso->ehdr.e_type == ET_EXEC)
	{
	  error (0, 0, "%s: R_386_TLS_DTPMOD32 reloc in executable?",
		 dso->filename);
	  return 1;
	}
      break;
    case R_386_TLS_TPOFF32:
    case R_386_TLS_TPOFF:
      if (dso->ehdr.e_type == ET_EXEC)
	error (0, 0, "%s: R_386_TLS_TPOFF relocs should not be present in prelinked ET_EXEC REL sections",
	       dso->filename);
      break;
    case R_386_COPY:
      if (dso->ehdr.e_type == ET_EXEC)
	/* COPY relocs are handled specially in generic code.  */
	return 0;
      error (0, 0, "%s: R_386_COPY reloc in shared library?", dso->filename);
      return 1;
    default:
      error (0, 0, "%s: Unknown i386 relocation type %d", dso->filename,
	     (int) GELF_R_TYPE (rel->r_info));
      return 1;
    }
  return 0;
}
