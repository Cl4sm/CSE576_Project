  long
b_write_buffer_to_file(buffer, filename)
  Buffer *buffer;
  char *filename;
{
  BufferBlock *i;
  long bytes_wrote = 0;
  long bs = buffer->blocksize;
  long blocks;
  int file = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);

  for (i = buffer->first_block, blocks = 1;
       i;
       i = i->next_block, blocks++) {
    long bytes;
    bytes = write(file,
                  i->data,
                  (blocks * bs > buffer->size) ? buffer->size % bs : bs);
    if (bytes < 0) {
      close(file);
      return -1;
    }
    bytes_wrote += bytes;
  }
  close(file);
  return bytes_wrote;
}
