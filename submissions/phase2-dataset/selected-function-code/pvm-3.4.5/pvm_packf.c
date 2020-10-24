int
#ifdef __STDC__
pvm_packf(const char *fmt, ...)
#else
pvm_packf(va_alist)
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

	cc = pvm_vpackf(fmt, ap);
	va_end(ap);

	return cc;
}
