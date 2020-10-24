char *
collapse(char *mask)
{
  unsigned int star = 0;
  char *m = mask;
  char *b = NULL;

  if (m)
  {
    do
    {
      if ((*m == '*') && (*(m + 1) == '*' || *(m + 1) == '?'))
      {
        b = m;

        do
        {
          if (*m == '*')
            star = 1;
          else
          {
            if (star && (*m != '?'))
            {
              *b++ = '*';
              star = 0;
            }

            *b++ = *m;

            if ((*m == '\\') && (*(m + 1) == '*' || *(m + 1) == '?'))
              *b++ = *++m;
          }
        } while (*m++);

        break;
      }
      else
      {
        if ((*m == '\\') && (*(m + 1) == '*' || *(m + 1) == '?'))
          ++m;
      }
    } while (*m++);
  }

  return mask;
}
