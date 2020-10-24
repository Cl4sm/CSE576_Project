arm_rel_to_rela (DSO *dso, GElf_Rel *rel, GElf_Rela *rela)
{
  rela->r_offset = rel->r_offset;
  rela->r_info = rel->r_info;
  switch (GELF_R_TYPE (rel->r_info))
    {
    case R_ARM_JUMP_SLOT:
      /* We should be never converting .rel.plt into .rela.plt.  */
      abort ();
    case R_ARM_RELATIVE:
    case R_ARM_ABS32:
    case R_ARM_TLS_TPOFF32:
      rela->r_addend = (Elf32_Sword) read_une32 (dso, rel->r_offset);
      break;
    case R_ARM_PC24:
      rela->r_addend = read_une32 (dso, rel->r_offset) << 8;
      rela->r_addend = ((Elf32_Sword) rela->r_addend) >> 6;
      break;
    case R_ARM_COPY:
    case R_ARM_GLOB_DAT:
    case R_ARM_TLS_DTPMOD32:
    case R_ARM_TLS_DTPOFF32:
      rela->r_addend = 0;
      break;
    }
  return 0;
}
