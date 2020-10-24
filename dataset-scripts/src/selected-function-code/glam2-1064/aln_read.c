void aln_read(alignment *a, FILE *stream) {
  char *line = NULL;
  size_t line_size = 0;
  int state = 0;
  a->seq_num = 0;
  a->seqs = NULL;
  a->key_positions = NULL;

  while (xgetline(&line, &line_size, stream)) {
    if (state == 0) {
      char *beg = skipws(line);
      char *end = skipnw(beg);
      if (*beg == '*' && *(end-1) == '*') {
        *end = 0;
        a->key_positions = xstrdup(beg);
        state = 1;
      }
    } else {
      char *name = line;
      char *start = next_word(name);
      char *seq = next_word(start);
      if (*seq == 0)
        break;
      *skipnw(name) = 0;
      *skipnw(start) = 0;
      *skipnw(seq) = 0;
      ++a->seq_num;
      /* slow but simple linear reallocation for now: */
      a->seqs = XREALLOC(a->seqs, a->seq_num);
      a->seqs[a->seq_num-1].name = xstrdup(name);
      a->seqs[a->seq_num-1].seq = xstrdup(seq);
      a->seqs[a->seq_num-1].start = xstrdup(start);
    }
  }

  free(line);
}