     SCM vect, len;
{
  long ol, l = INUM(len);
  sizet siz, sz;
  ASRTGO(NIMP(vect), badarg1);
  ol = LENGTH(vect);
  switch TYP7(vect) {
  default: badarg1: wta(vect, (char *)ARG1, s_resizuve);
  case tc7_string:
    ASRTGO(vect != nullstr, badarg1);
    sz = sizeof(char);
    ol++;
    l++;
    break;
  case tc7_vector:
    ASRTGO(vect != nullvect, badarg1);
    sz = sizeof(SCM);
    break;
#ifdef ARRAYS
  case tc7_Vbool:
    ol = (ol+LONG_BIT-1)/LONG_BIT;
    l = (l+LONG_BIT-1)/LONG_BIT;
  case tc7_VfixN32:
  case tc7_VfixZ32:
    sz = sizeof(long);
    break;
  case tc7_VfixN16:
  case tc7_VfixZ16:
    sz = sizeof(short);
    break;
  case tc7_VfixN8:
  case tc7_VfixZ8:
    sz = sizeof(char);
    break;
# ifdef FLOATS
  case tc7_VfloR32:
    sz = sizeof(float);
    break;
  case tc7_VfloC32:
    sz = 2*sizeof(float);
    break;
  case tc7_VfloR64:
    sz = sizeof(double);
    break;
  case tc7_VfloC64:
    sz = 2*sizeof(double);
    break;
# endif
#endif
  }
  ASRTER(INUMP(len), len, ARG2, s_resizuve);
  if (!l) l = 1L;
  siz = l * sz;
  if (siz != l * sz) wta(MAKINUM(l * sz), (char *) NALLOC, s_resizuve);
  DEFER_INTS;
  must_realloc_cell(vect, ol*sz, (long)siz, s_resizuve);
  if (VECTORP(vect))
    while(l > ol)
      VELTS(vect)[--l] = UNSPECIFIED;
  else if (STRINGP(vect))
    CHARS(vect)[l-1] = 0;
  SETLENGTH(vect, INUM(len), TYP7(vect));
  ALLOW_INTS;
  return vect;
}
