static struct dbFILE *
open_db_write(const char *filename, uint32_t version)
{
  struct dbFILE *f = MyCalloc(sizeof(*f));
  int fd = 0;

  strlcpy(f->filename, filename, sizeof(f->filename));

  filename = f->filename;
  f->mode = 'w';

  snprintf(f->tempname, sizeof(f->tempname), "%s.new", filename);

  if (f->tempname[0] == '\0' || !strcmp(f->tempname, filename))
  {
    ilog(LOG_TYPE_IRCD, "Opening database file %s for write: Filename too long",
         filename);
    MyFree(f);
    errno = ENAMETOOLONG;
    return NULL;
  }

  remove(f->tempname);

  /* Use open() to avoid people sneaking a new file in under us */
  fd = open(f->tempname, O_WRONLY | O_CREAT | O_EXCL, 0666);
  if (fd >= 0)
    f->fp = fdopen(fd, "wb");

  if (!f->fp || !write_file_version(f, version))
  {
    int errno_save = errno;
    static int walloped = 0;

    if (!walloped++)
      sendto_realops_flags(UMODE_ALL, L_ALL, SEND_NOTICE,
                           "Cannot create temporary database file %s",
                           f->tempname);

    errno = errno_save;
    ilog(LOG_TYPE_IRCD, "Cannot create temporary database file %s",
        f->tempname);

    if (f->fp)
      fclose(f->fp);

    remove(f->tempname);
    MyFree(f);

    errno = errno_save;
    return NULL;
  }

  return f;
}
