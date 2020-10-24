void
ChangePixmapCB(Widget w, int index, XtPointer p)
{
    char subdir[16];
extern Widget toplevel;

#if DEBUG
    fprintf(stderr, "ChangePixmapCB: %d\n", index);
#endif
    mb->CheckPixmap(index);
    if (globRes.imageSet != index + 1) {
        int xsize, ysize;
	globRes.imageSet = index + 1;
        InitPicture();
        GetGameSize(bd->game%NUM_GAME, xsize, ysize);
        GetBoardSizeFromGameSize(xsize, ysize, globRes.Width, globRes.Height);
        bd->SetGeometry(globRes.Width, globRes.Height);
        bd->WriteRcFile();
        SetGameStart();
    }
}
