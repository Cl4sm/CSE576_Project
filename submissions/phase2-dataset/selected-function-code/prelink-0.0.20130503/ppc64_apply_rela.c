ppc64_apply_rela (struct prelink_info *info, GElf_Rela *rela, char *buf)
{
  GElf_Addr value;

  value = info->resolve (info, GELF_R_SYM (rela->r_info),
			 GELF_R_TYPE (rela->r_info));
  value += rela->r_addend;
  switch (GELF_R_TYPE (rela->r_info))
    {
    case R_PPC64_NONE:
      break;
    case R_PPC64_GLOB_DAT:
    case R_PPC64_ADDR64:
    case R_PPC64_UADDR64:
      buf_write_be64 (buf, value);
      break;
    case R_PPC64_ADDR32:
    case R_PPC64_UADDR32:
      buf_write_be32 (buf, value);
      break;
    case R_PPC64_ADDR16_HA:
      value += 0x8000;
      /* FALLTHROUGH  */
    case R_PPC64_ADDR16_HI:
      value = value >> 16;
      /* FALLTHROUGH  */
    case R_PPC64_ADDR16:
    case R_PPC64_UADDR16:
    case R_PPC64_ADDR16_LO:
      buf_write_be16 (buf, value);
      break;
    case R_PPC64_ADDR16_HIGHERA:
      value += 0x8000;
      /* FALLTHROUGH  */
    case R_PPC64_ADDR16_HIGHER:
      buf_write_be16 (buf, value >> 32);
      break;
    case R_PPC64_ADDR16_HIGHESTA:
      value += 0x8000;
      /* FALLTHROUGH  */
    case R_PPC64_ADDR16_HIGHEST:
      buf_write_be16 (buf, value >> 48);
      break;
    case R_PPC64_ADDR16_LO_DS:
    case R_PPC64_ADDR16_DS:
      buf_write_be16 (buf, (value & 0xfffc)
			   | (buf_read_ube16 (buf) & 3));
      break;
    case R_PPC64_ADDR24:
      buf_write_be32 (buf, (value & 0x03fffffc)
			   | (buf_read_ube32 (buf) & 0xfc000003));
      break;
    case R_PPC64_ADDR14:
      buf_write_be32 (buf, (value & 0xfffc)
			   | (buf_read_ube32 (buf) & 0xffff0003));
      break;
    case R_PPC64_ADDR14_BRTAKEN:
    case R_PPC64_ADDR14_BRNTAKEN:
      buf_write_be32 (buf, (value & 0xfffc)
			   | (buf_read_ube32 (buf) & 0xffdf0003)
			   | ((((GELF_R_TYPE (rela->r_info)
				 == R_PPC64_ADDR14_BRTAKEN) << 21)
			       ^ (value >> 42)) & 0x00200000));
      break;
    case R_PPC64_REL24:
      buf_write_be32 (buf, ((value - rela->r_offset) & 0x03fffffc)
			   | (buf_read_ube32 (buf) & 0xfc000003));
      break;
    case R_PPC64_REL32:
      buf_write_be32 (buf, value - rela->r_offset);
      break;
    case R_PPC64_REL64:
      buf_write_be64 (buf, value - rela->r_offset);
      break;
    case R_PPC64_RELATIVE:
      error (0, 0, "%s: R_PPC64_RELATIVE in ET_EXEC object?",
	     info->dso->filename);
      return 1;
    default:
      return 1;
    }
  return 0;
}
