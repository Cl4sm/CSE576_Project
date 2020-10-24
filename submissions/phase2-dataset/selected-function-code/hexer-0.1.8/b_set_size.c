b_set_size(buffer, size)
  Buffer *buffer;
  unsigned long size;
{
  BufferBlock *i, *j = 0, *k;
  long block_number;

  assert(!buffer->read_only);
  BUFFER_CHANGED(buffer);
  if (size < buffer->size) {
    if (!(i = find_block(buffer, size))) return -1;
    k = i;
    for (i = i->next_block; i; i = j) {
      j = i->next_block;
      delete_buffer_block(i);
    }
    k->next_block = 0;
  } else {
    if (buffer->first_block == 0) {
      buffer->first_block = new_buffer_block(buffer->blocksize, 0);
    }
    for (i = buffer->first_block, block_number = 1;
	 i->next_block != 0;
	 i = i->next_block, block_number++);
    for (;
         block_number * buffer->blocksize < size;
         i = i->next_block, block_number++) {
      if (!i) return -1;
      i->next_block = new_buffer_block(buffer->blocksize, 0);
    }
  }
  buffer->size = size;
  return 0;
}
