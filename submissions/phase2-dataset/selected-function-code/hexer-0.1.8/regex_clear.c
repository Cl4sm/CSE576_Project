regex_clear(void)
  /* clear all slots.
   */
{
  int i;

  if (!rx_store_initialized) {
    memset(rx_store_begin, 0, RX_NSLOTS * sizeof(long));
    memset(rx_store_end, 0, RX_NSLOTS * sizeof(long));
    memset(rx_store_match, 0, RX_NSLOTS * sizeof(char *));
    rx_store_initialized = 1;
  } else {
    for (i = 0; i < RX_NSLOTS; ++i) {
      if (rx_store_match[i]) {
	free((char *)rx_store_match[i]);
	rx_store_match[i] = 0;
      }
      rx_store_begin[i] = rx_store_end[i] = 0;
    }
  }
}
