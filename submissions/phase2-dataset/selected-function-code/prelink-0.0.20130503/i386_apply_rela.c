static int
i386_apply_rela (struct prelink_info *info, GElf_Rela *rela, char *buf)
{
  GElf_Addr value;

  value = info->resolve (info, GELF_R_SYM (rela->r_info),
			 GELF_R_TYPE (rela->r_info));
  switch (GELF_R_TYPE (rela->r_info))
    {
    case R_386_NONE:
      break;
    case R_386_GLOB_DAT:
    case R_386_JMP_SLOT:
    case R_386_32:
      buf_write_le32 (buf, value + rela->r_addend);
      break;
    case R_386_PC32:
      buf_write_le32 (buf, value + rela->r_addend - rela->r_offset);
      break;
    case R_386_COPY:
      abort ();
    case R_386_RELATIVE:
      error (0, 0, "%s: R_386_RELATIVE in ET_EXEC object?", info->dso->filename);
      return 1;
    default:
      return 1;
    }
  return 0;
}
