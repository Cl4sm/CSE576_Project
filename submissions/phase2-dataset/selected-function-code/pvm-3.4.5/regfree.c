regfree (preg)
    regex_t *preg;
{
  if (preg->buffer != NULL)
    free (preg->buffer);
  preg->buffer = NULL;
  
  preg->allocated = 0;
  preg->used = 0;

  if (preg->fastmap != NULL)
    free (preg->fastmap);
  preg->fastmap = NULL;
  preg->fastmap_accurate = 0;

  if (preg->translate != NULL)
    free (preg->translate);
  preg->translate = NULL;
}
