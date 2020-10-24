rte_src_alloc_id(void)
{
  int i, j;
  for (i = src_id_pos; i < src_id_size; i++)
    if (src_ids[i] != 0xffffffff)
      goto found;

  /* If we are at least 7/8 full, expand */
  if (src_id_used > (src_id_size * 28))
    {
      src_id_size *= 2;
      src_ids = mb_realloc(src_ids, src_id_size * sizeof(u32));
      bzero(src_ids + i, (src_id_size - i) * sizeof(u32));
      goto found;
    }

  for (i = 0; i < src_id_pos; i++)
    if (src_ids[i] != 0xffffffff)
      goto found;

  ASSERT(0);

 found:
  ASSERT(i < 0x8000000);

  src_id_pos = i;
  j = u32_cto(src_ids[i]);

  src_ids[i] |= (1 << j);
  src_id_used++;
  return 32 * i + j;
}
