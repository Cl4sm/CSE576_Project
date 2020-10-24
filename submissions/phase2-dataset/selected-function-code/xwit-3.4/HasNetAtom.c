static Bool HasNetAtom(Display *dpy, Atom atom)
{
	static Atom *atoms;
	static unsigned long n_atoms = -1;
	int i;

	if (n_atoms == -1) {
		Atom type;
		int format;
		unsigned long bytes_after;

		n_atoms = 0;
		XGetWindowProperty (dpy, RootWindow (dpy, screen),
				    XInternAtom(dpy, "_NET_SUPPORTED", True),
				    0, LONG_MAX, False, XA_ATOM, &type, &format,
				    &n_atoms, &bytes_after, (void *)&atoms);

		if (type != XA_ATOM)
			return False;
	}

	if (atoms == NULL)
		return False;

	for (i = 0; i < n_atoms; i++)
		if (atoms[i] == atom)
			return True;
	return False;
}