static void game_compute_size(const game_params *params, int tilesize,
                              int *x, int *y)
{
    double hmul, vmul_c, vmul_g, vmul;
    int hintw = (params->npegs+1)/2;

    hmul = BORDER   * 2.0 +               /* border */
           1.0      * 2.0 +               /* vertical colour bar */
           1.0      * params->npegs +     /* guess pegs */
           PEG_GAP  * params->npegs +     /* guess gaps */
           PEG_HINT * hintw +             /* hint pegs */
           PEG_GAP  * (hintw - 1);        /* hint gaps */

    vmul_c = BORDER  * 2.0 +                    /* border */
             1.0     * params->ncolours +       /* colour pegs */
             PEG_GAP * (params->ncolours - 1);  /* colour gaps */

    vmul_g = BORDER  * 2.0 +                    /* border */
             1.0     * (params->nguesses + 1) + /* guesses plus solution */
             PEG_GAP * (params->nguesses + 1);  /* gaps plus gap above soln */

    vmul = max(vmul_c, vmul_g);

    *x = (int)ceil((double)tilesize * hmul);
    *y = (int)ceil((double)tilesize * vmul);
}