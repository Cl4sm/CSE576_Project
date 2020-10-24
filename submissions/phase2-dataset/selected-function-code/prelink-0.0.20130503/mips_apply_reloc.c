static int
mips_apply_reloc (struct prelink_info *info, GElf_Xword r_info,
		  GElf_Rela *rela, char *buf)
{
  DSO *dso;

  dso = info->dso;
  switch (GELF_R_TYPE (r_info))
    {
    case R_MIPS_NONE:
      break;

    case R_MIPS_REL32:
      mips_apply_adjustment (dso, rela, buf,
			     info->resolve (info, GELF_R_SYM (r_info),
					    GELF_R_TYPE (r_info)));
      break;

    default:
      return 1;
    }
  return 0;
}
