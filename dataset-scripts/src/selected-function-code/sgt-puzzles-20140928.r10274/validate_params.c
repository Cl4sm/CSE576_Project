static char *validate_params(const game_params *params, int full)
{
    if ((params->w2 & 1) || (params->h2 & 1))
        return "Width and height must both be even";
    if (params->w2 < 6 || params->h2 < 6)
        return "Width and height must be at least 6";
    if (params->unique) {
        static const long A177790[] = {
            /*
             * The nth element of this array gives the number of
             * distinct possible Unruly rows of length 2n (that is,
             * containing exactly n 1s and n 0s and not containing
             * three consecutive elements the same) for as long as
             * those numbers fit in a 32-bit signed int.
             *
             * So in unique-rows mode, if the puzzle width is 2n, then
             * the height must be at most (this array)[n], and vice
             * versa.
             *
             * This is sequence A177790 in the Online Encyclopedia of
             * Integer Sequences: http://oeis.org/A177790
             */
            1L, 2L, 6L, 14L, 34L, 84L, 208L, 518L, 1296L, 3254L,
            8196L, 20700L, 52404L, 132942L, 337878L, 860142L,
            2192902L, 5598144L, 14308378L, 36610970L, 93770358L,
            240390602L, 616787116L, 1583765724L
        };
        if (params->w2 < 2*lenof(A177790) &&
            params->h2 > A177790[params->w2/2]) {
            return "Puzzle is too tall for unique-rows mode";
        }
        if (params->h2 < 2*lenof(A177790) &&
            params->w2 > A177790[params->h2/2]) {
            return "Puzzle is too long for unique-rows mode";
        }
    }
    if (params->diff >= DIFFCOUNT)
        return "Unknown difficulty rating";

    return NULL;
}