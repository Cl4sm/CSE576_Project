static int
mips_rela_to_rel (DSO *dso, GElf_Rela *rela, GElf_Rel *rel)
{
  rel->r_offset = rela->r_offset;
  rel->r_info = rela->r_info;
  switch (GELF_R_TYPE (rela->r_info))
    {
    case R_MIPS_NONE:
      break;

    case R_MIPS_REL32:
    case R_MIPS_TLS_DTPREL32:
    case R_MIPS_TLS_TPREL32:
      /* These relocations have an in-place addend.  */
      write_ne32 (dso, rela->r_offset, rela->r_addend);
      break;

    case R_MIPS_GLOB_DAT:
    case R_MIPS_TLS_DTPMOD32:
      /* These relocations have no addend.  */
      write_ne32 (dso, rela->r_offset, 0);
      break;

    default:
      error (0, 0, "%s: Unknown MIPS relocation type %d", dso->filename,
	     (int) GELF_R_TYPE (rela->r_info));
      return 1;
    }
  return 0;
}
