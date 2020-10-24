static int
sparc64_apply_conflict_rela (struct prelink_info *info, GElf_Rela *rela,
			     char *buf, GElf_Addr dest_addr)
{
  switch (SPARC64_R_TYPE (rela->r_info))
    {
    case R_SPARC_64:
    case R_SPARC_UA64:
      buf_write_be64 (buf, rela->r_addend);
      break;
    case R_SPARC_32:
    case R_SPARC_UA32:
      buf_write_be32 (buf, rela->r_addend);
      break;
    case R_SPARC_16:
    case R_SPARC_UA16:
      buf_write_be16 (buf, rela->r_addend);
      break;
    case R_SPARC_8:
      buf_write_8 (buf, rela->r_addend);
      break;
    default:
      abort ();
    }
  return 0;
}
