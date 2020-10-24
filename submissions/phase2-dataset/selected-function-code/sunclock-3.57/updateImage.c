updateImage(Context)
struct Sundata * Context;
{
        int                     noon;
        double                  fnoon;
	double junk;

        /* If this is a full repaint of the window, force complete
           recalculation. */

        if (button_pressed) return;

        time(&Context->time);

	erase_obj = 1;
	if (Context->flags.colorlevel == MONOCHROME ||
            Context->flags.colorlevel == FULLCOLORS)
            drawSunAndMoon(Context);
        erase_obj = 0;

        (void) sunParams(Context->time + Context->jump, 
              &Context->sunlon, &Context->sundec, NULL);

        (void) phase(Context->time + Context->jump, 
              &Context->moondec, &Context->moonlon, 
              &junk,  &junk, &junk, &junk, &junk, &junk );
	Context->moonlon = fixangle(Context->moonlon+180.0) - 180.0;

        fnoon = Context->sunlon * (Context->zoom.width / 360.0) 
                         - (double) Context->zoom.dx;
        noon = (int) fnoon;
        Context->sunlon -= 180.0;

        /* Projecting the illumination curve  for the current seasonal
           instant is costly.  If we're running in real time, only  do
           it every PROJINT seconds.
           If the subsolar point has moved at least one pixel, also 
           update the illuminated area on the screen.   */

        if (Context->projtime < 0 || 
            (Context->time - Context->projtime) > PROJINT ||
            Context->noon != noon || Context->flags.update>=4) {
                Context->flags.update = 2;
                Context->projtime = Context->time;
                Context->noon = noon;
                Context->fnoon = fnoon;
                moveNightArea(Context);
		if (Context->flags.colorlevel==FULLCOLORS) {
		   drawAll(Context);
		   drawCities(Context);
		}
        }

        drawSunAndMoon(Context);
}
