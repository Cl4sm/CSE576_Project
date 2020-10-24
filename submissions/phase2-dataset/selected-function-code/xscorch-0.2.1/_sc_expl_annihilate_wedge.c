static void _sc_expl_annihilate_wedge(sc_config *c, const sc_explosion *e,
                                      int radius, bool fill) {
/* sc_expl_annihilate_wedge
   Clears (if fill == false) or fills (if fill == true) dirt in the
   explosion specified.  This updates the land buffer but it will NOT
   update the display screen.

   This function is used for code that is performing wedged explosions.
   It may be slightly less efficient than annihilate_circular; once I
   convince myself that this code actually works, I may go ahead and
   delete annihilate_circular.  */

   int cx=e->centerx;/* Center X of explosion */
   int cy=e->centery;/* Center Y of explosion */
   int dx;           /* Delta X (distance away from cx) - iterator variable */
   int dy;           /* Delta Y (distance away from cy) for _edge_ of circle */
   int rad2;         /* Radius squared */
   int rad2major2;   /* Radius^2 + the major_distance^2 */
   int min2thresh;   /* Minimum threshold to avoid redrawing columns where dx>dy */
   sc_wedge_boundaries wedge; /* Wedge boundary support */
   int min_x, max_x;          /* Affected range of dx */
   double left_angle;         /* Leftmost angle of wedge */
   double right_angle;        /* Rightmost angle of wedge */

   /* Figure out the minimum and maximum angles for the wedge */
   right_angle = e->direction - e->angular_width / 2;
   left_angle  = e->direction + e->angular_width / 2;

   /* By default, assume we are drawing "inside" the wedge, not
      outside of it.  We might change our mind on this once we
      figure out how wide the arc is...  */
   wedge.inside = true;

   /* Coding for detecting a wedge requires < 180 degree separation
      If > 180 degree seperation, reverse say draw outside the region
      If < 180 degree seperation, then we're okay.  */
   if((_sc_expl_clockwise_closest(left_angle, right_angle) - left_angle < -M_PI)) {
      left_angle     = left_angle + right_angle;
      right_angle    = left_angle - right_angle;
      left_angle     = left_angle - right_angle;
      wedge.inside   = false;
   }

   /* Make sure nothing is odd here with 360 degrees wrapping to 0 degrees...
    * Also skip the trig */
   if(e->angular_width > M_PI * 2 - 0.00001) {
      /* We're not using the wedge code in this case */
      wedge.inside   = false;

      /* 360, Draw everything! */
      wedge.section  = SC_WEDGE_SECTION_ALL;
      wedge.left_x   = 0;
      wedge.right_x  = 0;
      min_x          = 0;
      max_x          = 0;
      wedge.left_y   = radius;
      wedge.right_y  = radius;
      /* left_slope and right_slope are ignored */
   } else {
      wedge.left_x   = rint(radius * cos(left_angle));
      wedge.left_y   = rint(radius * sin(left_angle));
      wedge.left_slope =             tan(left_angle);
      wedge.right_x  = rint(radius * cos(right_angle));
      wedge.right_y  = rint(radius * sin(right_angle));
      wedge.right_slope =            tan(right_angle);
      min_x          = min(wedge.left_x, wedge.right_x);
      max_x          = max(wedge.left_x, wedge.right_x);

      if(min_x * max_x >= 0) {
         /* Both wedge boundaries are on the same side... */
         wedge.thirds = true;
         wedge.section = (wedge.inside ? SC_WEDGE_SECTION_MIDDLE
                                       : SC_WEDGE_SECTION_TOP | SC_WEDGE_SECTION_BOTTOM);
         min_x       = min(min_x, 0);
         max_x       = max(max_x, 0);
      } else {
         /* wedge boundaries on opposite sides */
         wedge.thirds = false;
         wedge.section = (_sc_expl_pixel_test(radius, &wedge) ? SC_WEDGE_SECTION_TOP
                                                              : SC_WEDGE_SECTION_BOTTOM);
      }

      #if SC_EXPL_DEBUG_WEDGES
         printf("**** New Explosion (expl_annihilate_wedge) ****\n");
         printf("center x=%04, y=%04, r=%04\n", cx, cy, radius);
         printf("left: %03dD right: %03dD, %s\n",
                (int)(left_angle * 180 / M_PI),
                (int)(right_angle * 180 / M_PI),
                wedge.inside ? "inside" : "outside");
         printf("dx_min = %04d, dx_max = %04d\n\n", min_x, max_x);
      #endif /* SC_EXPL_DEBUG_WEDGES */
   }

   /* DX = major axis, DY = minor axis */
   dx = 0;           /* DX starts at zero (iterator) */
   dy = radius;      /* DY is one radius away (edge of circle at cx+dx) */
   rad2 = radius * radius; /* Calculate Radius Squared */
   rad2major2 = rad2;      /* Radius^2 + major^2, running total */
   min2thresh = rad2 - dy; /* Minimum threshold before need to redraw edges */

   /* Should know that, we are incrementing DX every time.  However,
      if we call the transpose method every time as well, then we will
      be filling parts of the circle multiple times.  Hence the
      min2thresh variable. */
   do {
      /* Column pair is columns at cx-dx and cx+dx */
      _sc_expl_annihilate_column_pair(c, c->land,
                                      cx, cy, dx, dy,
                                      &wedge, fill,
                                      min_x, max_x);
      ++dx;
      rad2major2 -= dx + dx - 1;
      if(rad2major2 <= min2thresh) {
         /* swap dx and dy */
         _sc_expl_annihilate_column_pair(c, c->land,
                                         cx, cy, dy, dx,
                                         &wedge, fill,
                                         min_x, max_x);
         --dy;
         min2thresh -= dy + dy;
      }
   } while(dx <= dy);

}
