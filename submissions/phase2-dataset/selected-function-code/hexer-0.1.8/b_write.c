  long
b_write(buffer, source, position, count)
  Buffer *buffer;
  char *source;
  long position;
  long count;
{
  BufferBlock *block;
  unsigned long bs = buffer->blocksize;

  assert(!buffer->read_only);
  if (count) BUFFER_CHANGED(buffer);
  if (!(block = find_block(buffer, position))) return 0;
  if (count + position > buffer->size) count = buffer->size - position;
  if (count <= (bs - position % bs)) {
    b_memmove(block->data + position % bs, source, count);
    return count;
  } else {
    long r = bs - position % bs;
    b_memmove(block->data + position % bs, source, r);
    return r + b_write(buffer, source + r, position + r, count - r);
  }
}
