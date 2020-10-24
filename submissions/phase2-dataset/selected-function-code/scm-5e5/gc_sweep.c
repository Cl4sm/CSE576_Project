static void gc_sweep(contin_bad)
     int contin_bad;
{
  register CELLPTR ptr;
#ifdef POINTERS_MUNGED
  register SCM scmptr;
#else
# define scmptr (SCM)ptr
#endif
  register SCM nfreelist = EOL;
  register long n = 0;
  register sizet j, minc;
  long pre_m = mallocated;
  sizet i = 0;
  sizet seg_cells;
  while (i < hplim_ind) {
    ptr = CELL_UP(hplims[i++]);
    seg_cells = CELL_DN(hplims[i++]) - ptr;
    for (j = seg_cells; j--; ++ptr) {
#ifdef POINTERS_MUNGED
      scmptr = PTR2SCM(ptr);
#endif
      switch TYP7(scmptr) {
      case tcs_cons_imcar:
      case tcs_cons_nimcar:
      case tcs_cons_gloc:
      case tcs_closures:
	if (GCMARKP(scmptr)) goto cmrkcontinue;
	break;
      case tc7_specfun:
	if (GC8MARKP(scmptr)) goto c8mrkcontinue;
#ifdef CCLO
	if (tc16_cclo==GCTYP16(scmptr)) {
	  minc = (CCLO_LENGTH(scmptr)*sizeof(SCM));
	  goto freechars;
	}
#endif
	break;
      case tc7_vector:
	if (GC8MARKP(scmptr)) goto c8mrkcontinue;
	minc = (LENGTH(scmptr)*sizeof(SCM));
      freechars:
	must_free(CHARS(scmptr), minc);
/*	SETCHARS(scmptr, 0);*/
	break;
      case tc7_Vbool:
	if (GC8MARKP(scmptr)) goto c8mrkcontinue;
	minc = sizeof(long)*((HUGE_LENGTH(scmptr)+LONG_BIT-1)/LONG_BIT);
	goto freechars;
      case tc7_VfixZ32:
      case tc7_VfixN32:
	if (GC8MARKP(scmptr)) goto c8mrkcontinue;
	minc = HUGE_LENGTH(scmptr)*sizeof(long);
	goto freechars;
      case tc7_VfixN8:
      case tc7_VfixZ8:
	if (GC8MARKP(scmptr)) goto c8mrkcontinue;
	minc = HUGE_LENGTH(scmptr)*sizeof(char);
	goto freechars;
      case tc7_VfixZ16:
      case tc7_VfixN16:
	if (GC8MARKP(scmptr)) goto c8mrkcontinue;
	minc = HUGE_LENGTH(scmptr)*sizeof(short);
	goto freechars;
      case tc7_VfloR32:
	if (GC8MARKP(scmptr)) goto c8mrkcontinue;
	minc = HUGE_LENGTH(scmptr)*sizeof(float);
	goto freechars;
      case tc7_VfloC32:
	if (GC8MARKP(scmptr)) goto c8mrkcontinue;
	minc = HUGE_LENGTH(scmptr)*2*sizeof(float);
	goto freechars;
      case tc7_VfloR64:
	if (GC8MARKP(scmptr)) goto c8mrkcontinue;
	minc = HUGE_LENGTH(scmptr)*sizeof(double);
	goto freechars;
      case tc7_VfloC64:
	if (GC8MARKP(scmptr)) goto c8mrkcontinue;
	minc = HUGE_LENGTH(scmptr)*2*sizeof(double);
	goto freechars;
      case tc7_string:
	if (GC8MARKP(scmptr)) goto c8mrkcontinue;
	minc = HUGE_LENGTH(scmptr)+1;
	goto freechars;
      case tc7_msymbol:
	if (GC8MARKP(scmptr)) goto c8mrkcontinue;
	minc = LENGTH(scmptr)+1;
	goto freechars;
      case tc7_contin:
	if (GC8MARKP(scmptr)) {
	  if (contin_bad && CONT(scmptr)->length)
	    scm_warn("uncollected ", "", scmptr);
	  goto c8mrkcontinue;
	}
	minc = LENGTH(scmptr)*sizeof(STACKITEM) + sizeof(CONTINUATION);
	mallocated = mallocated - minc;
	free_continuation(CONT(scmptr)); break; /* goto freechars; */
      case tc7_ssymbol:
	if (GC8MARKP(scmptr)) goto c8mrkcontinue;
	/* Do not free storage because tc7_ssymbol means scmptr's
           storage was not created by a call to malloc(). */
	break;
      case tcs_subrs:
	continue;
      case tc7_port:
	if (GC8MARKP(scmptr)) goto c8mrkcontinue;
	if (OPENP(scmptr)) {
	  int k = PTOBNUM(scmptr);
	  if (!(k < numptob)) goto sweeperr;
				/* Yes, I really do mean ptobs[k].free */
				/* rather than ptobs[k].close.  .close */
				/* is for explicit CLOSE-PORT by user */
	  (ptobs[k].free)(STREAM(scmptr));
	  gc_ports_collected++;
	  SETSTREAM(scmptr, 0);
	  CAR(scmptr) &= ~OPN;
	}
	break;
      case tc7_smob:
	switch GCTYP16(scmptr) {
	case tc_free_cell:
	  if (GC8MARKP(scmptr)) goto c8mrkcontinue;
	  break;
#ifdef BIGDIG
	case tcs_bignums:
	  if (GC8MARKP(scmptr)) goto c8mrkcontinue;
	  minc = (NUMDIGS(scmptr)*sizeof(BIGDIG));
	  goto freechars;
#endif /* def BIGDIG */
#ifdef FLOATS
	case tc16_flo:
	  if (GC8MARKP(scmptr)) goto c8mrkcontinue;
	  switch ((int)(CAR(scmptr)>>16)) {
	  case (IMAG_PART | REAL_PART)>>16:
	    minc = 2*sizeof(double);
	    goto freechars;
	  case REAL_PART>>16:
	  case IMAG_PART>>16:
	    minc = sizeof(double);
	    goto freechars;
	  case 0:
	    break;
	  default:
	    goto sweeperr;
	  }
	  break;
#endif /* def FLOATS */
	default:
	  if (GC8MARKP(scmptr)) goto c8mrkcontinue;
	  {
	    int k = SMOBNUM(scmptr);
	    if (!(k < numsmob)) goto sweeperr;
	    minc = (smobs[k].free)((CELLPTR)scmptr);
	  }
	}
	break;
      default: sweeperr: wta(scmptr, s_bad_type, "gc_sweep");
      }
      ++n;
      CAR(scmptr) = (SCM)tc_free_cell;
      CDR(scmptr) = nfreelist;
      nfreelist = scmptr;
      continue;
    c8mrkcontinue:
      CLRGC8MARK(scmptr);
      continue;
    cmrkcontinue:
      CLRGCMARK(scmptr);
    }
#ifdef GC_FREE_SEGMENTS
    if (n==seg_cells) {
      heap_cells -= seg_cells;
      n = 0;
      free((char *)hplims[i-2]);
      /*      must_free((char *)hplims[i-2],
		sizeof(cell) * (hplims[i-1] - hplims[i-2])); */
      hplims[i-2] = 0;
      for (j = i;j < hplim_ind;j++) hplims[j-2] = hplims[j];
      hplim_ind -= 2;
      i -= 2;			/* need to scan segment just moved. */
      nfreelist = freelist;
    }
    else
#endif /* ifdef GC_FREE_SEGMENTS */
	freelist = nfreelist;
    gc_cells_collected += n;
    n = 0;
  }
  lcells_allocated += (heap_cells - gc_cells_collected - cells_allocated);
  cells_allocated = (heap_cells - gc_cells_collected);
  gc_malloc_collected = (pre_m - mallocated);
  lmallocated = lmallocated - gc_malloc_collected;
}
