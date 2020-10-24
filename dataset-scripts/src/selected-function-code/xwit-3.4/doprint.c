static void
doprint(Window window)
{
	XWindowAttributes attributes;
	unsigned char *name;

	if( MyFetchName(dpy,window,&name) ) {
		if (XGetWindowAttributes(dpy, window, &attributes) == 0)
			Fatal_Error("XGetWindowAttributes(0x%x)", window);

		printf("0x%x: x=%d y=%d w=%d h=%d d=%d ",
			(int)window,
			attributes.x,attributes.y,
			attributes.width,attributes.height,
			attributes.depth);
		putchar('\'');
		while( *name != '\0' ) {
			if( *name >= ' ' && ((*name)&0x80)== 0 ) {
				putchar(*name);
			} else
				printf("\\%03hho",*name);
			name++;
		}
		putchar('\'');
		putchar('\n');
	}
}