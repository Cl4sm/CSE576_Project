static void _sc_expl_annihilate_half_column(sc_config *c, sc_land *l,
                                            int cx, int cy, int dx, int dy,
                                            const sc_wedge_boundaries *wedge,
                                            int critical_y, bool fill) {

   #if SC_EXPL_DEBUG_WEDGES
      printf("expl_annihilate_half_column: dx=%04d, 2 sections, %04d -> %04d -> %04d\n",
             dx, cy - dy, cy + critical_y, cy + dy);
   #endif /* SC_EXPL_DEBUG_WEDGES */

   /* The middle section is illegal in half_column. */
   assert(!(wedge->section & SC_WEDGE_SECTION_MIDDLE));

   switch(wedge->section) {
      case SC_WEDGE_SECTION_ENDS:
         _sc_expl_annihilate_column(c, l, cx + dx, cy - dy, cy + dy, fill);
         break;
      case SC_WEDGE_SECTION_TOP:
         _sc_expl_annihilate_column(c, l, cx + dx, cy + critical_y, cy + dy, fill);
         break;
      case SC_WEDGE_SECTION_BOTTOM:
         _sc_expl_annihilate_column(c, l, cx + dx, cy - dy, cy + critical_y, fill);
         break;
      case SC_WEDGE_SECTION_NONE:
         #if SC_EXPL_DEBUG_WEDGES
            printf("expl_annihilate_half_column: draw none (too thin?)\n");
         #else
            assert(false);
         #endif /* SC_EXPL_DEBUG_WEDGES */
         break;
   }

}
