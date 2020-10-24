static struct dbFILE *
open_db_read(const char *filename)
{
  struct dbFILE *f = MyCalloc(sizeof(*f));
  FILE *fp = NULL;

  strlcpy(f->filename, filename, sizeof(f->filename));

  f->mode = 'r';
  fp = fopen(f->filename, "rb");

  if (!fp)
  {
    int errno_save = errno;

    if (errno != ENOENT)
      ilog(LOG_TYPE_IRCD, "Cannot read database file %s", f->filename);

    MyFree(f);
    errno = errno_save;
    return NULL;
  }

  f->fp = fp;
  return f;
}
