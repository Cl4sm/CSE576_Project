ChangeGameCB(Widget w, int index, XtPointer p)
{
    int xsize, ysize;
    int trial;

#if DEBUG
    fprintf(stderr, "ChangeGameCB: %d\n", index);
#endif
    if (index >= NUM_GAME) {
        if (index == NUM_GAME) {
            bd->ToggleClickTrial();
        } else {
            bd->ToggleGravity();
        }
        SetGameStart();
        bd->DrawAll();
    } else {
        mb->CheckGame(index);
        trial = bd->game / NUM_GAME;
        if (index == bd->game % NUM_GAME)
            return;
        index += trial * NUM_GAME;
        GetGameSize(index, xsize, ysize);
        bd->ChangeGame(index, xsize, ysize);
        GetBoardSizeFromGameSize(xsize, ysize, globRes.Width, globRes.Height);
        bd->SetGeometry(globRes.Width, globRes.Height);
        SetGameStart();
    }
    bd->WriteRcFile();
}
