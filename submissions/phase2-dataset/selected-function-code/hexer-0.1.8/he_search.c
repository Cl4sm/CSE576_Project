he_search(hedit, exp, replace, direction, wrap, increment, end,
          replace_str, replace_len, match_len)
  struct he_s *hedit;
  char *exp;
    /* regular expression.
     */
  char *replace;
    /* replace template.  the replace template may contain back references to
     * the regular expression (`\0', ... `\9').
     */
  int direction;
    /* `direction >= 0': forward search.
     * `direction < 0': reverse search.
     */
  int wrap;
    /* if `wrap' is set, the search continues at the top of the buffer/file
     * if the bottom has been (or vice versa, depending on `direction').
     */
  int increment;
    /* if `increment' is set, the search starts at `hedit->position + 1'
     * rather than at `hedit->position'.  if the direction is set to reverse
     * search, the `increment'-flag has no effect.
     */
  long end;
    /* if `wrap' is not set and `end' is not negative, the search ends at
     * position `end'.
     */
  char **replace_str;
    /* if `replace_str' is non-zero and a match was found, the replace
     * string generated from `replace' will be copied to `*replace_str'.
     * the memory for that replace string will be allocated via `malloc()'.
     * NOTE: the replace string wont be terminated with a null-character
     *   since it may contain null characters.
     */
  long *replace_len;
    /* the length of the replace sting is written to `*replace_len'.
     */
  long *match_len;
    /* the length of the match is written to `*replace_len'.
     */
  /* RETURN VALUE:  if a match was found, the position of the match is
   *   returned;  -1: search failed;  -2: illegal expression (check out
   *   `rx_error'/`rx_error_msg').
   * NOTE:  if the returned value is positive, `*replace_str' has to be 
   *   `free()'d by the caller.
   */
{
  static long *regex;
  long position;

  /* setup the regex I/O */
  regex_init(rxwrap_read, rxwrap_seek, rxwrap_tell);
  rxwrap_current_buffer = hedit->buffer;
  regex_reset();

  if (wrap || end < 0) end = hedit->buffer->size;

  if (exp) if (!(regex = regex_compile(exp, replace))) return -2;
  if (direction < 0) direction = -1; else direction = 1;
  if (direction > 0) {
    position = regex_search(regex, 0, end, hedit->position + !!increment,
                            1, replace_str, replace_len, match_len);
    if (wrap && position < 0) {
      position = regex_search(regex, 0, end, 0, 1,
                              replace_str, replace_len, match_len);
      if (position >= 0) he_message(0, "@Abwrapped@~");
    }
  } else {
    position = regex_search(regex, 0, end, hedit->position - !!hedit->position,
                            -1, replace_str, replace_len, match_len);
    if (wrap && position < 0) {
      position = regex_search(regex, 0, end, hedit->buffer->size - 1,
                              -1, replace_str, replace_len, match_len);
      if (position >= 0) he_message(0, "@Abwrapped@~");
    }
  }
  return position;
}
