  static char *
regex_ref(int slot)
  /* The contents of undefined slots default to an empty string, therefore
   * the return value is always a valid (non-zero) pointer.
   */
{
  long position, count;

  assert(slot >= 0 && slot < RX_NSLOTS);
  if (!rx_store_initialized) {
    memset(rx_store_begin, 0, RX_NSLOTS * sizeof(long));
    memset(rx_store_end, 0, RX_NSLOTS * sizeof(long));
    memset(rx_store_match, 0, RX_NSLOTS * sizeof(char *));
    rx_store_initialized = 1;
  }
  position = rx_store_begin[slot];
  count = rx_store_end[slot] - position;
  if (!rx_store_match[slot] && count) {
    rx_store_match[slot] = malloc(count);
    rx_seek(position);
    rx_read(rx_store_match[slot], count);
    return rx_store_match[slot];
  } else if (rx_store_match[slot])
    return rx_store_match[slot];
  else
    return "";
}
