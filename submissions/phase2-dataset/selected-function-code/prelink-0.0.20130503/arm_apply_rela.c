static int
arm_apply_rela (struct prelink_info *info, GElf_Rela *rela, char *buf)
{
  GElf_Addr value;
  Elf32_Sword val;

  value = info->resolve (info, GELF_R_SYM (rela->r_info),
			 GELF_R_TYPE (rela->r_info));
  switch (GELF_R_TYPE (rela->r_info))
    {
    case R_ARM_NONE:
      break;
    case R_ARM_GLOB_DAT:
    case R_ARM_JUMP_SLOT:
    case R_ARM_ABS32:
      buf_write_ne32 (info->dso, buf, value + rela->r_addend);
      break;
    case R_ARM_PC24:
      val = value + rela->r_addend - rela->r_offset;
      val >>= 2;
      if ((Elf32_Word) val + 0x800000 >= 0x1000000)
	{
	  error (0, 0, "%s: R_ARM_PC24 overflow", info->dso->filename);
	  return 1;
	}
      val &= 0xffffff;
      buf_write_ne32 (info->dso, buf, (buf_read_une32 (info->dso, buf) & 0xff000000) | val);
      break;
    case R_ARM_COPY:
      abort ();
    case R_ARM_RELATIVE:
      error (0, 0, "%s: R_ARM_RELATIVE in ET_EXEC object?", info->dso->filename);
      return 1;
    default:
      return 1;
    }
  return 0;
}
