void aln_msf_write(const alignment *a, FILE *stream) {
  static const size_t line_size = 50;
  static const size_t block_size = 10;
  const size_t max_len = aln_max_len(a);
  size_t i, j, k;

  fputs("PileUp\n\n", stream);  /* is this needed? */
  fprintf(stream, "   MSF: %lu Type: X Check: 0 ..\n\n",
	  (unsigned long)max_len);

  for (i = 0; i != a->seq_num; ++i) {
    const aligned_seq *s = &a->seqs[i];
    fprintf(stream, " Name: %-10.10s Len: %lu Check: 0 Weight: 1.00\n",
	    s->name, (unsigned long)strlen(s->seq));
  }

  fputs("\n//\n\n", stream);

  for (i = 0; i < max_len; i += line_size) {  /* theoretical overflow risk */
    for (j = 0; j != a->seq_num; ++j) {
      const aligned_seq *s = &a->seqs[j];
      const size_t len = strlen(s->seq);  /* slow */

      fprintf(stream, "%-10.10s", s->name);

      for (k = 0; k != line_size && i + k < len; ++k) {
	const int c = s->seq[i+k];
	if (k % block_size == 0)
	  putc(' ', stream);
	putc(c, stream);
      }

      putc('\n', stream);
    }
    putc('\n', stream);
  }
}