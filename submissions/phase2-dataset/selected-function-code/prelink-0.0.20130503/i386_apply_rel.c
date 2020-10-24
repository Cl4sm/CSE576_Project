static int
i386_apply_rel (struct prelink_info *info, GElf_Rel *rel, char *buf)
{
  GElf_Addr value;

  value = info->resolve (info, GELF_R_SYM (rel->r_info),
			 GELF_R_TYPE (rel->r_info));
  switch (GELF_R_TYPE (rel->r_info))
    {
    case R_386_NONE:
      break;
    case R_386_GLOB_DAT:
    case R_386_JMP_SLOT:
      buf_write_le32 (buf, value);
      break;
    case R_386_32:
      buf_write_le32 (buf, value + read_ule32 (info->dso, rel->r_offset));
      break;
    case R_386_PC32:
      buf_write_le32 (buf, value + read_ule32 (info->dso, rel->r_offset)
			   - rel->r_offset);
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
