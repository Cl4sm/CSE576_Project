tio_isprint(x)
  int x;
{
  if (x < 0) x += 0x100;
  if (x < 0x20) return 0;
  if (x > 0xff) return 0;
#if USE_SET
  if (s_get_option_bool("ascii")) {
    if (x > 0x7e) return 0;
  } else
    if (x > 0x7e && x < 0xa1) return 0;
#else
#if HEXER_ASCII_ONLY
  if (x > 0x7e) return 0;
#else /* Assume ISO 8859/1 character set. */
  if (x > 0x7e && x < 0xa1) return 0;
#endif
#endif
  return 1;
}
