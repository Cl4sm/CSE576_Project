  long
regex_search(regex, begin, end, start, direction,
             replace_str, replace_len, match_len)
  long *regex;
  long begin;
  long end;
  long start;
  int direction; /* >=0: forward; <0: reverse. */
  char **replace_str;
  long *replace_len;  /* the replace-string for the match is written to
		       * `*replace_str'/`replace_len'.  the memory for
		       * `*replace_str' is allocated by `regex_match()'
		       * via `malloc()'. */
  long *match_len;
{
  long position;
  int i;
  char *bp1;
  long base1;

  direction = direction < 0 ? -1 : 1;
  if (end < 0) end = (u_long)~0 >> 1;
  position = start;

  /* initialize the buffer */
  bp = buffer + (position & REGEX_BLOCKMASK);
  buffer_base = position & ~REGEX_BLOCKMASK;
  rx_seek(buffer_base);
  i = rx_read(buffer, 2 * REGEX_BLOCKSIZE);
  if (i < 2 * REGEX_BLOCKSIZE) {
    rx_size = buffer_base + i;
    memset(buffer + i, 0, 2 * REGEX_BLOCKSIZE - i);
  } else
    rx_size = 0;

  if (rx_size && end > rx_size) end = rx_size;
  if (rx_size && position >= rx_size) position = rx_size - 1;
  while (position < end && position >= begin) {
    if (rx_start >= 0)
      while (*bp != (char)rx_start && position < end) {
	if (*rx_interrupt) {
	  rx_error = (int)E_interrupt;
          *rx_interrupt = 0;
	  regex_clear();
	  return -1;
	}
	bp += direction;
	position += direction;
	REGEX_ADVANCE;
	if (rx_size && end > rx_size) end = rx_size;
      }
    bp1 = bp;
    base1 = buffer_base;
    if (regex_match(regex, position, replace_str, replace_len, match_len))
      return position;
    if (rx_error) return -1;
    if (rx_size && end > rx_size) end = rx_size;
    position += direction;
    if (base1 == buffer_base) {
      bp = bp1 + direction;
      REGEX_ADVANCE;
    } else {
      regex_set_position(position);
      if (rx_size && end > rx_size) end = rx_size;
    }
  }
  return -1;
}
