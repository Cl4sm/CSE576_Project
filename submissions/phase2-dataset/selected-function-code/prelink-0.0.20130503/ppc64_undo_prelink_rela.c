ppc64_undo_prelink_rela (DSO *dso, GElf_Rela *rela, GElf_Addr relaaddr)
{
  switch (GELF_R_TYPE (rela->r_info))
    {
    case R_PPC64_NONE:
      return 0;
    case R_PPC64_JMP_SLOT:
      /* .plt section will become SHT_NOBITS.  */
      return 0;
    case R_PPC64_JMP_IREL:
      /* .iplt section will become SHT_NOBITS.  */
      return 0;
    case R_PPC64_RELATIVE:
    case R_PPC64_ADDR64:
    case R_PPC64_IRELATIVE:
      write_be64 (dso, rela->r_offset, rela->r_addend);
      break;
    case R_PPC64_GLOB_DAT:
    case R_PPC64_UADDR64:
    case R_PPC64_DTPREL64:
    case R_PPC64_TPREL64:
    case R_PPC64_DTPMOD64:
    case R_PPC64_REL64:
      write_be64 (dso, rela->r_offset, 0);
      break;
    case R_PPC64_ADDR32:
    case R_PPC64_UADDR32:
    case R_PPC64_REL32:
      write_be32 (dso, rela->r_offset, 0);
      break;
    case R_PPC64_ADDR16_HA:
    case R_PPC64_DTPREL16_HA:
    case R_PPC64_TPREL16_HA:
    case R_PPC64_ADDR16_HI:
    case R_PPC64_DTPREL16_HI:
    case R_PPC64_TPREL16_HI:
    case R_PPC64_ADDR16:
    case R_PPC64_UADDR16:
    case R_PPC64_ADDR16_LO:
    case R_PPC64_DTPREL16:
    case R_PPC64_TPREL16:
    case R_PPC64_DTPREL16_LO:
    case R_PPC64_TPREL16_LO:
    case R_PPC64_ADDR16_HIGHERA:
    case R_PPC64_ADDR16_HIGHER:
    case R_PPC64_ADDR16_HIGHESTA:
    case R_PPC64_ADDR16_HIGHEST:
    case R_PPC64_ADDR16_LO_DS:
    case R_PPC64_ADDR16_DS:
      write_be16 (dso, rela->r_offset, 0);
      break;
    case R_PPC64_ADDR24:
    case R_PPC64_REL24:
      write_be32 (dso, rela->r_offset,
		  read_ube32 (dso, rela->r_offset) & 0xfc000003);
      break;
    case R_PPC64_ADDR14:
      write_be32 (dso, rela->r_offset,
		  read_ube32 (dso, rela->r_offset) & 0xffff0003);
      break;
    case R_PPC64_ADDR14_BRTAKEN:
    case R_PPC64_ADDR14_BRNTAKEN:
      write_be32 (dso, rela->r_offset,
		  read_ube32 (dso, rela->r_offset) & 0xffdf0003);
      break;
    case R_PPC64_COPY:
      if (dso->ehdr.e_type == ET_EXEC)
	/* COPY relocs are handled specially in generic code.  */
	return 0;
      error (0, 0, "%s: R_PPC64_COPY reloc in shared library?", dso->filename);
      return 1;
    default:
      error (0, 0, "%s: Unknown ppc relocation type %d", dso->filename,
	     (int) GELF_R_TYPE (rela->r_info));
      return 1;
    }
  return 0;
}
