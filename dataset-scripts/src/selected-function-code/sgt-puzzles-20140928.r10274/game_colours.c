static float *game_colours(frontend *fe, int *ncolours)
{
    float *ret = snewn(3 * NCOLOURS, float);

    frontend_default_colour(fe, &ret[COL_BACKGROUND * 3]);

    ret[COL_TEXT * 3 + 0] = 0.0F;
    ret[COL_TEXT * 3 + 1] = 0.0F;
    ret[COL_TEXT * 3 + 2] = 0.0F;

    ret[COL_DOMINO * 3 + 0] = 0.0F;
    ret[COL_DOMINO * 3 + 1] = 0.0F;
    ret[COL_DOMINO * 3 + 2] = 0.0F;

    ret[COL_DOMINOCLASH * 3 + 0] = 0.5F;
    ret[COL_DOMINOCLASH * 3 + 1] = 0.0F;
    ret[COL_DOMINOCLASH * 3 + 2] = 0.0F;

    ret[COL_DOMINOTEXT * 3 + 0] = 1.0F;
    ret[COL_DOMINOTEXT * 3 + 1] = 1.0F;
    ret[COL_DOMINOTEXT * 3 + 2] = 1.0F;

    ret[COL_EDGE * 3 + 0] = ret[COL_BACKGROUND * 3 + 0] * 2 / 3;
    ret[COL_EDGE * 3 + 1] = ret[COL_BACKGROUND * 3 + 1] * 2 / 3;
    ret[COL_EDGE * 3 + 2] = ret[COL_BACKGROUND * 3 + 2] * 2 / 3;

    *ncolours = NCOLOURS;
    return ret;
}