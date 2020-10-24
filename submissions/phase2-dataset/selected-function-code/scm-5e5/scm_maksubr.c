     const char *name;
     int type;
     SCM (*fcn)();
{
	subr_info info;
	int isubr;
	register SCM z;
	info.name = name;
	for (isubr = subrs_gra.len; 0 < isubr--;) {
	  if (0==strcmp(((char **)subrs_gra.elts)[isubr], name)) {
	    scm_warn(s_redefining, (char *)name, UNDEFINED);
	    goto foundit;
	  }
	}
	isubr = scm_grow_gra(&subrs_gra, (char *)&info);
 foundit:
	NEWCELL(z);
	if (!fcn && tc7_cxr==type) {
	  const char *p = name;
	  int code = 0;
	  while (*++p != 'r')
	    switch (*p) {
	    default: wta(UNDEFINED, "bad cxr name", (char *)name);
	    case 'a': code = (code<<2) + 1; continue;
	    case 'd': code = (code<<2) + 2; continue;
	    }
	  type += (code << 8);
	}
	CAR(z) = (isubr<<16) + type;
	SUBRF(z) = fcn;
	return z;
}
