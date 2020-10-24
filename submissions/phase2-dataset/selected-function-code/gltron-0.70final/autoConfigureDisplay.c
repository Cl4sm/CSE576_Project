static void autoConfigureDisplay(void) {
  int n_humans = 0;
  int i;
  int vp;

  defaultViewportPositions();

  /* loop thru players and find the humans */
  for (i=0; i < game->players; i++) {
    if (game->player[i].ai->active == AI_HUMAN) {
      viewport_content[n_humans] = i;
      n_humans++;
    }    
  }
 
  switch(n_humans) {
    case 0 :
      /*
         Not sure what the default should be for
         a game without human players. For now 
         just show a single viewport.
       */
      /* fall thru */
    case 1 :
      vp = VP_SINGLE;
      break;
    case 2 :
      vp = VP_SPLIT;
      break;
    default :
      defaultViewportPositions();
      vp = VP_FOURWAY;
  }  

  updateDisplay(vp);
}
