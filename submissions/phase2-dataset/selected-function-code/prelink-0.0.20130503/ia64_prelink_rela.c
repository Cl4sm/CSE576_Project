ia64_prelink_rela (struct prelink_info *info, GElf_Rela *rela,
		   GElf_Addr relaaddr)
{
  DSO *dso;
  GElf_Addr value;

  if ((GELF_R_TYPE (rela->r_info) & ~3) == R_IA64_REL32MSB
      || GELF_R_TYPE (rela->r_info) == R_IA64_NONE)
    /* Fast path: nothing to do.  */
    return 0;
  dso = info->dso;
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
	  write_le64 (dso, rela->r_offset, value);
	  write_le64 (dso, rela->r_offset + 8, gp);
	}
      else
	{
	  write_be64 (dso, rela->r_offset, value);
	  write_be64 (dso, rela->r_offset + 8, gp);
	}

      return 0;
    }
  else
    {
      error (0, 0, "%s: Unknown ia64 relocation type %d", dso->filename,
	     (int) GELF_R_TYPE (rela->r_info));
      return 1;
    }

  switch (GELF_R_TYPE (rela->r_info) & 3)
    {
    case 0: write_be32 (dso, rela->r_offset, value); break;
    case 1: write_le32 (dso, rela->r_offset, value); break;
    case 2: write_be64 (dso, rela->r_offset, value); break;
    case 3: write_le64 (dso, rela->r_offset, value); break;
    }
  return 0;
}
