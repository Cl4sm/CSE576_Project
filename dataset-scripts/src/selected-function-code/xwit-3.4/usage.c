void
usage()
{
	static char Revision[] = "$Revision: 3.4 $";
	char *rbeg, *rend;

	for (rbeg = Revision; *rbeg; rbeg++) {
		if (*rbeg == ' ') {
			break;
		}
	}
	if (*rbeg) {
		for (rend = ++rbeg; *rend; rend++) {
			if (*rend == ' ') {
				*rend = 0;
				break;
			}
		}
		fprintf(stderr, "%s version %s\n\n",
			program_name, rbeg);
	}

	fprintf(stderr,
	"usage: %s -display <display> -sync\n\
	-pop -focus -iconify -unmap -print \n\
	-raise -lower -opposite -[un]circulate\n\
	-resize w h -rows r -columns c -[r]move x y\n\
	-[r]warp x y -colormap <colormapid> -[no]save\n\
	-name <name> -iconname <name> -property <lookfor>\n\
	-bitmap <file> -mask <file> -[r]iconmove x y\n\
	-[no]backingstore -[no]saveunder\n\
	-[no]keyrepeat keycode ... keycode - keycode\n\
	-id <windowid> -root -current -select -all\n\
	-names <initialsubstrings>... [must be last]\n",
		program_name);
	exit(2);
}