ia64_adjust_rela (DSO *dso, GElf_Rela *rela, GElf_Addr start,
		  GElf_Addr adjust)
{
  if ((GELF_R_TYPE (rela->r_info) & ~3) == R_IA64_REL32MSB
      && rela->r_addend >= start)
    {
      rela->r_addend += adjust;
      switch (GELF_R_TYPE (rela->r_info) & 3)
	{
	case 0: write_be32 (dso, rela->r_offset, rela->r_addend); break;
	case 1: write_le32 (dso, rela->r_offset, rela->r_addend); break;
	case 2: write_be64 (dso, rela->r_offset, rela->r_addend); break;
	case 3: write_le64 (dso, rela->r_offset, rela->r_addend); break;
	}
    }
  else if ((GELF_R_TYPE (rela->r_info) & ~1) == R_IA64_IPLTMSB)
    {
      GElf_Addr val, gp;

      if (GELF_R_TYPE (rela->r_info) & 1)
	{
	  val = read_ule64 (dso, rela->r_offset);
	  gp = read_ule64 (dso, rela->r_offset + 8);
	}
      else
	{
	  val = read_ube64 (dso, rela->r_offset);
	  gp = read_ube64 (dso, rela->r_offset + 8);
	}
      if (gp == dso->info[DT_PLTGOT])
	{
	  if (val >= start)
	    val += adjust;
	  if (gp >= start)
	    gp += adjust;
	}
      if (GELF_R_TYPE (rela->r_info) & 1)
	{
	  write_le64 (dso, rela->r_offset, val);
	  write_le64 (dso, rela->r_offset + 8, gp);
	}
      else
	{
	  write_le64 (dso, rela->r_offset, val);
	  write_le64 (dso, rela->r_offset + 8, gp);
	}
    }
  return 0;
}
