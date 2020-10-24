static int racp(src, dst)
     SCM dst, src;
{
  long n = (ARRAY_DIMS(src)->ubnd - ARRAY_DIMS(src)->lbnd + 1);
  long inc_d, inc_s = ARRAY_DIMS(src)->inc;
  sizet i_d, i_s = ARRAY_BASE(src);
  dst = CAR(dst);
  inc_d = ARRAY_DIMS(dst)->inc;
  i_d = ARRAY_BASE(dst);
  src = ARRAY_V(src);
  dst = ARRAY_V(dst);
  switch TYP7(dst) {
  default: gencase: case tc7_vector:
    for (; n-- > 0; i_s += inc_s, i_d += inc_d)
      aset(dst, cvref(src, i_s, UNDEFINED), MAKINUM(i_d));
    break;
  case tc7_string: if (tc7_string != TYP7(src)) goto gencase;
    for (; n-- > 0; i_s += inc_s, i_d += inc_d)
      CHARS(dst)[i_d] = CHARS(src)[i_s];
    break;
  case tc7_Vbool: if (tc7_Vbool != TYP7(src)) goto gencase;
    if (1==inc_d && 1==inc_s && i_s%LONG_BIT==i_d%LONG_BIT && n>=LONG_BIT) {
      long *sv = (long *)VELTS(src);
      long *dv = (long *)VELTS(dst);
      sv += i_s/LONG_BIT;
      dv += i_d/LONG_BIT;
      if (i_s % LONG_BIT) {	/* leading partial word */
	*dv = (*dv & ~(~0L<<(i_s%LONG_BIT))) | (*sv & (~0L<<(i_s%LONG_BIT)));
	dv++;
	sv++;
	n -= LONG_BIT - (i_s % LONG_BIT);
      }
      IVDEP(src != dst,
	    for (; n >= LONG_BIT; n -= LONG_BIT, sv++, dv++)
	      *dv = *sv;)
	if (n)			/* trailing partial word */
	  *dv = (*dv & (~0L<<n)) | (*sv & ~(~0L<<n));
    }
    else {
      for (; n-- > 0; i_s += inc_s, i_d += inc_d)
	if (VELTS(src)[i_s/LONG_BIT] & (1L << (i_s%LONG_BIT)))
	  VELTS(dst)[i_d/LONG_BIT] |= (1L << (i_d%LONG_BIT));
	else
	  VELTS(dst)[i_d/LONG_BIT] &= ~(1L << (i_d%LONG_BIT));
    }
    break;
  case tc7_VfixN32:
  case tc7_VfixZ32: {
    long *d = (long *)VELTS(dst), *s = (long *)VELTS(src);
    if (TYP7(src)==TYP7(dst)) {
      IVDEP(src != dst,
	    for (; n-- > 0; i_s += inc_s, i_d += inc_d)
	      d[i_d] = s[i_s];)
	}
    else if (tc7_VfixZ32==TYP7(dst))
      for (; n-- > 0; i_s += inc_s, i_d += inc_d)
	d[i_d] = num2long(cvref(src, i_s, UNDEFINED),
			  (char *)ARG2, s_array_copy);
    else
      for (; n-- > 0; i_s += inc_s, i_d += inc_d)
	d[i_d] = num2ulong(cvref(src, i_s, UNDEFINED),
			   (char *)ARG2, s_array_copy);
    break;
  }
# ifdef FLOATS
  case tc7_VfloR32: {
    float *d = (float *)VELTS(dst);
    float *s = (float *)VELTS(src);
    switch TYP7(src) {
    default: goto gencase;
    case tc7_VfixZ32: case tc7_VfixN32:
      IVDEP(src != dst,
	    for (; n-- > 0; i_s += inc_s, i_d += inc_d)
	      d[i_d] = ((long *)s)[i_s]; )
	break;
    case tc7_VfloR32:
      IVDEP(src != dst,
	    for (; n-- > 0; i_s += inc_s, i_d += inc_d)
	      d[i_d] = s[i_s]; )
	break;
    case tc7_VfloR64:
      IVDEP(src != dst,
	    for (; n-- > 0; i_s += inc_s, i_d += inc_d)
	      d[i_d] = ((double *)s)[i_s]; )
	break;
    }
    break;
  }
  case tc7_VfloR64: {
    double *d = (double *)VELTS(dst);
    double *s = (double *)VELTS(src);
    switch TYP7(src) {
    default: goto gencase;
    case tc7_VfixZ32: case tc7_VfixN32:
      IVDEP(src != dst,
	    for (; n-- > 0; i_s += inc_s, i_d += inc_d)
	      d[i_d] = ((long *)s)[i_s]; )
	break;
    case tc7_VfloR32:
      IVDEP(src != dst,
	    for (; n-- > 0; i_s += inc_s, i_d += inc_d)
	      d[i_d] = ((float *)s)[i_s];)
	break;
    case tc7_VfloR64:
      IVDEP(src != dst,
	    for (; n-- > 0; i_s += inc_s, i_d += inc_d)
	      d[i_d] = s[i_s];)
	break;
    }
    break;
  }
  case tc7_VfloC32: {
    float (*d)[2] = (float (*)[2])VELTS(dst);
    float (*s)[2] = (float (*)[2])VELTS(src);
    switch TYP7(src) {
    default: goto gencase;
    case tc7_VfixZ32: case tc7_VfixN32:
      IVDEP(src != dst,
	    for (; n-- > 0; i_s += inc_s, i_d += inc_d) {
	      d[i_d][0] = ((long *)s)[i_s];
	      d[i_d][1] = 0.0;
	    })
	break;
    case tc7_VfloR32:
      IVDEP(src != dst,
	    for (; n-- > 0; i_s += inc_s, i_d += inc_d) {
	      d[i_d][0] = ((float *)s)[i_s];
	      d[i_d][1] = 0.0;
	    })
	break;
    case tc7_VfloR64:
      IVDEP(src != dst,
	    for (; n-- > 0; i_s += inc_s, i_d += inc_d) {
	      d[i_d][0] = ((double *)s)[i_s];
	      d[i_d][1] = 0.0;
	    })
	break;
    case tc7_VfloC32:
      IVDEP(src != dst,
	    for (; n-- > 0; i_s += inc_s, i_d += inc_d) {
	      d[i_d][0] = s[i_s][0];
	      d[i_d][1] = s[i_s][1];
	    })
	break;
    case tc7_VfloC64:
      IVDEP(src != dst,
	    for (; n-- > 0; i_s += inc_s, i_d += inc_d) {
	      d[i_d][0] = ((double (*)[2])s)[i_s][0];
	      d[i_d][1] = ((double (*)[2])s)[i_s][1];
	    })
	break;
    }
  }
  case tc7_VfloC64: {
    double (*d)[2] = (double (*)[2])VELTS(dst);
    double (*s)[2] = (double (*)[2])VELTS(src);
    switch TYP7(src) {
    default: goto gencase;
    case tc7_VfixZ32: case tc7_VfixN32:
      IVDEP(src != dst,
	    for (; n-- > 0; i_s += inc_s, i_d += inc_d) {
	      d[i_d][0] = ((long *)s)[i_s];
	      d[i_d][1] = 0.0;
	    })
	break;
    case tc7_VfloR32:
      IVDEP(src != dst,
	    for (; n-- > 0; i_s += inc_s, i_d += inc_d) {
	      d[i_d][0] = ((float *)s)[i_s];
	      d[i_d][1] = 0.0;
	    })
	break;
    case tc7_VfloR64:
      IVDEP(src != dst,
	    for (; n-- > 0; i_s += inc_s, i_d += inc_d) {
	      d[i_d][0] = ((double *)s)[i_s];
	      d[i_d][1] = 0.0;
	    })
	break;
    case tc7_VfloC32:
      IVDEP(src != dst,
	    for (; n-- > 0; i_s += inc_s, i_d += inc_d) {
	      d[i_d][0] = ((float (*)[2])s)[i_s][0];
	      d[i_d][1] = ((float (*)[2])s)[i_s][1];
	    })
	break;
    case tc7_VfloC64:
      IVDEP(src != dst,
	    for (; n-- > 0; i_s += inc_s, i_d += inc_d) {
	      d[i_d][0] = s[i_s][0];
	      d[i_d][1] = s[i_s][1];
	    })
	break;
    }
  }
# endif /* FLOATS */
  }
  return 1;
}
