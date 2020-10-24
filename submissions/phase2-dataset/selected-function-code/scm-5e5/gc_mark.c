     SCM p;
{
  register long i;
  register SCM ptr = p;
  CHECK_STACK;
 gc_mark_loop:
  if (IMP(ptr)) return;
 gc_mark_nimp:
  if (NCELLP(ptr)
      /* #ifndef RECKLESS */
      /* || PTR_GT(hplims[0], (CELLPTR)ptr) */
      /* || PTR_GE((CELLPTR)ptr, hplims[hplim_ind-1]) */
      /* #endif */
      ) wta(ptr, "rogue pointer in ", s_heap);
  switch TYP7(ptr) {
  case tcs_cons_nimcar:
    if (GCMARKP(ptr)) break;
    SETGCMARK(ptr);
    if (IMP(CDR(ptr))) {	/* IMP works even with a GC mark */
      ptr = CAR(ptr);
      goto gc_mark_nimp;
    }
    gc_mark(CAR(ptr));
    ptr = GCCDR(ptr);
    goto gc_mark_nimp;
  case tcs_cons_imcar:
  case tcs_cons_gloc:
    if (GCMARKP(ptr)) break;
    SETGCMARK(ptr);
    ptr = GCCDR(ptr);
    goto gc_mark_loop;
  case tcs_closures:
    if (GCMARKP(ptr)) break;
    SETGCMARK(ptr);
    if (IMP(GCENV(ptr))) {
      ptr = CODE(ptr);
      goto gc_mark_nimp;
    }
    gc_mark(CODE(ptr));
    ptr = GCENV(ptr);
    goto gc_mark_nimp;
  case tc7_specfun:
    if (GC8MARKP(ptr)) break;
    SETGC8MARK(ptr);
#ifdef CCLO
    if (tc16_cclo==GCTYP16(ptr)) {
      i = CCLO_LENGTH(ptr);
      if (i==0) break;
      while(--i>0) if (NIMP(VELTS(ptr)[i])) gc_mark(VELTS(ptr)[i]);
      ptr = VELTS(ptr)[0];
    }
    else
#endif
      ptr = CDR(ptr);
    goto gc_mark_loop;
  case tc7_vector:
    if (GC8MARKP(ptr)) break;
    SETGC8MARK(ptr);
    i = LENGTH(ptr);
    if (i==0) break;
    while(--i>0) if (NIMP(VELTS(ptr)[i])) gc_mark(VELTS(ptr)[i]);
    ptr = VELTS(ptr)[0];
    goto gc_mark_loop;
  case tc7_contin:
    if (GC8MARKP(ptr)) break;
    SETGC8MARK(ptr);
    mark_locations((STACKITEM *)VELTS(ptr),
		   (sizet)(LENGTH(ptr) +
			   (sizeof(STACKITEM) - 1 + sizeof(CONTINUATION)) /
			   sizeof(STACKITEM)));
    break;
  case tc7_string:
  case tc7_msymbol:
    if (GC8MARKP(ptr)) break;
    ASRTER(!(CHARS(ptr)[HUGE_LENGTH(ptr)]), MAKINUM(HUGE_LENGTH(ptr)),
	   s_wrong_length, s_gc);
  case tc7_ssymbol:
  case tc7_Vbool:
  case tc7_VfixZ32: case tc7_VfixN32:
  case tc7_VfixZ16: case tc7_VfixN16:
  case tc7_VfixN8: case tc7_VfixZ8:
  case tc7_VfloR32: case tc7_VfloC32:
  case tc7_VfloR64: case tc7_VfloC64:
    SETGC8MARK(ptr);
  case tcs_subrs:
    break;
  case tc7_port:
    if (GC8MARKP(ptr)) break;
    SETGC8MARK(ptr);
    i = PTOBNUM(ptr);
    if (!(i < numptob)) goto def;
    mark_port_table(ptr);
    if (!ptobs[i].mark) break;
    ptr = (ptobs[i].mark)(ptr);
    goto gc_mark_loop;
  case tc7_smob:
    if (GC8MARKP(ptr)) break;
    SETGC8MARK(ptr);
    switch TYP16(ptr) {		/* should be faster than going through smobs */
    case tc_free_cell:
      /* printf("found free_cell %X ", ptr); fflush(stdout); */
      ASRTER(tc_broken_heart!=CAR(ptr), ptr, "found ecache forward", s_gc);
      /*      CDR(ptr) = UNDEFINED */;
      break;
#ifdef BIGDIG
    case tcs_bignums:
      break;
#endif
#ifdef FLOATS
    case tc16_flo:
      break;
#endif
    default:
      i = SMOBNUM(ptr);
      if (!(i < numsmob)) goto def;
      SETGC8MARK(ptr);
      if (!smobs[i].mark)  break;
      ptr = (smobs[i].mark)(ptr);
      goto gc_mark_loop;
    }
    break;
  default: def: wta(ptr, s_bad_type, "gc_mark");
  }
}
