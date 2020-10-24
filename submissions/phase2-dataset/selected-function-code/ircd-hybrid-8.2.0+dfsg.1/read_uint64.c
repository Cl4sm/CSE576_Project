int
read_uint64(uint64_t *ret, struct dbFILE *f)
{
  int64_t c1 = fgetc(f->fp);
  int64_t c2 = fgetc(f->fp);
  int64_t c3 = fgetc(f->fp);
  int64_t c4 = fgetc(f->fp);
  int64_t c5 = fgetc(f->fp);
  int64_t c6 = fgetc(f->fp);
  int64_t c7 = fgetc(f->fp);
  int64_t c8 = fgetc(f->fp);

  if (c1 == EOF || c2 == EOF || c3 == EOF || c4 == EOF ||
      c5 == EOF || c6 == EOF || c7 == EOF || c8 == EOF)
    return -1;

  *ret = c1 << 56 | c2 << 48 | c3 << 40 | c4 << 32 |
         c5 << 24 | c6 << 16 | c7 <<  8 | c8;
  return 0;
}
