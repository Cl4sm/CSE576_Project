static char *
fmtdelta(register time_t t)
{
	register char *cp;
	register int minus;
	static char buf[132];

	minus = 0;
	if (t < 0) {
		t = -t;
		++minus;
	}
	if (t < 60) {
		cp = "second";
	} else if (t < 60 * 60) {
		t /= 60;
		cp = "minute";
	} else if (t < 24 * 60 * 60) {
		t /= (60 * 60);
		cp = "hour";
	} else {
		t /= (24 * 60 * 60);
		cp = "day";
	}
	if (minus)
		t = -t;
	(void)sprintf(buf, "%u %s%s", (u_int32_t)t, cp, PLURAL(t));
	return(buf);
}