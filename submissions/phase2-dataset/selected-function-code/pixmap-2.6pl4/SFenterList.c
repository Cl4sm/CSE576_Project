SFenterList(w, n, event)
	Widget				w;
	register int			n;
	register XEnterWindowEvent	*event;
{
	register int	new;

	/* sanity */
	if (SFcurrentInvert[n] != -1) {
		SFinvertEntry(n);
		SFcurrentInvert[n] = -1;
	}

	new = SFnewInvertEntry(n, (XMotionEvent *) event);
	if (new != -1) {
		SFcurrentInvert[n] = new;
		SFinvertEntry(n);
	}
}
