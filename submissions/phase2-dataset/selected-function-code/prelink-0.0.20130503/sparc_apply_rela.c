sparc_apply_rela (struct prelink_info *info, GElf_Rela *rela, char *buf)
{
  GElf_Addr value;

  value = info->resolve (info, GELF_R_SYM (rela->r_info),
			 GELF_R_TYPE (rela->r_info));
  value += rela->r_addend;
  switch (GELF_R_TYPE (rela->r_info))
    {
    case R_SPARC_NONE:
      break;
    case R_SPARC_DISP32:
      value -= rela->r_offset;
    case R_SPARC_GLOB_DAT:
    case R_SPARC_32:
    case R_SPARC_UA32:
      buf_write_be32 (buf, value);
      break;
    case R_SPARC_DISP16:
      value -= rela->r_offset;
    case R_SPARC_16:
    case R_SPARC_UA16:
      buf_write_be16 (buf, value);
      break;
    case R_SPARC_DISP8:
      value -= rela->r_offset;
    case R_SPARC_8:
      buf_write_8 (buf, value);
      break;
    case R_SPARC_LO10:
      buf_write_be32 (buf, (buf_read_ube32 (buf) & ~0x3ff) | (value & 0x3ff));
      break;
    case R_SPARC_HI22:
      buf_write_be32 (buf, (buf_read_ube32 (buf) & 0xffc00000)
			   | ((value >> 10) & 0x3fffff));
      break;
    case R_SPARC_WDISP30:
      buf_write_be32 (buf, (buf_read_ube32 (buf) & 0xc0000000)
			   | (((value - rela->r_offset) >> 2) & 0x3fffffff));
      break;
    case R_SPARC_RELATIVE:
      error (0, 0, "%s: R_SPARC_RELATIVE in ET_EXEC object?",
	     info->dso->filename);
      return 1;
    default:
      return 1;
    }
  return 0;
}