int main(int argc, char *argv[]) 
{
  int xfd;
  int buttonStatus = -1;
  int i;
  fd_set rfds;
  XEvent Event;

  gtk_init(&argc, &argv);
  parseArguments(argc, argv);
  readHistory();
  
  if (withdrawnMode) {
    runwin();
    exit(0);
  }

  createXBMfromXPM(wminet_mask_bits, bbrun, wminet_mask_width, wminet_mask_height);
  openXwindow(argc, argv, bbrun, wminet_mask_bits, wminet_mask_width, wminet_mask_height);
  AddMouseRegion(0, 0, 0, 16, 16);   // whole area
  xfd = ConnectionNumber(display);

  while (1) {
    RedrawWindow();
    FD_ZERO(&rfds);
    FD_SET(xfd, &rfds);
    select(xfd + 1, &rfds, NULL, NULL, NULL);

    // X Events
    while (XPending(display)) {
      XNextEvent(display, &Event);
      switch (Event.type) {
        case Expose:
          RedrawWindow();
          break;
        case DestroyNotify:
          XCloseDisplay(display);
          exit(0);
          break;
        case ButtonPress:
          buttonStatus = CheckMouseRegion(Event.xbutton.x, Event.xbutton.y);
          break;
        case ButtonRelease:
          i = CheckMouseRegion(Event.xbutton.x, Event.xbutton.y);
          if ((buttonStatus == i) && (buttonStatus == 0)) {
            runwin();
            break;
          }
          buttonStatus = -1;
          RedrawWindow();
          break;
      }
    }
  }
  
  return 0;
}
