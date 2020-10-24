pvm_vunpackf(fmt, ap)
	char *fmt;
	va_list ap;
{
	char *p = fmt;
	int cc;
	int cnt, std;		/* count and stride of element */
	int vh, vl, vu;		/* short, long, unsigned variety */
	struct encvec *codef;
	char *cp;			/* pointers for each type */
	short *hp;
	int *np;
	long *lp;
	float *fp;
	double *dp;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_UNPACKF,TEV_EVENT_ENTRY)) {
			TEV_PACK_STRING( TEV_DID_PF, TEV_DATA_SCALAR, fmt, 1, 1 );
			TEV_FIN;
		}
	}

	if (!pvmrbuf) {
		cc = PvmNoBuf;
		goto hork;
	}

	codef = pvmrbuf->m_codef;

	while (*p) {
		if (*p++ == '%') {
			cnt = 1;
			std = 1;
			vl = vh = vu = 0;
			if (*p == '*') {
				cnt = va_arg(ap, int);
				p++;
			} else
				if (isdigit(*p)) {
					cnt = atoi(p);
					while (isdigit(*++p)) ;
				}
			if (*p == '.') {
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
				cp = va_arg(ap, char *);
				cc = (codef->dec_byte) (pvmrbuf, (void*)cp, cnt, std, 1);
				break;

			case 'd':
				if (vl) {
					lp = va_arg(ap, long *);
					cc = (vu ? codef->dec_ulong : codef->dec_long)
							(pvmrbuf, (void*)lp, cnt, std, sizeof(long));
				} else
					if (vh) {
						hp = va_arg(ap, short *);
						cc = (vu ? codef->dec_ushort : codef->dec_short)
								(pvmrbuf, (void*)hp, cnt, std, sizeof(short));
					} else {
						np = va_arg(ap, int *);
						cc = (vu ? codef->dec_uint : codef->dec_int)
								(pvmrbuf, (void*)np, cnt, std, sizeof(int));
					}
				break;

			case 'f':
				if (vl) {
					dp = va_arg(ap, double *);
					cc = (codef->dec_double)
							(pvmrbuf, (void*)dp, cnt, std, sizeof(double));
				} else {
					fp = va_arg(ap, float *);
					cc = (codef->dec_float)
							(pvmrbuf, (void*)fp, cnt, std, sizeof(float));
				}
				break;

			case 'x':
				if (vl) {
					dp = va_arg(ap, double *);
					cc = (codef->dec_dcplx)
							(pvmrbuf, (void*)dp, cnt, std, sizeof(double)*2);
				} else {
					fp = va_arg(ap, float *);
					cc = (codef->dec_cplx)
							(pvmrbuf, (void*)fp, cnt, std, sizeof(float)*2);
				}
				break;

			case 's':
				cp = va_arg(ap, char *);
				if (!(cc = (codef->dec_int) (pvmrbuf, (void*)&cnt, 1, 1, sizeof(int))))
					cc = (codef->dec_byte) (pvmrbuf, (void*)cp, cnt, 1, 1);
				break;

			default:
				cc = PvmBadParam;
				break;
			}

			if (cc)
				goto hork;
		}
	}
	cc = PvmOk;

hork:
	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_UNPACKF,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_unpackf", cc);
	return cc;
}
