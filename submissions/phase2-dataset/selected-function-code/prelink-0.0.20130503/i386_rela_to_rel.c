i386_rela_to_rel (DSO *dso, GElf_Rela *rela, GElf_Rel *rel)
{
  rel->r_offset = rela->r_offset;
  rel->r_info = rela->r_info;
  switch (GELF_R_TYPE (rel->r_info))
    {
    case R_386_JMP_SLOT:
      /* We should be never converting .rel.plt into .rela.plt
	 and thus never .rela.plt back to .rel.plt.  */
      abort ();
    case R_386_RELATIVE:
    case R_386_IRELATIVE:
    case R_386_32:
    case R_386_PC32:
    case R_386_TLS_TPOFF32:
    case R_386_TLS_TPOFF:
      write_le32 (dso, rela->r_offset, rela->r_addend);
      break;
    case R_386_COPY:
    case R_386_GLOB_DAT:
    case R_386_TLS_DTPMOD32:
    case R_386_TLS_DTPOFF32:
      write_le32 (dso, rela->r_offset, 0);
      break;
    }
  return 0;
}
