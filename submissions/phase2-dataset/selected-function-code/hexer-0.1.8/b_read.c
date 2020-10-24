b_read(buffer, target, position, count)
  Buffer *buffer;
  char *target;
  long position;
  long count;
{
  BufferBlock *block;
  unsigned long bs = buffer->blocksize;

  if (!(block = find_block(buffer, position))) return 0;
  if (count + position > buffer->size) count = buffer->size - position;
  if (count <= (bs - position % bs)) {
    b_memmove(target, block->data + position % bs, count);
    return count;
  } else {
    long r = bs - position % bs;
    b_memmove(target, block->data + position % bs, r);
    r += b_read(buffer, target + r, position + r, count - r);
    return r;
  }
}
