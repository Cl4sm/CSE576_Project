cris_apply_conflict_rela (struct prelink_info *info, GElf_Rela *rela,
			  char *buf, GElf_Addr dest_addr)
{
  switch (GELF_R_TYPE (rela->r_info))
    {
    case R_CRIS_GLOB_DAT:
    case R_CRIS_JUMP_SLOT:
    case R_CRIS_32:
      buf_write_le32 (buf, rela->r_addend);
      break;
    case R_CRIS_16:
      buf_write_le16 (buf, rela->r_addend);
      break;
    case R_CRIS_8:
      buf_write_8 (buf, rela->r_addend);
      break;
    default:
      abort ();
    }
  return 0;
}
