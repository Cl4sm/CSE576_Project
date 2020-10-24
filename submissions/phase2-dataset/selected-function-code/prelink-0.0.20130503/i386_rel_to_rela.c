i386_rel_to_rela (DSO *dso, GElf_Rel *rel, GElf_Rela *rela)
{
  rela->r_offset = rel->r_offset;
  rela->r_info = rel->r_info;
  switch (GELF_R_TYPE (rel->r_info))
    {
    case R_386_JMP_SLOT:
      /* We should be never converting .rel.plt into .rela.plt.  */
      abort ();
    case R_386_RELATIVE:
    case R_386_IRELATIVE:
    case R_386_32:
    case R_386_PC32:
    case R_386_TLS_TPOFF32:
    case R_386_TLS_TPOFF:
      rela->r_addend = (Elf32_Sword) read_ule32 (dso, rel->r_offset);
      break;
    case R_386_COPY:
    case R_386_GLOB_DAT:
    case R_386_TLS_DTPOFF32:
    case R_386_TLS_DTPMOD32:
      rela->r_addend = 0;
      break;
    }
  return 0;
}
