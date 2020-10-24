int main(int argc, char **argv) {
  data d;
  FILE *fp;

  prog_name = "glam2scan";  /* for error messages */
  getargs(&d.a, argc, argv);
  init(&d);

  fputs("GLAM2scan\nVersion "
#include "version.h"
	"\n\n", d.out);
  printargs(d.out, argc, argv);
  putc('\n', d.out);

  /*
  print_vec(d.delete_scores, d.m.width);
  print_vec(d.insert_scores, d.m.width);
  print_mat(d.match_scores, d.m.width, d.alph.size);
  */

  fp = xfopen(d.a.seq_file, "r");
  while (fasta_read(&d.f, fp) != EOF) {
    first_word(d.f.title);
    tr_fasta(&d.f, d.alph.encode);
    scan_seq(&d, '+');
    if (d.a.two_strands) {
      rc_fasta(&d.f, d.alph.size);
      scan_seq(&d, '-');
    }
    free_fasta(&d.f);
  }
  xfclose(fp);

  SORT(d.hits, d.hit_num, cmp_alignment);
  print_hits(d.out, d.hits, &d);
  return 0;
}