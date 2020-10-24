  long
b_copy_to_file(buffer, filename, position, count)
  Buffer *buffer;
  char *filename;
  long position;
  long count;
{
  char *tmp = malloc(buffer->blocksize);
  long bytes_read = 0, bytes_wrote = 0;
  int file = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);

  if (file < 0 || !tmp) {
    if (tmp) free((char *)tmp);
    if (file >= 0) close(file);
    return -1;
  }
  do {
    bytes_read =
      b_read(buffer, tmp, position,
	     (buffer->blocksize < count) ? buffer->blocksize : count);
    if (write(file, tmp, bytes_read) < 0) {
      free((char *)tmp);
      close(file);
      return -1;
    }
    count -= bytes_read;
    bytes_wrote += bytes_read;
    position += bytes_read;
    if (bytes_read == 0 || count <= 0) break;
  } while (1);
  free((char *)tmp);
  close(file);
  return bytes_wrote;
}
