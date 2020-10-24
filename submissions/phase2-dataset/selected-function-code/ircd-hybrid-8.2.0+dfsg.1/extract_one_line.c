extract_one_line(struct dbuf_queue *qptr, char *buffer)
{
  int line_bytes = 0, eol_bytes = 0;
  dlink_node *ptr;

  DLINK_FOREACH(ptr, qptr->blocks.head)
  {
    struct dbuf_block *block = ptr->data;
    unsigned int idx;

    if (ptr == qptr->blocks.head)
      idx = qptr->pos;
    else
      idx = 0;

    for (; idx < block->size; ++idx)
    {
      char c = block->data[idx];

      if (IsEol(c))
      {
        ++eol_bytes;

        /* Allow 2 eol bytes per message */
        if (eol_bytes == 2)
          goto out;
      }
      else if (eol_bytes)
        goto out;
      else if (line_bytes++ < IRCD_BUFSIZE - 2)
        *buffer++ = c;
    }
  }

out:

  /*
   * Now, if we haven't found an EOL, ignore all line bytes
   * that we have read, since this is a partial line case.
   */
  if (eol_bytes)
    *buffer = '\0';
  else
    line_bytes = 0;

  /* Remove what is now unnecessary */
  dbuf_delete(qptr, line_bytes + eol_bytes);

  return IRCD_MIN(line_bytes, IRCD_BUFSIZE - 2);
}
