static bool _sc_ai_validate_angle(const sc_config *c, sc_player *p, int angle) {
/* sc_ai_validate_angle
   Checks to see if there is a line of sight at least SC_AI_VALIDATE_DIST
   pixels away from the AI tank in the specified angle.  This is to prevent
   the AI from firing directly into an adjacent cliff, causing the AI to
   get absorbed into its own explosion.  Returns true if the path appears
   to be clear along the angle, and false otherwise.  This is HEURISTIC!  */

   double dx;
   double dy;
   double x;
   double y;
   int step;
   int cx;
   int cy;
   
   assert(c != NULL && p != NULL);

   dx = SC_AI_VALIDATE_DIST * cos(angle * M_PI / 180);
   dy = SC_AI_VALIDATE_DIST * sin(angle * M_PI / 180);

   if(fabs(dx) > fabs(dy)) {
      step = ceil(fabs(dx));
   } else {
      step = ceil(fabs(dy));
   }

   x = sc_player_turret_x(p, angle);
   y = sc_player_turret_y(p, angle);
   dx /= step;
   dy /= step;

   while(step > 0) {
      cx = rint(x);
      cy = rint(y);
      if(sc_land_translate_xy(c->land, &cx, &cy)) {
         if(!SC_LAND_IS_SKY(*SC_LAND_XY(c->land, cx, cy))) {
            return(false);
         }
      }
      x += dx;
      y += dy;
      --step;
   }

   return(true);

}
