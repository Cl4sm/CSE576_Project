static int
x86_64_prelink_rela (struct prelink_info *info, GElf_Rela *rela,
		     GElf_Addr relaaddr)
{
  DSO *dso;
  GElf_Addr value;

  dso = info->dso;
  if (GELF_R_TYPE (rela->r_info) == R_X86_64_NONE
      || GELF_R_TYPE (rela->r_info) == R_X86_64_IRELATIVE)
    return 0;
  else if (GELF_R_TYPE (rela->r_info) == R_X86_64_RELATIVE)
    {
      write_le64 (dso, rela->r_offset, rela->r_addend);
      return 0;
    }
  value = info->resolve (info, GELF_R_SYM (rela->r_info),
			 GELF_R_TYPE (rela->r_info));
  switch (GELF_R_TYPE (rela->r_info))
    {
    case R_X86_64_GLOB_DAT:
    case R_X86_64_JUMP_SLOT:
    case R_X86_64_64:
      write_le64 (dso, rela->r_offset, value + rela->r_addend);
      break;
    case R_X86_64_32:
      write_le32 (dso, rela->r_offset, value + rela->r_addend);
      break;
    case R_X86_64_PC32:
      write_le32 (dso, rela->r_offset, value + rela->r_addend - rela->r_offset);
      break;
    case R_X86_64_DTPOFF64:
      write_le64 (dso, rela->r_offset, value + rela->r_addend);
      break;
    /* DTPMOD64 and TPOFF64 is impossible to predict in shared libraries
       unless prelink sets the rules.  */
    case R_X86_64_DTPMOD64:
      if (dso->ehdr.e_type == ET_EXEC)
	{
	  error (0, 0, "%s: R_X86_64_DTPMOD64 reloc in executable?",
		 dso->filename);
	  return 1;
	}
      break;
    case R_X86_64_TPOFF64:
      if (dso->ehdr.e_type == ET_EXEC && info->resolvetls)
	write_le64 (dso, rela->r_offset,
		    value + rela->r_addend - info->resolvetls->offset);
      break;
    case R_X86_64_COPY:
      if (dso->ehdr.e_type == ET_EXEC)
	/* COPY relocs are handled specially in generic code.  */
	return 0;
      error (0, 0, "%s: R_X86_64_COPY reloc in shared library?", dso->filename);
      return 1;
    default:
      error (0, 0, "%s: Unknown X86-64 relocation type %d", dso->filename,
	     (int) GELF_R_TYPE (rela->r_info));
      return 1;
    }
  return 0;
}
