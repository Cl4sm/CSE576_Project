s390x_apply_rela (struct prelink_info *info, GElf_Rela *rela, char *buf)
{
  GElf_Addr value;

  value = info->resolve (info, GELF_R_SYM (rela->r_info),
			 GELF_R_TYPE (rela->r_info));
  value += rela->r_addend;
  switch (GELF_R_TYPE (rela->r_info))
    {
    case R_390_NONE:
      break;
    case R_390_GLOB_DAT:
    case R_390_JMP_SLOT:
    case R_390_64:
      buf_write_be64 (buf, value);
      break;
    case R_390_PC64:
      buf_write_be64 (buf, value - rela->r_offset);
      break;
    case R_390_32:
      buf_write_be32 (buf, value);
      break;
    case R_390_PC32:
      buf_write_be32 (buf, value - rela->r_offset);
      break;
    case R_390_PC32DBL:
    case R_390_PLT32DBL:
      buf_write_be32 (buf, ((Elf32_Sword) (value - rela->r_offset)) >> 1);
      break;
    case R_390_16:
      buf_write_be16 (buf, value);
      break;
    case R_390_PC16:
      buf_write_be16 (buf, value - rela->r_offset);
      break;
    case R_390_PC16DBL:
    case R_390_PLT16DBL:
      buf_write_be16 (buf, ((int16_t) (value - rela->r_offset)) >> 1);
      break;
    case R_390_8:
      buf_write_8 (buf, value);
      break;
    case R_390_COPY:
      abort ();
    case R_390_RELATIVE:
      error (0, 0, "%s: R_390_RELATIVE in ET_EXEC object?", info->dso->filename);
      return 1;
    default:
      return 1;
    }
  return 0;
}
