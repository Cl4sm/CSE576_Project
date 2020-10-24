check_string(char *s)
{
  char *str = s;
  static char star[] = "*";

  if (EmptyString(str))
    return star;

  for (; *s; ++s)
  {
    if (IsSpace(*s))
    {
      *s = '\0';
      break;
    }
  }

  return EmptyString(str) ? star : str;
}
