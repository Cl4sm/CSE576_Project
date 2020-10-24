void
cli_printf(cli *c, int code, char *msg, ...)
{
  va_list args;
  byte buf[CLI_LINE_SIZE];
  int cd = code;
  int errcode;
  int size, cnt;

  if (cd < 0)
    {
      cd = -cd;
      if (cd == c->last_reply)
	size = bsprintf(buf, " ");
      else
	size = bsprintf(buf, "%04d-", cd);
      errcode = -8000;
    }
  else if (cd == CLI_ASYNC_CODE)
    {
      size = 1; buf[0] = '+'; 
      errcode = cd;
    }
  else
    {
      size = bsprintf(buf, "%04d ", cd);
      errcode = 8000;
    }

  c->last_reply = cd;
  va_start(args, msg);
  cnt = bvsnprintf(buf+size, sizeof(buf)-size-1, msg, args);
  va_end(args);
  if (cnt < 0)
    {
      cli_printf(c, errcode, "<line overflow>");
      return;
    }
  size += cnt;
  buf[size++] = '\n';
  memcpy(cli_alloc_out(c, size), buf, size);
}
