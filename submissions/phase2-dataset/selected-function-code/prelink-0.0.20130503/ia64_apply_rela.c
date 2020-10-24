static int
ia64_apply_rela (struct prelink_info *info, GElf_Rela *rela, char *buf)
{
  GElf_Addr value;

  value = info->resolve (info, GELF_R_SYM (rela->r_info),
			 GELF_R_TYPE (rela->r_info));
  value += rela->r_addend;
  if ((GELF_R_TYPE (rela->r_info) & ~3) == R_IA64_DIR32MSB)
    {
      /* Nothing to do.  */
    }
  else if ((GELF_R_TYPE (rela->r_info) & ~3) == R_IA64_PCREL32MSB)
    {
      value -= rela->r_offset & -16;
    }
  else if ((GELF_R_TYPE (rela->r_info) & ~3) == R_IA64_FPTR32MSB)
    {
      /* FIXME */
    }
  else if ((GELF_R_TYPE (rela->r_info) & ~1) == R_IA64_IPLTMSB)
    {
      GElf_Addr gp = info->resolveent->pltgot;

      if (GELF_R_TYPE (rela->r_info) & 1)
	{
	  buf_write_le64 (buf, value);
	  buf_write_le64 (buf + 8, gp);
	}
      else
	{
	  buf_write_be64 (buf, value);
	  buf_write_be64 (buf + 8, gp);
	}
      return 0;
    }
  else
    return 1;

  switch (GELF_R_TYPE (rela->r_info) & 3)
    {
    case 0: buf_write_be32 (buf, value); break;
    case 1: buf_write_le32 (buf, value); break;
    case 2: buf_write_be64 (buf, value); break;
    case 3: buf_write_le64 (buf, value); break;
    }
  return 0;
}
