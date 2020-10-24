    PixmapWidget PW;
{

    if (_PWDEBUG)
	fprintf(stderr, "Scanning... Current: %d\n", PW->pixmap.current);
    if ((PW->pixmap.current > 0) 
	&& 
	(!PW->pixmap.request_stack[PW->pixmap.current--].trap)) {
	PWRemoveRequest((Widget) PW);
	TrappingLoop(PW);
    }
    else
	if (PW->pixmap.cardinal > 0) {
	    if (_PWDEBUG)
		fprintf(stderr, "Trapping... Current: %d\n", PW->pixmap.current+1);
	    if(!Engage(PW, ++PW->pixmap.current))
		PWTerminateRequest((Widget) PW, True);
	}
}
