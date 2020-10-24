void print_consensus(FILE *fp, const glam2_aln *aln, const data *d) {
  const int width = aln->width;
  const glam2_col *cols = aln->cols;
  const int alph_size = cols->alph_size;
  const int coord_width = digits(d->seqs.maxlen);
  int **counts = xmalloc2(width, alph_size * sizeof(int));
  int *max_insert = xmalloc(width * sizeof(int));
  int i, j;

  for (i = 0; i < width; ++i)
    COPY(counts[i], cols[i].emission_counts, alph_size);

  /* get maximum insert size at each position: */
  for (i = 0; i < width; ++i)
    max_insert[i] = max_right_insert(aln, i);

  for (i = 0; i < alph_size; ++i) {
    int end_flag = 1;
    for (j = 0; j < width; ++j) {
      const int *m = max_int(counts[j], alph_size);
      if (*m * 5 >= cols[j].match_count && *m > 0)
	  end_flag = 0;
    }
    if (end_flag)
      break;

    put_pad(' ', name_width + coord_width + 2, fp);

    for (j = 0; j < width; ++j) {
      int *m = max_int(counts[j], alph_size);
      if (*m * 5 >= cols[j].match_count && *m > 0)
	putc(d->alph.decode[m - counts[j]], fp);
      else
	putc(' ', fp);
      *m = 0;

      put_pad(' ', max_insert[j], fp);
    }

    putc('\n', fp);
  }

  free(max_insert);
  free2(counts, width);
}