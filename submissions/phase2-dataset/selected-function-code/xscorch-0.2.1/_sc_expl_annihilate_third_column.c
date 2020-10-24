                                             int cx, int cy, int dx, int dy,
                                             const sc_wedge_boundaries *wedge,
                                             bool fill) {

   /* Fills in 1, 2, or 3 of the sliced sections.
      3 section case only occurs at border cases. */
   int ldy = (int)(wedge->left_slope  * dx);
   int rdy = (int)(wedge->right_slope * dx);
   int min_boundary_y = min(ldy, rdy);
   int max_boundary_y = max(ldy, rdy);
   min_boundary_y = max(-dy, min_boundary_y);
   max_boundary_y = min( dy, max_boundary_y);

   #if SC_EXPL_DEBUG_WEDGES
      printf("expl_annihilate_third_column: dx=%04d, 3 sections, %04d -> %04d -> %04d -> %04d\n",
             dx, cy - dy, cy + min_boundary_y, cy + max_boundary_y, cy + dy);
   #endif /* SC_EXPL_DEBUG_WEDGES */

   switch(wedge->section) {
      case SC_WEDGE_SECTION_ALL:
         _sc_expl_annihilate_column(c, l, cx + dx, cy - dy, cy + dy, fill);
         break;
      case SC_WEDGE_SECTION_UPPER:
         _sc_expl_annihilate_column(c, l, cx + dx, cy + min_boundary_y, cy + dy, fill);
         break;
      case SC_WEDGE_SECTION_ENDS:
         _sc_expl_annihilate_column(c, l, cx + dx, cy + max_boundary_y, cy + dy, fill);
         _sc_expl_annihilate_column(c, l, cx + dx, cy - dy, cy + min_boundary_y, fill);
         break;
      case SC_WEDGE_SECTION_TOP:
         _sc_expl_annihilate_column(c, l, cx + dx, cy + max_boundary_y, cy + dy, fill);
         break;
      case SC_WEDGE_SECTION_LOWER:
         _sc_expl_annihilate_column(c, l, cx + dx, cy - dy, cy + max_boundary_y, fill);
         break;
      case SC_WEDGE_SECTION_MIDDLE:
         _sc_expl_annihilate_column(c, l, cx + dx, cy + min_boundary_y, cy + max_boundary_y, fill);
         break;
      case SC_WEDGE_SECTION_BOTTOM:
         _sc_expl_annihilate_column(c, l, cx + dx, cy - dy, cy + min_boundary_y, fill);
         break;
      case SC_WEDGE_SECTION_NONE:
         #if SC_EXPL_DEBUG_WEDGES
            printf("expl_annihilate_third_column: draw none (too thin?)\n");
         #else
            assert(false);
         #endif /* SC_EXPL_DEBUG_WEDGES */
         break;
   }

}
