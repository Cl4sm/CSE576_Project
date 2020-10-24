b_read_buffer_from_file(buffer, filename)
  Buffer *buffer;
  char *filename;
{
  BufferBlock *i = 0;
  long bytes_read = 0;
  char *tmp;
  int file = open(filename, O_RDONLY);
    
  assert(!buffer->read_only);
  BUFFER_CHANGED(buffer);
  if (file <= 0) {
    close(file);
    return -1;
  }
  b_clear(buffer);
  do {
    tmp = (char *)malloc(buffer->blocksize);
    if (tmp == 0) {
      close(file);
      return -1;
    }
    bytes_read = read(file, tmp, buffer->blocksize);
    if (bytes_read > 0)	{
      if (buffer->first_block == 0) {
	buffer->first_block =
	  new_buffer_block(buffer->blocksize, tmp);
	i = buffer->first_block;
      } else {
	i->next_block = new_buffer_block(buffer->blocksize, tmp);
	i = i->next_block;
      }
      buffer->size += bytes_read;
    } else {
      free((char *)tmp);
      break;
    }
  } while (1);
  close(file);
  return buffer->size;
}
