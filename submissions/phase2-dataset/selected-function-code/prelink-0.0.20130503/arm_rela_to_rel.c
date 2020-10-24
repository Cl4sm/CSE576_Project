static int
arm_rela_to_rel (DSO *dso, GElf_Rela *rela, GElf_Rel *rel)
{
  rel->r_offset = rela->r_offset;
  rel->r_info = rela->r_info;
  switch (GELF_R_TYPE (rel->r_info))
    {
    case R_ARM_JUMP_SLOT:
      /* We should be never converting .rel.plt into .rela.plt
	 and thus never .rela.plt back to .rel.plt.  */
      abort ();
    case R_ARM_RELATIVE:
    case R_ARM_ABS32:
    case R_ARM_TLS_TPOFF32:
      write_ne32 (dso, rela->r_offset, rela->r_addend);
      break;
    case R_ARM_PC24:
      write_ne32 (dso, rela->r_offset,
		  (read_une32 (dso, rela->r_offset) & 0xff000000)
		  | ((rela->r_addend >> 2) & 0xffffff));
      break;
    case R_ARM_GLOB_DAT:
    case R_ARM_TLS_DTPMOD32:
    case R_ARM_TLS_DTPOFF32:
      write_ne32 (dso, rela->r_offset, 0);
      break;
    }
  return 0;
}
