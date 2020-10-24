x86_64_apply_conflict_rela (struct prelink_info *info, GElf_Rela *rela,
			    char *buf, GElf_Addr dest_addr)
{
  GElf_Rela *ret;
  switch (GELF_R_TYPE (rela->r_info))
    {
    case R_X86_64_GLOB_DAT:
    case R_X86_64_JUMP_SLOT:
    case R_X86_64_64:
      buf_write_le64 (buf, rela->r_addend);
      break;
    case R_X86_64_IRELATIVE:
      if (dest_addr == 0)
	return 5;
      ret = prelink_conflict_add_rela (info);
      if (ret == NULL)
	return 1;
      ret->r_offset = dest_addr;
      ret->r_info = GELF_R_INFO (0, R_X86_64_IRELATIVE);
      ret->r_addend = rela->r_addend;
      break;
    case R_X86_64_32:
      buf_write_le32 (buf, rela->r_addend);
      break;
    default:
      abort ();
    }
  return 0;
}
