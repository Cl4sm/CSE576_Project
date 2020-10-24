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
  {
    int bdx =  -1+(end_b);
    int adx =  -1+(end_a);
    while (((start_b)<=(bdx))
	 && ((start_a)<=(adx))
	 && (procs->array_refsEql_P(a, adx, 0, b, bdx, 0))) {
      {
	bdx =  -1+(bdx);
	adx =  -1+(adx);
      }
    }
    {
      int bsx = start_b;
      int asx = start_a;
      while (((bsx)<(bdx))
	   && ((asx)<(adx))
	   && (procs->array_refsEql_P(a, asx, 0, b, bsx, 0))) {
	{
	  bsx = 1+(bsx);
	  asx = 1+(asx);
	}
      }
      {
	int delta = ((bdx)-(bsx))-((adx)-(asx));
	if (0 > (delta))
	  return diff2ez(fp, fpoff, ccrr, b, bsx, 1+(bdx), a, asx, 1+(adx), edits, edx, -(epo), procs, (delta)+(p_lim));
	else return diff2ez(fp, fpoff, ccrr, a, asx, 1+(adx), b, bsx, 1+(bdx), edits, edx, epo, procs, p_lim);
      }
    }
  }
}
