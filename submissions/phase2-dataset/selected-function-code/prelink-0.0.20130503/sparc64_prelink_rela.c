sparc64_prelink_rela (struct prelink_info *info, GElf_Rela *rela,
		      GElf_Addr relaaddr)
{
  DSO *dso = info->dso;
  GElf_Addr value;

  if (SPARC64_R_TYPE (rela->r_info) == R_SPARC_NONE)
    return 0;
  else if (SPARC64_R_TYPE (rela->r_info) == R_SPARC_RELATIVE)
    {
      /* 64-bit SPARC handles RELATIVE relocs as
	 *(long *)rela->r_offset = l_addr + rela->r_addend,
	 so we must update the memory.  */
      write_be64 (dso, rela->r_offset, rela->r_addend);
      return 0;
    }
  value = info->resolve (info, GELF_R_SYM (rela->r_info),
			 SPARC64_R_TYPE (rela->r_info));
  value += rela->r_addend;
  switch (SPARC64_R_TYPE (rela->r_info))
    {
    case R_SPARC_GLOB_DAT:
    case R_SPARC_64:
    case R_SPARC_UA64:
      write_be64 (dso, rela->r_offset, value);
      break;
    case R_SPARC_32:
    case R_SPARC_UA32:
      write_be32 (dso, rela->r_offset, value);
      break;
    case R_SPARC_JMP_SLOT:
      sparc64_fixup_plt (dso, rela, value);
      break;
    case R_SPARC_8:
      write_8 (dso, rela->r_offset, value);
      break;
    case R_SPARC_16:
    case R_SPARC_UA16:
      write_be16 (dso, rela->r_offset, value);
      break;
    case R_SPARC_LO10:
      write_be32 (dso, rela->r_offset,
		  (value & 0x3ff) | (read_ube32 (dso, rela->r_offset) & ~0x3ff));
      break;
    case R_SPARC_LM22:
    case R_SPARC_HI22:
      write_be32 (dso, rela->r_offset,
		  ((value >> 10) & 0x3fffff)
		  | (read_ube32 (dso, rela->r_offset) & 0xffc00000));
      break;
    case R_SPARC_DISP8:
      write_8 (dso, rela->r_offset, value - rela->r_offset);
      break;
    case R_SPARC_DISP16:
      write_be16 (dso, rela->r_offset, value - rela->r_offset);
      break;
    case R_SPARC_DISP32:
      write_be32 (dso, rela->r_offset, value - rela->r_offset);
      break;
    case R_SPARC_DISP64:
      write_be64 (dso, rela->r_offset, value - rela->r_offset);
      break;
    case R_SPARC_WDISP30:
      write_be32 (dso, rela->r_offset,
		  (((value - rela->r_offset) >> 2) & 0x3fffffff)
		  | (read_ube32 (dso, rela->r_offset) & 0xc0000000));
      break;
    case R_SPARC_TLS_DTPOFF64:
      write_be64 (dso, rela->r_offset, value + rela->r_addend);
      break;
    /* DTPMOD64 and TPOFF64 is impossible to predict in shared libraries
       unless prelink sets the rules.  */
    case R_SPARC_TLS_DTPMOD64:
      if (dso->ehdr.e_type == ET_EXEC)
	{
	  error (0, 0, "%s: R_SPARC_TLS_DTPMOD64 reloc in executable?",
		 dso->filename);
	  return 1;
	}
      break;
    case R_SPARC_TLS_TPOFF64:
      if (dso->ehdr.e_type == ET_EXEC && info->resolvetls)
	write_be64 (dso, rela->r_offset,
		    value + rela->r_addend - info->resolvetls->offset);
      break;
    case R_SPARC_TLS_LE_HIX22:
      if (dso->ehdr.e_type == ET_EXEC && info->resolvetls)
	write_be32 (dso, rela->r_offset,
		    (read_ube32 (dso, rela->r_offset) & 0xffc00000)
		    | (((~(value + rela->r_addend - info->resolvetls->offset))
			>> 10) & 0x3fffff));
      break;
    case R_SPARC_TLS_LE_LOX10:
      if (dso->ehdr.e_type == ET_EXEC && info->resolvetls)
	write_be32 (dso, rela->r_offset,
		    (read_ube32 (dso, rela->r_offset) & 0xffffe000) | 0x1c00
		    | ((value + rela->r_addend - info->resolvetls->offset)
		       & 0x3ff));
      break;
    case R_SPARC_H44:
      write_be32 (dso, rela->r_offset,
		  ((value >> 22) & 0x3fffff)
		  | (read_ube32 (dso, rela->r_offset) & 0xffc00000));
      break;
    case R_SPARC_M44:
      write_be32 (dso, rela->r_offset,
		  ((value >> 12) & 0x3ff)
		  | (read_ube32 (dso, rela->r_offset) & ~0x3ff));
      break;
    case R_SPARC_L44:
      write_be32 (dso, rela->r_offset,
		  (value & 0xfff) | (read_ube32 (dso, rela->r_offset) & ~0xfff));
      break;
    case R_SPARC_HH22:
      write_be32 (dso, rela->r_offset,
		  ((value >> 42) & 0x3fffff)
		  | (read_ube32 (dso, rela->r_offset) & 0xffc00000));
      break;
    case R_SPARC_HM10:
      write_be32 (dso, rela->r_offset,
		  ((value >> 32) & 0x3ff)
		  | (read_ube32 (dso, rela->r_offset) & ~0x3ff));
      break;
    case R_SPARC_OLO10:
      write_be32 (dso, rela->r_offset,
		  (((value & 0x3ff) + (GELF_R_TYPE (rela->r_info) >> 8)) & 0x1fff)
		  | (read_ube32 (dso, rela->r_offset) & ~0x1fff));
      break;
    case R_SPARC_COPY:
      if (dso->ehdr.e_type == ET_EXEC)
	/* COPY relocs are handled specially in generic code.  */
	return 0;
      error (0, 0, "%s: R_SPARC_COPY reloc in shared library?", dso->filename);
      return 1;
    default:
      error (0, 0, "%s: Unknown sparc relocation type %d", dso->filename,
	     (int) SPARC64_R_TYPE (rela->r_info));
      return 1;
    }
  return 0;
}
