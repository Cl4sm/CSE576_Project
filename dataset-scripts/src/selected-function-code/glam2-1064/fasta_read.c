int fasta_read(fasta *f, FILE *fp) {
  char *title;
  int *seq;
  size_t titlen;
  int seqlen;
  register int c;

  /* skip any junk before '>': */
  while ((c = getc(fp)) != EOF)
    if (c == '>')
      break;
  if (ferror(fp))
    die("%s: error reading fasta file: %s\n", prog_name, strerror(errno));
  if (c == EOF)
    return EOF;

  titlen = get_title(&title, fp);
  seqlen = get_seq(&seq, fp);

  f->title = title;
  f->titlen = titlen;
  f->seq = seq;
  f->seqlen = seqlen;
  f->rcseq = NULL;  /* so it can be freed */
  return 0;
}