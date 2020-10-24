int
#if defined (__STDC__) || defined(IMA_WIN32_WATCOM)
pvmlogprintf(const char *fmt, ...)
#else
pvmlogprintf(va_alist)
	va_dcl
#endif
{
	va_list ap;
	int cc;

#if defined (__STDC__) || defined(IMA_WIN32_WATCOM)

	va_start(ap, fmt);
#else
	char    *fmt;

	va_start(ap);
	fmt = va_arg(ap, char *);
#endif
	cc = vpvmlogprintf(fmt, ap);
	va_end(ap);

	return cc;
}
