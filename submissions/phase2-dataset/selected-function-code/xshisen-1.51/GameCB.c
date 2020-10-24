GameCB(Widget w, int index)
{
    static long pausedTime;
    int pauseStatus;
#if DEBUG
    fprintf(stderr, "GameCB: %d\n", index);
#endif
    switch(index) {
    case 0: /* Start */
        bd->LogGiveUp();
        bd->Shuffle();
        bd->DrawAll();
        mb->Sensitive();
        tm->ResetTimer();
        break;
    case 1: /* Restart */
        bd->LogGiveUp();
        bd->Restart();
        bd->DrawAll();
        mb->Sensitive();
        tm->ResetTimer();
        break;
    case 2: /* Pause */
        pauseStatus = bd->TogglePause();
        if (pauseStatus == 1) { // Paused
            mb->PauseOn();
            pausedTime = tm->GetTimer();
        } else if (pauseStatus == 0) {  // Pause cancelled
            mb->PauseOff();
            tm->SetTimer(pausedTime);
        }
        break;
    case 3: /* Score */
        sc->DisplayScore(bd->game);
        break;
    case 4: /* Personal Score */
        sc->PersonalStat(bd->game);
        break;
    case 5: /* About */
        Message(MESSAGE_SYMBOL, globRes.aboutString);
        break;
    case 6: /* Exit */
        exit(0);
    }
}
