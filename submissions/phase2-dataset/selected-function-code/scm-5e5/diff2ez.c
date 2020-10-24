int diff2ez(fp, fpoff, ccrr, a, start_a, end_a, b, start_b, end_b, edits, edx, epo, procs, p_lim)
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
  int len_a = (end_a)-(start_a);
  int len_b = (end_b)-(start_b);
  if (!(p_lim))
    if ((len_b)==(len_a))
      return 0;
    else {
      int T_edx = edx;
      int adx = start_a;
      int bdx = start_b;
      int edx = T_edx;
L_loop:
      if ((bdx)>=(end_b))
	return (len_b)-(len_a);
      else if ((adx)>=(end_a))
	{
	  int T_edx = edx;
	  int idx = bdx;
	  int edx = T_edx;
	  while (!((idx)>=(end_b))) {
	    edits[edx] = (epo)*(1+(idx));
	    {
	      idx = 1+(idx);
	      edx = 1+(edx);
	    }
	  }
	  return (len_b)-(len_a);
	}
      else if (procs->array_refsEql_P(a, adx, 0, b, bdx, 0))
	{
	  adx = 1+(adx);
	  bdx = 1+(bdx);
	  goto L_loop;
	}
      else {
	edits[edx] = (epo)*(1+(bdx));
	{
	  bdx = 1+(bdx);
	  edx = 1+(edx);
	  goto L_loop;
	}
      }
    }
  else if ((len_a)<=(p_lim))
    {
      int idx = start_a;
      int jdx = start_b;
      while (!(((idx)>=(end_a))
	     && ((jdx)>=(end_b)))) {
	if ((jdx)<(end_b))
	  {
	    edits[edx] = (epo)*(1+(jdx));
	    edx = 1+(edx);
	  }
	if ((idx)<(end_a))
	  {
	    edits[edx] = (epo)*( -1-(idx));
	    edx = 1+(edx);
	  }
	{
	  idx = 1+(idx);
	  jdx = 1+(jdx);
	}
      }
      return (len_a)+(len_b);
    }
  else return diff_divide_and_conquer(fp, fpoff, ccrr, a, start_a, end_a, b, start_b, end_b, edits, edx, epo, procs, p_lim);
}
