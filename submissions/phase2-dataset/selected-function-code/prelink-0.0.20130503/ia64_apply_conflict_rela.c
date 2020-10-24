static int
ia64_apply_conflict_rela (struct prelink_info *info, GElf_Rela *rela,
			  char *buf, GElf_Addr dest_addr)
{
  if ((GELF_R_TYPE (rela->r_info) & ~1) == R_IA64_IPLTMSB)
    {
      GElf_Addr gp = 0;
      int i;

      for (i = 0; i < info->ent->ndepends; ++i)
	if (info->ent->depends[i]->base <= rela->r_addend
	    && info->ent->depends[i]->end > rela->r_addend)
	  {
	    gp = info->ent->depends[i]->pltgot;
	    break;
	  }

      if (i == info->ent->ndepends)
	abort ();

      if (GELF_R_TYPE (rela->r_info) & 1)
	{
	  buf_write_le64 (buf, rela->r_addend);
	  buf_write_le64 (buf + 8, gp);
	}
      else
	{
	  buf_write_be64 (buf, rela->r_addend);
	  buf_write_be64 (buf + 8, gp);
	}
      return 0;
    }

  switch (GELF_R_TYPE (rela->r_info))
    {
    case R_IA64_DIR32MSB: buf_write_be32 (buf, rela->r_addend); break;
    case R_IA64_DIR32LSB: buf_write_le32 (buf, rela->r_addend); break;
    case R_IA64_DIR64MSB: buf_write_be64 (buf, rela->r_addend); break;
    case R_IA64_DIR64LSB: buf_write_le64 (buf, rela->r_addend); break;
    default:
      abort ();
    }
  return 0;
}
