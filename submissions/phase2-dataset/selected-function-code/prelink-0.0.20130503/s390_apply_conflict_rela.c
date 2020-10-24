static int
s390_apply_conflict_rela (struct prelink_info *info, GElf_Rela *rela,
			  char *buf, GElf_Addr dest_addr)
{
  GElf_Rela *ret;
  switch (GELF_R_TYPE (rela->r_info))
    {
    case R_390_32:
      buf_write_be32 (buf, rela->r_addend);
      break;
    case R_390_16:
      buf_write_be16 (buf, rela->r_addend);
      break;
    case R_390_8:
      buf_write_8 (buf, rela->r_addend);
      break;
    case R_390_IRELATIVE:
      if (dest_addr == 0)
	return 5;
      ret = prelink_conflict_add_rela (info);
      if (ret == NULL)
	return 1;
      ret->r_offset = dest_addr;
      ret->r_info = GELF_R_INFO (0, R_390_IRELATIVE);
      ret->r_addend = rela->r_addend;
      break;
    default:
      abort ();
    }
  return 0;
}
