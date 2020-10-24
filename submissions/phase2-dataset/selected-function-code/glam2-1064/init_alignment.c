void init_alignment(alignment *aln, const data *d, int strand) {
  const int width = d->m.width;
  const int *seq = strand == '+' ? d->f.seq : d->f.rcseq;
  const int *hp = d->hit_positions;
  const int *hm = d->hit_matches;
  const int start = hp[0];
  const int end = hp[width-1] + hm[width-1];
  const int deletions = width - sum_int(hm, width);
  const int aln_size = end - start + deletions;
  char *name = xstrdup(d->f.title);
  int *seq1 = xmalloc(aln_size * sizeof(int));
  int *seq2 = xmalloc(aln_size * sizeof(int));
  int aln_pos = 0;
  int seq_pos = start;
  int mot_pos;

  for (mot_pos = 0; mot_pos < width; ++mot_pos) {
    while (seq_pos < hp[mot_pos]) {
      seq2[aln_pos] = seq[seq_pos++];
      seq1[aln_pos++] = 0;
    }
    if (hm[mot_pos] == 1) {  /* match */
      assert(seq[seq_pos] < d->alph.size);  /* ambiguous residue forbidden */
      seq2[aln_pos] = seq[seq_pos++];
    } else  /* deletion */
      seq2[aln_pos] = d->alph.size;
    seq1[aln_pos++] = 1;
  }

  aln->name = name;
  aln->strand = strand;
  aln->coord1 = strand == '+' ? start+1 : d->f.seqlen - start;
  aln->coord2 = strand == '+' ? end : d->f.seqlen - end + 1;
  aln->aln_size = aln_size;
  aln->seq1 = seq1;
  aln->seq2 = seq2;
  aln->score = d->dp_mat[width][end];
}