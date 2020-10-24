static int
alpha_apply_rela (struct prelink_info *info, GElf_Rela *rela, char *buf)
{
  GElf_Addr value;

  value = info->resolve (info, GELF_R_SYM (rela->r_info),
			 GELF_R_TYPE (rela->r_info));
  switch (GELF_R_TYPE (rela->r_info))
    {
    case R_ALPHA_NONE:
      break;
    case R_ALPHA_GLOB_DAT:
    case R_ALPHA_REFQUAD:
    case R_ALPHA_JMP_SLOT:
      buf_write_le64 (buf, value + rela->r_addend);
      break;
    case R_ALPHA_RELATIVE:
      error (0, 0, "%s: R_ALPHA_RELATIVE in ET_EXEC object?", info->dso->filename);
      return 1;
    default:
      return 1;
    }
  return 0;
}
