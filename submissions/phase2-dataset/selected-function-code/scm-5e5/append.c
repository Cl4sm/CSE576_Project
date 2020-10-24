SCM append(args)
     SCM args;
{
	SCM res = EOL;
	SCM *lloc = &res, arg;
	if (IMP(args)) {
		ASRTER(NULLP(args), args, ARGn, s_append);
		return res;
		}
	ASRTER(CONSP(args), args, ARGn, s_append);
	while (1) {
		arg = CAR(args);
		args = CDR(args);
		if (IMP(args)) {
			*lloc = arg;
			ASRTER(NULLP(args), args, ARGn, s_append);
			return res;
		}
		ASRTER(CONSP(args), args, ARGn, s_append);
		for (;NIMP(arg);arg = CDR(arg)) {
			ASRTER(CONSP(arg), arg, ARGn, s_append);
			*lloc = cons(CAR(arg), EOL);
			lloc = &CDR(*lloc);
		}
		ASRTER(NULLP(arg), arg, ARGn, s_append);
	}
}
