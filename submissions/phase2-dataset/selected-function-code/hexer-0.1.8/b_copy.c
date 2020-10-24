  long
b_copy(target_buffer, source_buffer, target_position, source_position, count)
  Buffer *target_buffer;
  Buffer *source_buffer;
  long target_position;
  long source_position;
  long count;
{
  BufferBlock *t_block;
  long t_offset = target_position % target_buffer->blocksize;
  long t_blocksize = target_buffer->blocksize;

  assert(!target_buffer->read_only);
  if (count) BUFFER_CHANGED(target_buffer);
  if ((target_buffer == source_buffer) &&
      (target_position > source_position)) {
    long r;
    r = b_copy_forward(target_buffer, target_position,
                       source_position, count);
    return r;
  }
  if (!(t_block = find_block(target_buffer, target_position))) return -1;
  if (count <= t_blocksize - t_offset) {
    long r = b_read(source_buffer, t_block->data + t_offset,
                    source_position, count);
    return r;
  } else {
    long r = b_read(source_buffer, t_block->data + t_offset,
                    source_position, t_blocksize - t_offset);
    if (r >= t_blocksize - t_offset)
      r += b_copy(target_buffer, source_buffer,
                  target_position + r, source_position + r, count - r);
    return r;
  }
}
