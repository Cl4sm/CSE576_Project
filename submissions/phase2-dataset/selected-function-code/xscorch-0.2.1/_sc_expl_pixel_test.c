static bool _sc_expl_pixel_test(int dy, const sc_wedge_boundaries *wedge) {
/* sc_expl_pixel_test */

   int dx            = 0;  /* currently this is never called with any other dx */
   int left_cross_r  = (wedge->left_y  * dx - wedge->left_x  * dy);
   int right_cross_r = (wedge->right_y * dx - wedge->right_x * dy);
   bool is_inside    = (left_cross_r > 0) && (right_cross_r < 0);
   bool border       =
      ((left_cross_r == 0) && (right_cross_r <  0)) ||
      ((left_cross_r >  0) && (right_cross_r == 0));

   if(border) {
      return(true);
   }
   if(wedge->inside) {
      return(is_inside);
   } else {
      return(!is_inside);
   }

}
