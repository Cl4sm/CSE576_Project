mips_rel_to_rela (DSO *dso, GElf_Rel *rel, GElf_Rela *rela)
{
  rela->r_offset = rel->r_offset;
  rela->r_info = rel->r_info;
  switch (GELF_R_TYPE (rel->r_info))
    {
    case R_MIPS_REL32:
    case R_MIPS_TLS_DTPREL32:
    case R_MIPS_TLS_TPREL32:
      /* These relocations have an in-place addend.  */
      rela->r_addend = (int32_t) read_une32 (dso, rel->r_offset);
      break;

    case R_MIPS_NONE:
    case R_MIPS_GLOB_DAT:
    case R_MIPS_TLS_DTPMOD32:
      /* These relocations have no addend.  */
      rela->r_addend = 0;
      break;

    default:
      error (0, 0, "%s: Unknown MIPS relocation type %d", dso->filename,
	     (int) GELF_R_TYPE (rel->r_info));
      return 1;
    }
  return 0;
}
