s390_prelink_rela (struct prelink_info *info, GElf_Rela *rela,
		   GElf_Addr relaaddr)
{
  DSO *dso = info->dso;
  GElf_Addr value;

  if (GELF_R_TYPE (rela->r_info) == R_390_NONE
      || GELF_R_TYPE (rela->r_info) == R_390_IRELATIVE)
    return 0;
  else if (GELF_R_TYPE (rela->r_info) == R_390_RELATIVE)
    {
      write_be32 (dso, rela->r_offset, rela->r_addend);
      return 0;
    }
  value = info->resolve (info, GELF_R_SYM (rela->r_info),
			 GELF_R_TYPE (rela->r_info));
  value += rela->r_addend;
  switch (GELF_R_TYPE (rela->r_info))
    {
    case R_390_GLOB_DAT:
    case R_390_JMP_SLOT:
      write_be32 (dso, rela->r_offset, value - rela->r_addend);
      break;
    case R_390_32:
      write_be32 (dso, rela->r_offset, value);
      break;
    case R_390_PC32:
      write_be32 (dso, rela->r_offset, value - rela->r_offset);
      break;
    case R_390_PC32DBL:
    case R_390_PLT32DBL:
      write_be32 (dso, rela->r_offset,
		  ((Elf32_Sword) (value - rela->r_offset)) >> 1);
      break;
    case R_390_16:
      write_be16 (dso, rela->r_offset, value);
      break;
    case R_390_PC16:
      write_be16 (dso, rela->r_offset, value - rela->r_offset);
      break;
    case R_390_PC16DBL:
    case R_390_PLT16DBL:
      write_be16 (dso, rela->r_offset,
		  ((int16_t) (value - rela->r_offset)) >> 1);
      break;
    case R_390_8:
      write_8 (dso, rela->r_offset, value);
      break;
    case R_390_TLS_DTPOFF:
      write_be32 (dso, rela->r_offset, value);
      break;
    /* DTPMOD and TPOFF is impossible to predict in shared libraries
       unless prelink sets the rules.  */
    case R_390_TLS_DTPMOD:
      if (dso->ehdr.e_type == ET_EXEC)
	{
	  error (0, 0, "%s: R_390_TLS_DTPMOD reloc in executable?",
		 dso->filename);
	  return 1;
	}
      break;
    case R_390_TLS_TPOFF:
      if (dso->ehdr.e_type == ET_EXEC && info->resolvetls)
	write_be32 (dso, rela->r_offset,
		    value - info->resolvetls->offset);
      break;
    case R_390_COPY:
      if (dso->ehdr.e_type == ET_EXEC)
	/* COPY relocs are handled specially in generic code.  */
	return 0;
      error (0, 0, "%s: R_390_COPY reloc in shared library?", dso->filename);
      return 1;
    default:
      error (0, 0, "%s: Unknown S390 relocation type %d", dso->filename,
	     (int) GELF_R_TYPE (rela->r_info));
      return 1;
    }
  return 0;
}
