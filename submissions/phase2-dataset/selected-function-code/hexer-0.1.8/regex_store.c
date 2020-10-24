  static int
regex_store(long slot, long begin, long end)
  /* The text between the positions `begin' (incl.) and `end' (excl.) is
   * stored in slot `slot'.  (Actually only the `begin' and `end' positions
   * are stored.)
   */
{
  assert(slot >= 0 && slot < RX_NSLOTS);
  assert(end >= begin);
  if (!rx_store_initialized) {
    memset(rx_store_begin, 0, RX_NSLOTS * sizeof(long));
    memset(rx_store_end, 0, RX_NSLOTS * sizeof(long));
    memset(rx_store_match, 0, RX_NSLOTS * sizeof(char *));
    rx_store_initialized = 1;
  }
  if (rx_store_match[slot]) {
    free((char *)rx_store_match[slot]);
    rx_store_match[slot] = 0;
  }
  rx_store_begin[slot] = begin;
  rx_store_end[slot] = end;
  return 0;
}
