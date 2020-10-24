unsigned char *
get_data_from_iterator (struct data_iterator *it, GElf_Addr size)
{
  unsigned char *ptr;

  /* If we're at the end of a data block, move onto the next.  */
  if (it->data && it->data->d_off + it->data->d_size == it->sec_offset)
    it->data = elf_getdata (it->dso->scn[it->sec], it->data);

  if (it->data == NULL)
    {
      /* Find out which section contains the next byte.  */
      it->sec = addr_to_sec (it->dso, it->addr);
      if (it->sec < 0)
	return NULL;

      /* Fast-forward to the block that contains ADDR, if any.  */
      it->sec_offset = it->addr - it->dso->shdr[it->sec].sh_addr;
      do
	it->data = elf_getdata (it->dso->scn[it->sec], it->data);
      while (it->data && it->data->d_off + it->data->d_size <= it->sec_offset);
    }

  /* Make sure that all the data we want is included in this block.  */
  if (it->data == NULL
      || it->data->d_off > it->sec_offset
      || it->data->d_off + it->data->d_size < it->sec_offset + size)
    return NULL;

  ptr = (unsigned char *) it->data->d_buf + (it->sec_offset - it->data->d_off);
  it->sec_offset += size;
  it->addr += size;
  return ptr;
}
