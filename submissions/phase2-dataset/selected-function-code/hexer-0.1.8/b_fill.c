  long
b_fill(buffer, c, position, count)
  Buffer *buffer;
  char c;
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
    memset(block->data + position % bs, c, count);
    return count;
  } else {
    long r = bs - position % bs;
    memset(block->data + position % bs, c, r);
    return r + b_fill(buffer, c, position + r, count - r);
  }
}
