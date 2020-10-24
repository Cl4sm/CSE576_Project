b_paste_from_file(buffer, filename, position)
  Buffer *buffer;
  char *filename;
  long position;
{
  char *tmp = malloc(buffer->blocksize);
  long bytes_read, bytes_wrote = 0;
  int file = open(filename, O_RDONLY);
  
  assert(!buffer->read_only);
  if (file < 0 || !tmp) {
    if (tmp) free((char *)tmp);
    if (file >= 0) close(file);
    return -1;
  }
  BUFFER_CHANGED(buffer);
  do {
    bytes_read = read(file, tmp, buffer->blocksize);
    if (b_insert(buffer, position, bytes_read) < 0) {
      free((char *)tmp);
      close(file);
      return -1;
    }
    bytes_wrote += b_write(buffer, tmp, position, bytes_read);
    position += bytes_read;
  } while (bytes_read > 0);
  free((char *)tmp);
  close(file);
  return bytes_wrote;
}
