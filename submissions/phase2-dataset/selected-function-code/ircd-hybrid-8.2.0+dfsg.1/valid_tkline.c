time_t
valid_tkline(const char *data, const int minutes)
{
  const unsigned char *p = (const unsigned char *)data;
  unsigned char tmpch = '\0';
  time_t result = 0;

  while ((tmpch = *p++))
  {
    if (!IsDigit(tmpch))
      return 0;

    result *= 10;
    result += (tmpch & 0xF);
  }

  /*
   * In the degenerate case where oper does a /quote kline 0 user@host :reason
   * i.e. they specifically use 0, I am going to return 1 instead
   * as a return value of non-zero is used to flag it as a temporary kline
   */
  if (result == 0)
    result = 1;

  /*
   * If the incoming time is in seconds convert it to minutes for the purpose
   * of this calculation
   */
  if (!minutes)
    result = result / 60;

  if (result > MAX_TDKLINE_TIME)
    result = MAX_TDKLINE_TIME;

  result = result * 60;  /* turn it into seconds */

  return result;
}
