ppc_undo_prelink_rela (DSO *dso, GElf_Rela *rela, GElf_Addr relaaddr)
{
  switch (GELF_R_TYPE (rela->r_info))
    {
    case R_PPC_NONE:
      return 0;
    case R_PPC_RELATIVE:
    case R_PPC_GLOB_DAT:
    case R_PPC_ADDR32:
    case R_PPC_UADDR32:
    case R_PPC_REL32:
    case R_PPC_DTPMOD32:
    case R_PPC_DTPREL32:
    case R_PPC_TPREL32:
      write_be32 (dso, rela->r_offset, 0);
      break;
    case R_PPC_JMP_SLOT:
      /* .plt section will become SHT_NOBITS if DT_PPC_GOT is not present,
	 otherwise .plt section will be unprelinked in
	 ppc_arch_undo_prelink.  */
      return 0;
    case R_PPC_IRELATIVE:
      /* .iplt section will become SHT_NOBITS.  */
      return 0;
    case R_PPC_ADDR16:
    case R_PPC_UADDR16:
    case R_PPC_ADDR16_LO:
    case R_PPC_ADDR16_HI:
    case R_PPC_ADDR16_HA:
    case R_PPC_DTPREL16:
    case R_PPC_TPREL16:
    case R_PPC_DTPREL16_LO:
    case R_PPC_TPREL16_LO:
    case R_PPC_DTPREL16_HI:
    case R_PPC_TPREL16_HI:
    case R_PPC_DTPREL16_HA:
    case R_PPC_TPREL16_HA:
      write_be16 (dso, rela->r_offset, 0);
      break;
    case R_PPC_ADDR24:
    case R_PPC_REL24:
      write_be32 (dso, rela->r_offset,
		  read_ube32 (dso, rela->r_offset) & 0xfc000003);
      break;
    case R_PPC_ADDR14:
      write_be32 (dso, rela->r_offset,
		  read_ube32 (dso, rela->r_offset) & 0xffff0003);
      break;
    case R_PPC_ADDR14_BRTAKEN:
    case R_PPC_ADDR14_BRNTAKEN:
      write_be32 (dso, rela->r_offset,
		  read_ube32 (dso, rela->r_offset) & 0xffdf0003);
      break;
    case R_PPC_COPY:
      if (dso->ehdr.e_type == ET_EXEC)
	/* COPY relocs are handled specially in generic code.  */
	return 0;
      error (0, 0, "%s: R_PPC_COPY reloc in shared library?", dso->filename);
      return 1;
    default:
      error (0, 0, "%s: Unknown ppc relocation type %d", dso->filename,
	     (int) GELF_R_TYPE (rela->r_info));
      return 1;
    }
  return 0;
}
