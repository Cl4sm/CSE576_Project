b_copy_forward(buffer, target_position, source_position, count)
  Buffer *buffer;
  long target_position;
  long source_position;
  long count;
{
  BufferBlock *t_block, *s_block;
  long bs = buffer->blocksize;
  long t_offset = target_position % bs; /* target offset. */
  long s_offset = source_position % bs; /* source offset. */
  long tr_offset = (target_position + count - 1) % bs + 1;
    /* target reverse offset. */
  long sr_offset = (source_position + count) % bs;
    /* source reverse offset. */

  assert(!buffer->read_only);
  assert(target_position > source_position);
  BUFFER_CHANGED(buffer);
  if (target_position > buffer->size) return -1;
  if (count + target_position > buffer->size)
    count = buffer->size - target_position;
  assert(t_block = find_block(buffer, target_position + count - 1));
  if (count <= bs - t_offset) {
    if (count <= bs - s_offset) {
      assert(s_block = find_block(buffer, source_position));
      b_memmove(t_block->data + t_offset, s_block->data + s_offset, count);
    } else {
      assert(s_offset > t_offset);
      assert(s_block = find_block(buffer, source_position + count));
      b_memmove(t_block->data + tr_offset - sr_offset, s_block->data,
                sr_offset);
      assert(s_block = find_block(buffer, source_position));
      b_memmove(t_block->data + t_offset, s_block->data + s_offset,
                count - sr_offset);
    }
    return count;
  } else {
    long r = tr_offset;
    if (tr_offset >= sr_offset) {
      if (sr_offset) {
        assert(s_block = find_block(buffer, source_position + count));
        b_memmove(t_block->data + tr_offset - sr_offset,
                s_block->data, sr_offset);
      }
      if (sr_offset != tr_offset) {
        assert(s_block =
               find_block(buffer, source_position + count - tr_offset));
        b_memmove(t_block->data, s_block->data + bs - tr_offset + sr_offset,
                tr_offset - sr_offset);
      }
    } else {
      assert(s_block = find_block(buffer, source_position + count));
      b_memmove(t_block->data, s_block->data + sr_offset - tr_offset,
              tr_offset);
    }
    r += b_copy_forward(buffer, target_position, source_position, count - r);
    return r;
  }
}
