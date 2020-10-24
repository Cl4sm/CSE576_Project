sparc_undo_prelink_rela (DSO *dso, GElf_Rela *rela, GElf_Addr relaaddr)
{
  int sec;

  switch (GELF_R_TYPE (rela->r_info))
    {
    case R_SPARC_NONE:
      return 0;
    case R_SPARC_RELATIVE:
      /* 32-bit SPARC handles RELATIVE relocs as
	 *(int *)rela->r_offset += l_addr + rela->r_addend.
	 RELATIVE relocs against .got traditionally used to have the
	 addend in memory pointed by r_offset and 0 r_addend,
	 other RELATIVE relocs and more recent RELATIVE relocs have 0
	 in memory and non-zero r_addend.
	 Always store 0 to memory when doing undo.  */
      assert (rela->r_addend == 0);
      rela->r_addend = (Elf32_Sword) read_ube32 (dso, rela->r_offset);
      write_be32 (dso, rela->r_offset, 0);
      /* Tell undo_prelink_rela routine it should update the
	 relocation.  */
      return 2;
    case R_SPARC_GLOB_DAT:
    case R_SPARC_32:
    case R_SPARC_UA32:
    case R_SPARC_DISP32:
    case R_SPARC_TLS_DTPMOD32:
    case R_SPARC_TLS_DTPOFF32:
    case R_SPARC_TLS_TPOFF32:
      write_be32 (dso, rela->r_offset, 0);
      break;
    case R_SPARC_JMP_SLOT:
      sec = addr_to_sec (dso, rela->r_offset);
      if (sec != -1)
	{
	  /* sethi .-.plt, %g1
	     b,a .plt+0  */
	  write_be32 (dso, rela->r_offset,
		      0x03000000
		      | ((rela->r_offset - dso->shdr[sec].sh_addr)
			 & 0x3fffff));
	  write_be32 (dso, rela->r_offset + 4,
		      0x30800000
		      | (((dso->shdr[sec].sh_addr - rela->r_offset - 4) >> 2)
			 & 0x3fffff));
	}
      break;
    case R_SPARC_8:
    case R_SPARC_DISP8:
      write_8 (dso, rela->r_offset, 0);
      break;
    case R_SPARC_16:
    case R_SPARC_UA16:
    case R_SPARC_DISP16:
      write_be16 (dso, rela->r_offset, 0);
      break;
    case R_SPARC_LO10:
      write_be32 (dso, rela->r_offset,
		  read_ube32 (dso, rela->r_offset) & ~0x3ff);
      break;
    case R_SPARC_TLS_LE_LOX10:
      write_be32 (dso, rela->r_offset,
		  read_ube32 (dso, rela->r_offset) & 0xffffe000);
      break;
    case R_SPARC_HI22:
    case R_SPARC_TLS_LE_HIX22:
      write_be32 (dso, rela->r_offset,
		  read_ube32 (dso, rela->r_offset) & 0xffc00000);
      break;
    case R_SPARC_WDISP30:
      write_be32 (dso, rela->r_offset,
		  read_ube32 (dso, rela->r_offset) & 0xc0000000);
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
