regex_set_position(long position)
  /* NOTE: `bp' and `buffer_base' *must* be initialized before calling
   *   `regex_set_position'.
   */
{
  long i;

  if (position - buffer_base >= 0) {
    if (position - buffer_base > REGEX_BLOCKSIZE) {
      if (position - buffer_base < 2 * REGEX_BLOCKSIZE) {
	bp = buffer + (position & REGEX_BLOCKMASK);
	rx_memmove(buffer - REGEX_BLOCKSIZE, buffer, 2 * REGEX_BLOCKSIZE);
	buffer_base += REGEX_BLOCKSIZE;
	rx_seek(buffer_base);
	i = rx_read(buffer + REGEX_BLOCKSIZE, REGEX_BLOCKSIZE);
	if (i < REGEX_BLOCKSIZE) {
	  rx_size = buffer_base + i;
	  memset(buffer + REGEX_BLOCKSIZE + i, 0, REGEX_BLOCKSIZE - i);
	}
	return 0;
      } else
        goto rebuild_buffer;
    }
    bp = buffer + (position & REGEX_BLOCKMASK);
    return 0;
  }

rebuild_buffer:
  bp = buffer + (position & REGEX_BLOCKMASK);
  buffer_base = position & ~REGEX_BLOCKMASK;
  rx_seek(buffer_base);
  i = rx_read(buffer, 2 * REGEX_BLOCKSIZE);
  if (i < 2 * REGEX_BLOCKSIZE) {
    rx_size = buffer_base + i;
    memset(buffer + i, 0, 2 * REGEX_BLOCKSIZE - i);
  }
  return 0;
}
