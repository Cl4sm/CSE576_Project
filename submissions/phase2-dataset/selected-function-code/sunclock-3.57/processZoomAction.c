Sundata * Context;
int x;
int y;
int button;
int evtype;
{
           static int click_pos=-1;
           double v1, v2;
	   int but_pos;

           if (evtype == ButtonRelease && click_pos>=0) {
	       drawButton(Zoom, click_pos, 0);
               click_pos = -1;
  	   } 

	   if (y>areah+63 && y<areah+63+Context->gdata->menustrip) {
	      but_pos = getButton(Zoom, x, y);
	      if (but_pos>=N_ZOOM)
		 zoom_newhint = '\033';
	      else
	      if (but_pos>=0)
		 zoom_newhint = ZoomKey[2*but_pos];
	      if (evtype==MotionNotify) {
	         if (but_pos>=0)
		    showZoomHint(but_pos);
	      }
              if (evtype==ButtonPress && but_pos>=0) {
	         drawButton(Zoom, but_pos, 1);
		 click_pos  = but_pos;
	      }
	      if (evtype==ButtonRelease) {
		 if (zoom_newhint == '\033') {
		    PopZoom(Context);
		    return;
		 }
		 if (zoom_newhint == 'W') do_zoom = -1;
		 processKey(Context->win, tolower(zoom_newhint));
		 showZoomHint(but_pos);
		 zoom_mode = 0;
	      }
	      return;
	   }
	   if (button_pressed>=2) return;

           if (evtype==ButtonRelease)
	       drawButton(Zoom, N_ZOOM+1, 0);
           if (evtype==MotionNotify)
	       drawButton(Zoom, N_ZOOM+1, -2);

	   if (x>areah+170 && x<areah+170+Context->gdata->charspace && 
               y>21 && y<39) {
               if (evtype==MotionNotify) {
		  drawButton(Zoom, N_ZOOM+1, -1);
		  zoom_newhint = '"';
		  showZoomHint(getNumCmd(zoom_newhint));
	       }
               if (evtype==ButtonPress) {
		  drawButton(Zoom, N_ZOOM+1, 1);
	       }
               if (evtype==ButtonRelease) {
	          zoom_active = 1 -zoom_active;
	          zoom_mode |= 16;
	       }
	   } else
	   if (x>=60 && x<=areah+60 && y>=30 && y<=40 && button_pressed) {
              v1 = exp((double)(x-66)*log(100.00)/(double)(areah-6));
	      if (v1 != Context->newzoom.fx) {
		 Context->newzoom.fx = v1;
		 zoom_mode |= 10;
                 (void) setZoomAspect(Context, 2);
	      }
	   } else
	   if (x>=40 && x<=50 && y>=50 && y<=areah+50 && button_pressed) {
              v2  = exp((double)(y-53)*log(100.00)/(double)(areah-6));
	      if (v2 != Context->newzoom.fy) {
		 Context->newzoom.fy = v2;
		 zoom_mode |= 12;
                 (void) setZoomAspect(Context, 1);
	      }
	   }
           else
	   if (x>=60 && y>=50 && x<=60+areaw && y<=50+areah && button_pressed){
              v1 = ((double)(x-60))/((double)areaw);
              v2 = ((double)(y-50))/((double)areah);
              if (v1!=Context->newzoom.fdx || v2!=Context->newzoom.fdy) {
		 Context->newzoom.fdx = v1;
		 Context->newzoom.fdy = v2;
	         zoom_mode |= 8;
	      }
	      (void) setZoomAspect(Context, 3);
	   }
	   else {
	     zoom_newhint = ' ';
	     showZoomHint(-1);
	     if (button_pressed) return;
	   }

	   if (zoom_mode>0) {
              setZoomDimension(Context);
	      if (zoom_active && !button_pressed &&
                  memcmp(&Context->newzoom, 
                         &Context->zoom, sizeof(ZoomSettings))) {
		 button_pressed = 0;
		 activateZoom(Context, -1);
	      } else
	         setupZoom(zoom_mode);
 	      if (!button_pressed) zoom_mode = 0;
	   }
}
