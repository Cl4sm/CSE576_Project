void gameMouse(int buttons, int state, int x, int y) {
  if(state == SYSTEM_MOUSEPRESSED) {
    if(buttons == SYSTEM_MOUSEBUTTON_LEFT) gInput.mouse1 = 1;
    if(buttons == SYSTEM_MOUSEBUTTON_RIGHT) gInput.mouse2 = 1;
  } else if(state == SYSTEM_MOUSERELEASED) {
    if(buttons == SYSTEM_MOUSEBUTTON_LEFT) gInput.mouse1 = 0;
    if(buttons == SYSTEM_MOUSEBUTTON_RIGHT) gInput.mouse2 = 0;
  }

  /*
  if(getSettingi("camType") == CAM_TYPE_MOUSE) 
    if(state == SYSTEM_MOUSEPRESSED) {
      if(buttons == SYSTEM_MOUSEBUTTON_LEFT) {
	cam_r -= CAM_DR;
	if(cam_r < CAM_R_MIN) cam_r = CAM_R_MIN;
      } else if(buttons == SYSTEM_MOUSEBUTTON_RIGHT) {
	cam_r += CAM_DR;
	if(cam_r > CAM_R_MAX) cam_r = CAM_R_MAX;
      }
    }
  */
  /* fprintf(stderr, "new cam_r: %.2f\n", cam_r); */
}
