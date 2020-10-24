int is_directory_traversal(char *str)
{
  unsigned int mode, countdots;
  /* mode 0 = fresh, 1 = just dots, 2 = not just dots */

  mode = countdots = 0;

  while (*str)
  {
    char ch = *str++;

    if ((ch == '/') && (mode == 1) && (countdots > 1))
      return 1;

    if (ch == '/')
    {
       mode = countdots = 0;
       continue;
    }

    if (ch == '.')
    {
      if (mode == 0)
        mode = 1;

      countdots++;
    }
    else
      mode = 2;
  }

  if ((mode == 1) && (countdots > 1))
    return 1;

  return 0;
}