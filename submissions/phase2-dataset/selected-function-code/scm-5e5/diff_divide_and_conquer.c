int diff_divide_and_conquer(fp, fpoff, ccrr, a, start_a, end_a, b, start_b, end_b, edits, edx, epo, procs, p_lim)
     int *fp;
     int fpoff;
     int *ccrr;
     void *a;
     int start_a;
     int end_a;
     void *b;
     int start_b;
     int end_b;
     int *edits;
     int edx;
     int epo;
     fp_procs *procs;
     int p_lim;
{
  int mid_a = ((start_a)+(end_a))/2;
  int len_b = (end_b)-(start_b);
  int len_a = (end_a)-(start_a);
  {
    int tcst = (p_lim)+(p_lim)+((len_b)-(len_a));
    int *cc = &(ccrr[0]);
    int *rr = &(ccrr[(len_b)+1]);
    int m2 = (end_a)-(mid_a);
    int m1 = (mid_a)-(start_a);
    fp_init(cc, 0, (len_a)+(len_b), 0, len_b);
    fp_init(fp, fpoff,  -1, -(1+(p_lim)), 1+(p_lim)+((len_b)-(m1)));
    fp_compare(fp, fpoff, cc, procs->subarray(a, start_a, mid_a), m1, procs->subarray(b, start_b, end_b), len_b, procs->array_refsEql_P, MIN(p_lim, len_a));
    fp_init(rr, 0, (len_a)+(len_b), 0, len_b);
    fp_init(fp, fpoff,  -1, -(1+(p_lim)), 1+(p_lim)+((len_b)-(m2)));
    fp_compare(fp, fpoff, rr, procs->subarray(a, mid_a, end_a), m2, procs->subarray(b, start_b, end_b), len_b, procs->array_refs_revEql_P, MIN(p_lim, len_a));
    {
      int b_splt = diff_mid_split(len_b, rr, cc, tcst);
      int est_c = cc[b_splt];
      int est_r = rr[(len_b)-(b_splt)];
      check_cost("cc", est_c, diff2et(fp, fpoff, ccrr, a, start_a, mid_a, b, start_b, (start_b)+(b_splt), edits, edx, epo, procs, ((est_c)-((b_splt)-((mid_a)-(start_a))))/2));
      check_cost("rr", est_r, diff2et(fp, fpoff, ccrr, a, mid_a, end_a, b, (start_b)+(b_splt), end_b, edits, (est_c)+(edx), epo, procs, ((est_r)-(((len_b)-(b_splt))-((end_a)-(mid_a))))/2));
      return (est_c)+(est_r);
    }
  }
}
