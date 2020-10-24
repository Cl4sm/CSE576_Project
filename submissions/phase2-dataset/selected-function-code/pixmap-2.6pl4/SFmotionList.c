void
SFmotionList(w, n, event)
	Widget			w;
	register int		n;
	register XMotionEvent	*event;
{
	register int	new;

	new = SFnewInvertEntry(n, event);

	if (new != SFcurrentInvert[n]) {
		if (SFcurrentInvert[n] != -1) {
			SFinvertEntry(n);
		}
		SFcurrentInvert[n] = new;
		if (new != -1) {
			SFinvertEntry(n);
		}
	}
}
