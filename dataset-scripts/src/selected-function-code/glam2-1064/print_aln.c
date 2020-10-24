void print_aln(FILE *fp, glam2_aln *aln, data *d) {
  const int width = aln->width;
  const glam2_col *cols = aln->cols;
  const fasta *seqs = d->seqs.f;
  const int coord_width = digits(d->seqs.maxlen);
  int *max_insert = xmalloc(width * sizeof(int));
  int i, j, k;

  /* get maximum insert size at each position: */
  for (i = 0; i < width; ++i)
    max_insert[i] = max_right_insert(aln, i);

  put_pad(' ', name_width + coord_width + 2, fp);
  for (j = 0; j < width; ++j) {
    putc('*', fp);
    put_pad('.', max_insert[j], fp);
  }
  putc('\n', fp);

  for (i = 0; i < aln->seq_num; ++i) {
    const int strand = aln->strands[i];
    const int *seq = strand == '+' ? seqs[i].seq : seqs[i].rcseq;
    const int start = LEFT_POS(&cols[0], i);  /* zero-based coordinate */
    const int end = RIGHT_POS(&cols[width-1], i);
    const int coord1 = strand == '+' ? start+1 : seqs[i].seqlen - start;
    const int coord2 = strand == '+' ? end : seqs[i].seqlen - end + 1;
    if (!ALIGNED(aln, i))
      continue;

    fprintf(fp, "%-*.*s", name_width, name_width, seqs[i].title);
    fprintf(fp, " %*d ", coord_width, coord1);

    for (j = 0; j < width; ++j) {
      int pos = LEFT_POS(&cols[j], i);
      if (cols[j].matches[i])
	putc(d->alph.decode[seq[pos++]], fp);
      else
	putc('.', fp);

      for (k = 0; k < RIGHT_INSERT(aln, j, i); ++k)
	putc(d->alph.decode[seq[pos++]], fp);
      put_pad('.', max_insert[j] - k, fp);
    }

    fprintf(fp, " %*d ", coord_width, coord2);
    fprintf(fp, "%c ", strand);
    fprintf(fp, "%#.3g\n",
	    marginal_score(&d->scorer, aln, i, &seqs[i]) / xlog(2));
  }

  free(max_insert);
}