int dstr_append(dstring *string, const char *str, size_t len)
{
  size_t newlen;

  if (len == (size_t) -1)
    len = strlen(str);

  newlen = string->len + len;

  /* make sure there is room */

  if (newlen >= string->space)
    {
      char *new;

      newlen *= 2;

      new = malloc(newlen);
      if (new == 0)
	return -1;

      string->space = newlen;

      memcpy(new, string->str, string->len);

      if (string->str != string->sbuf)
	free(string->str);

      string->str = new;
    }

  /* append the string */

  memcpy(string->str + string->len, str, len);

  string->len += len;
  string->str[string->len] = 0;

  return 0;
}
