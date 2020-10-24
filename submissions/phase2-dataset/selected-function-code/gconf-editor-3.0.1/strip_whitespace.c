static char *
strip_whitespace (const char *text)
{
	const char *p;
	const char *end;
	GString *str;

	p = text;
	end = text + strlen (text);

	/* First skip the leading whitespace */
	while (p != end && g_unichar_isspace (g_utf8_get_char (p))) {
	  p = g_utf8_next_char (p);
	}
	  
	str = g_string_new (NULL);
	
	while (p != end) {
		gunichar ch;

		ch = g_utf8_get_char (p);

		if (g_unichar_isspace (ch)) {
			while (p != end && g_unichar_isspace (ch)) {
				p = g_utf8_next_char (p);
				ch = g_utf8_get_char (p);
			}

			p = g_utf8_prev_char (p);
			g_string_append_unichar (str, ' ');
		}
		else {
			g_string_append_unichar (str, ch);
		}

		p = g_utf8_next_char (p);
	}

	return g_string_free (str, FALSE);
}