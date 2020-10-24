sparc_prelink_rela (struct prelink_info *info, GElf_Rela *rela,
		    GElf_Addr relaaddr)
{
  DSO *dso = info->dso;
  GElf_Addr value;

  if (GELF_R_TYPE (rela->r_info) == R_SPARC_NONE)
    return 0;
  else if (GELF_R_TYPE (rela->r_info) == R_SPARC_RELATIVE)
    {
      /* 32-bit SPARC handles RELATIVE relocs as
	 *(int *)rela->r_offset += l_addr + rela->r_addend.
	 RELATIVE relocs against .got traditionally used to have the
	 addend in memory pointed by r_offset and 0 r_addend,
	 other RELATIVE relocs and more recent .got RELATIVE relocs
	 too have 0 in memory and non-zero r_addend.  For prelinking,
	 we need the value in memory to be already relocated for
	 l_addr == 0 case, so we have to make sure r_addend will be 0.  */
      if (rela->r_addend == 0)
	return 0;
      value = read_ube32 (dso, rela->r_offset);
      value += rela->r_addend;
      rela->r_addend = 0;
      write_be32 (dso, rela->r_offset, value);
      /* Tell prelink_rela routine it should update the relocation.  */
      return 2;
    }
  value = info->resolve (info, GELF_R_SYM (rela->r_info),
			 GELF_R_TYPE (rela->r_info));
  value += rela->r_addend;
  switch (GELF_R_TYPE (rela->r_info))
    {
    case R_SPARC_GLOB_DAT:
    case R_SPARC_32:
    case R_SPARC_UA32:
      write_be32 (dso, rela->r_offset, value);
      break;
    case R_SPARC_JMP_SLOT:
      sparc_fixup_plt (dso, rela, value);
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
    case R_SPARC_WDISP30:
      write_be32 (dso, rela->r_offset,
		  (((value - rela->r_offset) >> 2) & 0x3fffffff)
		  | (read_ube32 (dso, rela->r_offset) & 0xc0000000));
      break;
    case R_SPARC_TLS_DTPOFF32:
      write_be32 (dso, rela->r_offset, value + rela->r_addend);
      break;
    /* DTPMOD32 and TPOFF32 is impossible to predict in shared libraries
       unless prelink sets the rules.  */
    case R_SPARC_TLS_DTPMOD32:
      if (dso->ehdr.e_type == ET_EXEC)
	{
	  error (0, 0, "%s: R_SPARC_TLS_DTPMOD32 reloc in executable?",
		 dso->filename);
	  return 1;
	}
      break;
    case R_SPARC_TLS_TPOFF32:
      if (dso->ehdr.e_type == ET_EXEC && info->resolvetls)
	write_be32 (dso, rela->r_offset,
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
    case R_SPARC_COPY:
      if (dso->ehdr.e_type == ET_EXEC)
	/* COPY relocs are handled specially in generic code.  */
	return 0;
      error (0, 0, "%s: R_SPARC_COPY reloc in shared library?", dso->filename);
      return 1;
    default:
      error (0, 0, "%s: Unknown sparc relocation type %d", dso->filename,
	     (int) GELF_R_TYPE (rela->r_info));
      return 1;
    }
  return 0;
}
