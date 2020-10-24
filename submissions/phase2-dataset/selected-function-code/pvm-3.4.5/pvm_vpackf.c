pvm_vpackf(fmt, ap)
	char *fmt;
	va_list ap;
{
	char *p = fmt;
	int cc;
	int cnt, std;			/* count and stride of element */
	int vh, vl, vu;			/* short, long, unsigned variety */
	int isv;				/* param is a value not address */
	struct encvec *codef;
	char tc, *cp;			/* each type and a pointer */
	short th, *hp;
	int tn, *np;
	long tl, *lp;
	float tf, *fp;
	double td, *dp;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_PACKF,TEV_EVENT_ENTRY)) {
			TEV_PACK_STRING( TEV_DID_PF, TEV_DATA_SCALAR, fmt, 1, 1 );
			TEV_FIN;
		}
	}

	if (p[0] == '%' && p[1] == '+') {
		cc = va_arg(ap, int);
		if ((cc = pvm_initsend(cc)) < 0)
			goto hork;
		p += 2;
	}

	if (!pvmsbuf) {
		cc = PvmNoBuf;
		goto hork;
	}

	codef = pvmsbuf->m_codef;

	while (*p) {
		if (*p++ == '%') {
			cnt = 1;
			std = 1;
			isv = 1;
			vl = vh = vu = 0;
			if (*p == '*') {
				cnt = va_arg(ap, int);
				isv = 0;
				p++;
			} else
				if (isdigit(*p)) {
					cnt = atoi(p);
					isv = 0;
					while (isdigit(*++p)) ;
				}
			if (*p == '.') {
				isv = 0;
				if (*++p == '*') {
					std = va_arg(ap, int);
					p++;
				} else
					if (isdigit(*p)) {
						std = atoi(p);
						while (isdigit(*++p)) ;
					}
			}
			for (cc = 1; cc; ) {
				switch (*p++) {

				case 'h':
					vh = 1;
					break;

				case 'l':
					vl = 1;
					break;

				case 'u':
					vu = 1;
					break;

				default:
					p--;
					cc = 0;
				}
			}

			switch (*p++) {

			case 'c':
				if (isv) {
					tc = (char) va_arg(ap, int);
					cp = &tc;
				} else
					cp = va_arg(ap, char *);
				cc = (codef->enc_byte) (pvmsbuf, (void*)cp, cnt, std, 1);
#ifdef	DEBUGPACKF
				printf("%d %d %s%schar\n", cnt, std, (vu ? "unsigned " : ""),
						(isv ? "" : "&"));
#else
				vu = vu; /* sgi compiler */
#endif
				break;

			case 'd':
				if (vl) {
					if (isv) {
						tl = va_arg(ap, long);
						lp = &tl;
					} else
						lp = va_arg(ap, long *);
					cc = (codef->enc_long)
							(pvmsbuf, (void*)lp, cnt, std, sizeof(long));
				} else
					if (vh) {
						if (isv) {
							th = (short) va_arg(ap, int);
							hp = &th;
						} else
							hp = va_arg(ap, short *);
						cc = (codef->enc_short)
								(pvmsbuf, (void*)hp, cnt, std, sizeof(short));
					} else {
						if (isv) {
							tn = va_arg(ap, int);
							np = &tn;
						} else
							np = va_arg(ap, int *);
						cc = (codef->enc_int)
								(pvmsbuf, (void*)np, cnt, std, sizeof(int));
					}
#ifdef	DEBUGPACKF
				printf("%d %d %s%s%s%sint\n", cnt, std,
						(vu ? "unsigned " : ""),
						(vl ? "long " : ""),
						(vh ? "short " : ""),
						(isv ? "" : "&"));
#endif
				break;

			case 'f':
				if (vl) {
					if (isv) {
						td = va_arg(ap, double);
						dp = &td;
					} else
						dp = va_arg(ap, double *);
					cc = (codef->enc_double)
							(pvmsbuf, (void*)dp, cnt, std, sizeof(double));
				} else {
					if (isv) {
/*
						tf = va_arg(ap, float);
*/
						td = va_arg(ap, double);
						tf = td;
						fp = &tf;
					} else
						fp = va_arg(ap, float *);
					cc = (codef->enc_float)
							(pvmsbuf, (void*)fp, cnt, std, sizeof(float));
				}
#ifdef	DEBUGPACKF
				printf("%d %d %s%s\n", cnt, std, (isv ? "" : "&"),
						(vl ? "double" : "float"));
#endif
				break;

			case 'x':
				if (vl) {
					dp = va_arg(ap, double *);
					cc = (codef->enc_dcplx)
							(pvmsbuf, (void*)dp, cnt, std, sizeof(double)*2);
				} else {
					fp = va_arg(ap, float *);
					cc = (codef->enc_cplx)
							(pvmsbuf, (void*)fp, cnt, std, sizeof(float)*2);
				}
#ifdef	DEBUGPACKF
				printf("%d %d %s&cplx\n", cnt, std, (vl ? "double " : ""));
#endif
				break;

			case 's':
				if (pvmsbuf->m_enc == 2)	/* oops, can't do this */
					cc = PvmNotImpl;
				else {
					cp = va_arg(ap, char *);
					cnt = strlen(cp) + 1;
					if (!(cc = (codef->enc_int) (pvmsbuf, (void*)&cnt, 1, 1, sizeof(int))))
						cc = (codef->enc_byte) (pvmsbuf, (void*)cp, cnt, 1, 1);
#ifdef	DEBUGPACKF
					printf("string\n");
#endif
				}
				break;

			default:
				cc = PvmBadParam;
				break;
			}

			if (cc)
				goto hork;
		}
/*
		else
			printf("'%c'\n", *(p - 1));
*/
	}
	cc = PvmOk;

hork:
	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_PACKF,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_packf", cc);
	return cc;
}
