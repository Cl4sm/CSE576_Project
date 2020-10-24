static void set_selection(frontend *fe, GdkAtom selection)
{
    if (!paste_initialised) {
	compound_text_atom = gdk_atom_intern("COMPOUND_TEXT", FALSE);
	utf8_string_atom = gdk_atom_intern("UTF8_STRING", FALSE);
	paste_initialised = TRUE;
    }

    /*
     * For this simple application we can safely assume that the
     * data passed to this function is pure ASCII, which means we
     * can return precisely the same stuff for types STRING,
     * COMPOUND_TEXT or UTF8_STRING.
     */

    if (gtk_selection_owner_set(fe->area, selection, CurrentTime)) {
	gtk_selection_clear_targets(fe->area, selection);
	gtk_selection_add_target(fe->area, selection,
				 GDK_SELECTION_TYPE_STRING, 1);
	gtk_selection_add_target(fe->area, selection, compound_text_atom, 1);
	gtk_selection_add_target(fe->area, selection, utf8_string_atom, 1);
    }
}