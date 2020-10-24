sparc64_undo_prelink_rela (DSO *dso, GElf_Rela *rela, GElf_Addr relaaddr)
{
  int sec;

  switch (GELF_R_TYPE (rela->r_info))
    {
    case R_SPARC_NONE:
      break;
    case R_SPARC_JMP_SLOT:
      sec = addr_to_sec (dso, rela->r_offset);
      if (sec != -1)
	{
	  if (rela->r_addend == 0)
	    {
	      /* sethi .-.plt, %g1
		 b,a %xcc, .plt+0x20  */
	      write_be32 (dso, rela->r_offset,
			  0x03000000
			  | ((rela->r_offset - dso->shdr[sec].sh_addr)
			     & 0x3fffff));
	      write_be32 (dso, rela->r_offset + 4,
			  0x30680000
			  | (((dso->shdr[sec].sh_addr + 32
			       - rela->r_offset - 4) >> 2)
			     & 0x7ffff));
	      write_be32 (dso, rela->r_offset + 8, 0x01000000);
	      write_be32 (dso, rela->r_offset + 12, 0x01000000);
	      write_be32 (dso, rela->r_offset + 16, 0x01000000);
	      write_be32 (dso, rela->r_offset + 20, 0x01000000);
	      write_be32 (dso, rela->r_offset + 24, 0x01000000);
	      write_be32 (dso, rela->r_offset + 28, 0x01000000);
	    }
	  else
	    {
	      GElf_Addr slot = ((rela->r_offset + 0x400
				 - dso->shdr[sec].sh_addr)
				/ 0x1400) * 0x1400
			       + dso->shdr[sec].sh_addr - 0x400;
	      /* slot+12 contains: ldx [%o7 + X], %g1  */
	      GElf_Addr ptr = slot + (read_ube32 (dso, slot + 12) & 0xfff) + 4;

	      write_be64 (dso, rela->r_offset,
			  dso->shdr[sec].sh_addr
			  - (slot + ((rela->r_offset - ptr) / 8) * 24 + 4));
	    }
	}
      break;
    case R_SPARC_RELATIVE:
    case R_SPARC_GLOB_DAT:
    case R_SPARC_64:
    case R_SPARC_UA64:
    case R_SPARC_DISP64:
    case R_SPARC_TLS_DTPMOD64:
    case R_SPARC_TLS_DTPOFF64:
    case R_SPARC_TLS_TPOFF64:
      write_be64 (dso, rela->r_offset, 0);
      break;
    case R_SPARC_32:
    case R_SPARC_UA32:
    case R_SPARC_DISP32:
      write_be32 (dso, rela->r_offset, 0);
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
    case R_SPARC_LM22:
    case R_SPARC_HI22:
      write_be32 (dso, rela->r_offset,
		  read_ube32 (dso, rela->r_offset) & 0xffc00000);
      break;
    case R_SPARC_WDISP30:
      write_be32 (dso, rela->r_offset,
		  read_ube32 (dso, rela->r_offset) & 0xc0000000);
      break;
    case R_SPARC_H44:
      write_be32 (dso, rela->r_offset,
		  read_ube32 (dso, rela->r_offset) & 0xffc00000);
      break;
    case R_SPARC_M44:
      write_be32 (dso, rela->r_offset,
		  read_ube32 (dso, rela->r_offset) & ~0x3ff);
      break;
    case R_SPARC_L44:
      write_be32 (dso, rela->r_offset,
		  read_ube32 (dso, rela->r_offset) & ~0xfff);
      break;
    case R_SPARC_HH22:
      write_be32 (dso, rela->r_offset,
		  read_ube32 (dso, rela->r_offset) & 0xffc00000);
      break;
    case R_SPARC_HM10:
      write_be32 (dso, rela->r_offset,
		  read_ube32 (dso, rela->r_offset) & ~0x3ff);
      break;
    case R_SPARC_OLO10:
      write_be32 (dso, rela->r_offset,
		  read_ube32 (dso, rela->r_offset) & ~0x1fff);
      break;
    case R_SPARC_TLS_LE_LOX10:
      write_be32 (dso, rela->r_offset,
		  read_ube32 (dso, rela->r_offset) & 0xffffe000);
      break;
    case R_SPARC_TLS_LE_HIX22:
      write_be32 (dso, rela->r_offset,
		  read_ube32 (dso, rela->r_offset) & 0xffc00000);
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
