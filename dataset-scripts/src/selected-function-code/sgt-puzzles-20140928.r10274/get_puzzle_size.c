static void get_puzzle_size(document *doc, struct puzzle *pz,
			    float *w, float *h, float *scale)
{
    float ww, hh, ourscale;

    /* Get the preferred size of the game, in mm. */
    pz->game->print_size(pz->par, &ww, &hh);

    /* Adjust for user-supplied scale factor. */
    ourscale = doc->userscale;

    /*
     * FIXME: scale it down here if it's too big for the page size.
     * Rather than do complicated things involving scaling all
     * columns down in proportion, the simplest approach seems to
     * me to be to scale down until the game fits within one evenly
     * divided cell of the page (i.e. width/pw by height/ph).
     * 
     * In order to do this step we need the page size available.
     */

    *scale = ourscale;
    *w = ww * ourscale;
    *h = hh * ourscale;
}