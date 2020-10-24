int
write_uint64(uint64_t val, struct dbFILE *f)
{
  if (fputc((val >> 56) & 0xFF, f->fp) == EOF)
    return -1;
  if (fputc((val >> 48) & 0xFF, f->fp) == EOF)
    return -1;
  if (fputc((val >> 40) & 0xFF, f->fp) == EOF)
    return -1;
  if (fputc((val >> 32) & 0xFF, f->fp) == EOF)
    return -1;
  if (fputc((val >> 24) & 0xFF, f->fp) == EOF)
    return -1;
  if (fputc((val >> 16) & 0xFF, f->fp) == EOF)
    return -1;
  if (fputc((val >>  8) & 0xFF, f->fp) == EOF)
    return -1;
  if (fputc((val)       & 0xFF, f->fp) == EOF)
    return -1;
  return 0;
}
