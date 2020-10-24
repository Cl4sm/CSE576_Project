#ifdef __STDC__
pvm_unpackf(const char *fmt, ...)
#else
pvm_unpackf(va_alist)
	va_dcl
#endif
{
	va_list ap;
	int cc;

#ifdef __STDC__
	va_start(ap, fmt);
#else
	char    *fmt;

	va_start(ap);
	fmt = va_arg(ap, char *);
#endif

	cc = pvm_vunpackf(fmt, ap);
	va_end(ap);

	return cc;
}
