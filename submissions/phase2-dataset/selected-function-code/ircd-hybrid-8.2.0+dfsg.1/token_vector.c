int
token_vector(char *names, char token, char *vector[], int size)
{
  int count = 0;
  char *start = names;
  char *end = NULL;

  assert(names);
  assert(vector);
  assert(size > 1);

  vector[count++] = start;

  for (end = strchr(start, token); end;
       end = strchr(start, token))
  {
    *end++ = '\0';
    start = end;

    if (*start)
    {
      vector[count++] = start;
      if (count < size)
        continue;
    }

    break;
  }

  return count;
}
