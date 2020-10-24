uint32_t
get_file_version(struct dbFILE *f)
{
  uint32_t version = 0;

  if (read_uint32(&version, f) == -1)
  {
    ilog(LOG_TYPE_IRCD, "Error reading version number on %s: %s",
         f->filename, strerror(errno));
    return 0;
  }

  if (version < 1)
  {
    ilog(LOG_TYPE_IRCD, "Invalid version number (%u) on %s",
         version, f->filename);
    return 0;
  }

  return version;
}
