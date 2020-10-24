int scm_cell_p(x)
     SCM x;
{
	register int i, j;
	register CELLPTR ptr;
	if (NCELLP(x)) return 0;
	ptr = (CELLPTR)SCM2PTR(x);
	i = 0;
	j = hplim_ind;
	do {
		if (PTR_GT(hplims[i++], ptr)) break;
		if (PTR_LE(hplims[--j], ptr)) break;
		if ((i != j)
		    && PTR_LE(hplims[i++], ptr)
		    && PTR_GT(hplims[--j], ptr)) continue;
		return !0; /* NFREEP(x) */
	} while(i<j);
	if (PTR_LE(scm_ecache, ptr)
	    && PTR_GT(scm_ecache+scm_ecache_len, ptr))
	  return !0; /* so we can print environments while debugging */
	return 0;
}
