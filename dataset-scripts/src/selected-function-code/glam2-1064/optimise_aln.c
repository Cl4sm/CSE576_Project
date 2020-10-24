void optimise_aln(glam2_aln *best, data *d) {
  glam2_aln *aln = &d->aln;
  int no_improvement = 0;
  int i;

  aln_copy(aln, best);
  if (d->a.profile)
    fputs("Temperature, Columns, Sequences, Score:\n", d->out);

  for (i = 0; no_improvement < d->a.stop_after; ++i) {
    double temperature = d->a.temperature /
      xpow(d->a.cool, (double)i / d->a.stop_after);
    if (temperature < d->a.frozen)
      temperature = d->a.frozen;
    if (d->a.profile)
      fprintf(d->out, "%g\t%d\t%d\t%g\n",
	      temperature, aln->width, aln->aligned_seq, aln->score / xlog(2));
    /*
    print_aln(d->out, aln, d);
    */
    update_aln(aln, d, temperature);
    if (aln->score > best->score) {
      aln_copy(best, aln);
      no_improvement = 0;
    } else
      ++no_improvement;
  }

  if (d->a.profile)
    putc('\n', d->out);
  fprintf(stderr, "%d iterations\n", i);
}