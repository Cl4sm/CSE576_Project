void site_sample(glam2_aln *aln, int seq_pick, data *d, double temperature) {
  int strand, end;
  const fasta *f = &d->seqs.f[seq_pick];
  unalign(aln, seq_pick, f);
  get_score_matrices(&d->sm, aln, &d->scorer);
  heat_scores(&d->sm, aln->width, d->alph.size, temperature);
  if (d->a.two_strands)
    copy_scores(&d->sm, aln->width, d->alph.size);
  FORWARD(aln, seq_pick, '+', d, temperature);
  if (d->a.two_strands)
    FORWARD(aln, seq_pick, '-', d, temperature);
  strand = pick_strand(aln, seq_pick, d);
  end = pick_endpoint(&d->sm, aln->width, f->seqlen, strand);
  /*
  puts("Delete scores:");
  print_vec(d->sm.delete_scores, aln->width);
  puts("Insert scores:");
  print_vec(d->sm.insert_scores, aln->width);
  puts("Match scores:");
  print_mat(d->sm.match_scores, aln->width, d->alph.size);
  puts("DP matrix:");
  print_mat(d->sm.dp_mat, aln->width+1, f->seqlen+1);
  printf("strand=%c end=%d\n", strand ? strand : '0', end);
  */
  TRACEBACK(aln, seq_pick, strand, end, d, temperature);
  realign(aln, seq_pick, f);
}