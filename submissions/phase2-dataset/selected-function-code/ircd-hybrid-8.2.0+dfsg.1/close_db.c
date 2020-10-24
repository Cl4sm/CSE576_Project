int
close_db(struct dbFILE *f)
{
  int res;

  if (!f->fp)
  {
    errno = EINVAL;
    return -1;
  }

  res = fclose(f->fp);
  f->fp = NULL;

  if (res != 0)
    return -1;

  if (f->mode == 'w' && f->tempname[0] && strcmp(f->tempname, f->filename))
  {
    if (rename(f->tempname, f->filename) < 0)
    {
      int errno_save = errno;

      sendto_realops_flags(UMODE_ALL, L_ALL, SEND_NOTICE, "Unable to move new "
                           "data to database file %s; new data NOT saved.",
                           f->filename);
      errno = errno_save;
      ilog(LOG_TYPE_IRCD, "Unable to move new data to database file %s; new "
           "data NOT saved.", f->filename);
      remove(f->tempname);
    }
  }

  MyFree(f);
  return 0;
}
