void scm_iprin1(exp, port, writing)
     SCM exp;
     SCM port;
     int writing;
{
  register long i;
taloop:
  switch (7 & PTR2INT(exp)) {
  case 2:
  case 6:
    scm_intprint(INUM(exp), 10, port);
    break;
  case 4:
    if (ICHRP(exp)) {
      i = ICHR(exp);
      if (writing) lputs("#\\", port);
      if (!writing) lputc((int)i, port);
      else if ((i <= ' ') && charnames[i]) lputs(charnames[i], port);
#ifndef EBCDIC
      else if (i=='\177')
	lputs(charnames[(sizeof charnames/sizeof(char *))-1], port);
#endif /* ndef EBCDIC */
      else if (i > '\177')
	scm_intprint(i, -8, port);
      else lputc((int)i, port);
    }
    else if (SCM_LINUMP(exp)) {
      lputs("#<line ", port);
      scm_intprint(SCM_LINUM(exp), -10, port);
      lputc('>', port);
    }
    else if (IFLAGP(exp) && (ISYMNUM(exp)<(sizeof isymnames/sizeof(char *))))
      lputs(ISYMCHARS(exp), port);
    else if (ILOCP(exp)) {
      lputs("#@", port);
      scm_intprint((long)IFRAME(exp), -10, port);
      lputc(ICDRP(exp)?'-':'+', port);
      scm_intprint((long)IDIST(exp), -10, port);
    }
    else goto idef;
    break;
  case 1:			/* gloc */
    if (!scm_cell_p(exp-1)) {
      scm_ipruk("gloc", exp, port);
      break;
    }
    lputs("#@", port);
#ifdef _M_ARM
    /* MS CLARM compiler workaround */
    exp = CAR(MS_CLARM_dumy = exp - 1);
#else
    exp = CAR(exp-1);
#endif
    goto taloop;
  default:
  idef:
    scm_ipruk("immediate", exp, port);
    break;
  case 0:
    if (!scm_cell_p(exp)) {
      scm_ipruk("heap", exp, port);
      break;
    }
    switch TYP7(exp) {
    case (127 & IM_LET):
      if (CAR(exp) != IM_LET) {
	lputs("(#@call ", port);
	exp = CDR(exp);
	scm_iprin1(CAR(exp), port, writing);
	scm_iprlist(" ", CAR(CDR(exp)), ')', port, writing);
	break;
      }
      /* else fall through */
    case (127 & IM_AND): case (127 & IM_BEGIN): case (127 & IM_CASE):
    case (127 & IM_COND): case (127 & IM_DO): case (127 & IM_IF):
    case (127 & IM_LAMBDA): case (127 & IM_LETSTAR):
    case (127 & IM_LETREC): case (127 & IM_OR): case (127 & IM_QUOTE):
    case (127 & IM_SET): case (127 & IM_FUNCALL):
    case tcs_cons_inum:
    case tcs_cons_iloc:
    case tcs_cons_chflag:
    case tcs_cons_gloc:
    case tcs_cons_nimcar:
      scm_iprlist("(", exp, ')', port, writing);
      break;
    case tcs_closures:
      scm_princlosure(exp, port, writing);
      break;
    case tc7_string:
      if (writing) {
	lputc('\"', port);
	for (i = 0;i<LENGTH(exp);++i) switch (CHARS(exp)[i]) {
	case '\"':
	case '\\':
	  lputc('\\', port);
	default:
	  lputc(CHARS(exp)[i], port);
	}
	lputc('\"', port);
	break;
      }
    case tcs_symbols:
      if (writing) {		/* slashified symbol */
	for (i = 0;i<LENGTH(exp);++i) switch (CHARS(exp)[i]) {
	case 'A': case 'B': case 'C': case 'D': case 'E':
	case 'F': case 'G': case 'H': case 'I': case 'J':
	case 'K': case 'L': case 'M': case 'N': case 'O':
	case 'P': case 'Q': case 'R': case 'S': case 'T':
	case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
	  if (case_sensitize_symbols) goto skipit;
	case '\\': case '\"': case '\'': case '(': case ')': case '#':
	  lputc('\\', port);
	skipit:
	default:
	  lputc(CHARS(exp)[i], port);
	}
	break;
      }
      else
	lfwrite(CHARS(exp), (sizet)sizeof(char), (sizet)LENGTH(exp), port);
      break;
    case tc7_vector:
      lputs("#(", port);
      for (i = 0;i+1<LENGTH(exp);++i) {
	/* CHECK_INTS; */
	scm_iprin1(VELTS(exp)[i], port, writing);
	lputc(' ', port);
      }
      if (i<LENGTH(exp)) {
	/* CHECK_INTS; */
	scm_iprin1(VELTS(exp)[i], port, writing);
      }
      lputc(')', port);
      break;
    case tc7_Vbool: case tc7_VfixN8: case tc7_VfixZ8:
    case tc7_VfixN16: case tc7_VfixZ16: case tc7_VfixN32: case tc7_VfixZ32:
    case tc7_VfloR32: case tc7_VfloC32: case tc7_VfloR64: case tc7_VfloC64:
      raprin1(exp, port, writing);
      break;
    case tcs_subrs:
      lputs("#<primitive-procedure ", port);
      lputs(SNAME(exp), port);
      lputc('>', port);
      break;
    case tc7_specfun:
#ifdef CCLO
      if (tc16_cclo==TYP16(exp)) {
	lputs("#<compiled-closure ", port);
	scm_iprin1(CCLO_SUBR(exp), port, writing);
	lputc(' ', port);
	scm_iprin1(VELTS(exp)[1], port, writing);
	lputc('>', port);
	break;
      }
#endif
      lputs("#<primitive-procedure ", port);
      lputs(CHARS(CDR(exp)), port);
      lputc('>', port);
      break;
    case tc7_contin:
      lputs("#<continuation ", port);
      scm_intprint(LENGTH(exp), -10, port);
      lputs(" @ ", port);
      scm_intprint((long)CHARS(exp), -16, port);
      lputc('>', port);
      break;
    case tc7_port:
      i = PTOBNUM(exp);
      if (i<numptob) {
	if (ptobs[i].print && (ptobs[i].print)(exp, port, writing))
	  ;
	else
	  prinport(exp, port, ptobs[i].name ? ptobs[i].name : "unknown");
	break;
      }
      goto punk;
    case tc7_smob:
      i = SMOBNUM(exp);
      if (i<numsmob && smobs[i].print && (smobs[i].print)(exp, port, writing))
	break;
      goto punk;
    default: punk: scm_ipruk("type", exp, port);
    }
  }
}
