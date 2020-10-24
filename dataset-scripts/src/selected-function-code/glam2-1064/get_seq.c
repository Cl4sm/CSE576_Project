static int get_seq(int **seq, FILE *fp) {
  int len = 0;  /* sequence length so far */
  int mem = sizeof(int);  /* bytes allocated to sequence so far */
  int *buf = xmalloc(mem);  /* sequence */
  register int c;

  while((c = getc(fp)) != EOF) {
    if (c == '>')
      break;
    else if (isspace(c))  /* we might want to read e.g. gap characters */
      continue;
    if (len * sizeof(int) == mem)
      buf = moremem(buf, &mem);
    buf[len] = c;
    ++len;
  }
  if (ferror(fp))
    die("%s: error reading fasta file: %s\n", prog_name, strerror(errno));

  if (c == '>')
    xungetc(c, fp);
  buf = XREALLOC(buf, len);  /* shrink-wrap */
  *seq = buf;  /* might be NULL */
  return len;  /* might be zero */
}