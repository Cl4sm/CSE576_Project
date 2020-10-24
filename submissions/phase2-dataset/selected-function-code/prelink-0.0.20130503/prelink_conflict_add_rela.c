GElf_Rela *
prelink_conflict_add_rela (struct prelink_info *info)
{
  GElf_Rela *ret;

  if (info->conflict_rela_alloced == info->conflict_rela_size)
    {
      info->conflict_rela_alloced += 10;
      info->conflict_rela = realloc (info->conflict_rela,
				     info->conflict_rela_alloced
				     * sizeof (GElf_Rela));
      if (info->conflict_rela == NULL)
	{
	  error (0, ENOMEM, "Could not build .gnu.conflict section memory image");
	  return NULL;
	}
    }
  ret = info->conflict_rela + info->conflict_rela_size++;
  ret->r_offset = 0;
  ret->r_info = 0;
  ret->r_addend = 0;
  return ret;
}
