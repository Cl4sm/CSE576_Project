sh_apply_rela (struct prelink_info *info, GElf_Rela *rela, char *buf)
{
  GElf_Addr value;

  value = info->resolve (info, GELF_R_SYM (rela->r_info),
			 GELF_R_TYPE (rela->r_info));
  value += rela->r_addend;
  switch (GELF_R_TYPE (rela->r_info))
    {
    case R_SH_NONE:
      break;
    case R_SH_GLOB_DAT:
    case R_SH_JMP_SLOT:
    case R_SH_DIR32:
      buf_write_ne32 (info->dso, buf, value);
      break;
    case R_SH_REL32:
      buf_write_ne32 (info->dso, buf, value - rela->r_offset);
      break;
    case R_SH_COPY:
      abort ();
    case R_SH_RELATIVE:
      error (0, 0, "%s: R_SH_RELATIVE in ET_EXEC object?", info->dso->filename);
      return 1;
    default:
      return 1;
    }
  return 0;
}
