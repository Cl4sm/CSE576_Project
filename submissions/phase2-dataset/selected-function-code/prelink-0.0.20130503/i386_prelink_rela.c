static int
i386_prelink_rela (struct prelink_info *info, GElf_Rela *rela,
		   GElf_Addr relaaddr)
{
  DSO *dso;
  GElf_Addr value;

  if (GELF_R_TYPE (rela->r_info) == R_386_RELATIVE
      || GELF_R_TYPE (rela->r_info) == R_386_IRELATIVE
      || GELF_R_TYPE (rela->r_info) == R_386_NONE)
    /* Fast path: nothing to do.  */
    return 0;
  dso = info->dso;
  value = info->resolve (info, GELF_R_SYM (rela->r_info),
			 GELF_R_TYPE (rela->r_info));
  switch (GELF_R_TYPE (rela->r_info))
    {
    case R_386_GLOB_DAT:
    case R_386_JMP_SLOT:
      write_le32 (dso, rela->r_offset, value + rela->r_addend);
      break;
    case R_386_32:
      write_le32 (dso, rela->r_offset, value + rela->r_addend);
      break;
    case R_386_PC32:
      write_le32 (dso, rela->r_offset, value + rela->r_addend - rela->r_offset);
      break;
    case R_386_TLS_DTPOFF32:
      write_le32 (dso, rela->r_offset, value + rela->r_addend);
      break;
    /* DTPMOD32 and TPOFF{32,} is impossible to predict unless prelink
       sets the rules.  */
    case R_386_TLS_DTPMOD32:
      if (dso->ehdr.e_type == ET_EXEC)
	{
	  error (0, 0, "%s: R_386_TLS_DTPMOD32 reloc in executable?",
		 dso->filename);
	  return 1;
	}
      break;
    case R_386_TLS_TPOFF32:
      if (dso->ehdr.e_type == ET_EXEC && info->resolvetls)
	write_le32 (dso, rela->r_offset,
		    -(value + rela->r_addend - info->resolvetls->offset));
      break;
    case R_386_TLS_TPOFF:
      if (dso->ehdr.e_type == ET_EXEC && info->resolvetls)
	write_le32 (dso, rela->r_offset,
		    value + rela->r_addend - info->resolvetls->offset);
      break;
    case R_386_COPY:
      if (dso->ehdr.e_type == ET_EXEC)
	/* COPY relocs are handled specially in generic code.  */
	return 0;
      error (0, 0, "%s: R_386_COPY reloc in shared library?", dso->filename);
      return 1;
    default:
      error (0, 0, "%s: Unknown i386 relocation type %d", dso->filename,
	     (int) GELF_R_TYPE (rela->r_info));
      return 1;
    }
  return 0;
}
