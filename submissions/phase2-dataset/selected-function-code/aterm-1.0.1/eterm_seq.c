	void
eterm_seq(int op, const char *str)
{
	assert(str != NULL);
	switch (op) {
#if defined(BACKGROUND_IMAGE) || defined(TRANSPARENT)
		case 2:
			/* { shade | tint } [ class ; ] value */
			if (strncmp(str, "tint", 4) == 0) {
				char *class;
				char *value;

				if (!(class = strchr((char *) str, ';')))
					break;
				*class++ = '\0';
				if (!(value = strchr(class, ';'))) {
					/* Default class is bg. */
					value = class;
					class = "bg";
				} else
					*value++ = '\0';

				if (strcmp(class, "bg") == 0) {
					set_window_color(Color_tint, value);
				}
			}
			break;
#endif
	}
}
