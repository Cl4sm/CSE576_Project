void
doExpose(w)
Window w;
{
        struct Sundata * Context;

        Context = getContext(w);
        if (!Context) return;

        if (w == Menu) { setupMenu(-1); return; }
        if (w == Filesel) { setupFilesel(-1); return; }
        if (w == Zoom) { setupZoom(-1); return; }
        if (w == Option) { setupOption(-1); return; }
        if (w == Urban) { setupUrban(-1); return; }

        Context->flags.update = 2;
        showMapImage(Context);

	Context->flags.bottom &= 1;
	drawBottomline(Context);

	clearStrip(Context);
	Context->flags.hours_shown = 0;
        writeStrip(Context);
}
