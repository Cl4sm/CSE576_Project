static int
cris_apply_rela (struct prelink_info *info, GElf_Rela *rela, char *buf)
{
  GElf_Addr value;

  value = info->resolve (info, GELF_R_SYM (rela->r_info),
			 GELF_R_TYPE (rela->r_info));
  value += rela->r_addend;
  switch (GELF_R_TYPE (rela->r_info))
    {
    case R_CRIS_NONE:
      break;
    case R_CRIS_GLOB_DAT:
    case R_CRIS_JUMP_SLOT:
    case R_CRIS_32:
      buf_write_le32 (buf, value);
      break;
    case R_CRIS_16:
      buf_write_le16 (buf, value);
      break;
    case R_CRIS_8:
      buf_write_8 (buf, value);
      break;
    case R_CRIS_32_PCREL:
      buf_write_le32 (buf, value - rela->r_offset - 4);
      break;
    case R_CRIS_16_PCREL:
      buf_write_le16 (buf, value - rela->r_offset - 2);
      break;
    case R_CRIS_8:
      buf_write_8 (buf, value - rela->r_offset - 1);
      break;
    case R_CRIS_COPY:
      abort ();
    case R_CRIS_RELATIVE:
      error (0, 0, "%s: R_CRIS_RELATIVE in ET_EXEC object?", info->dso->filename);
      return 1;
    default:
      return 1;
    }
  return 0;
}
