     char *str1;
     const char *str2;
     long n;
{
  if (!n) n = strlen(str2);
  if (str1)
    {
      long len = strlen(str1);
      str1 = (char *)realloc(str1, (sizet)(len + n + 1));
      if (!str1) return 0L;
      strncat(str1 + len, str2, n);
      return str1;
    }
  str1 = (char *)malloc((sizet)(n + 1));
  if (!str1) return 0L;
  str1[0] = 0;
  strncat(str1, str2, n);
  return str1;
}
