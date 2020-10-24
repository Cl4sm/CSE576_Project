void midend_serialise(midend *me,
                      void (*write)(void *ctx, void *buf, int len),
                      void *wctx)
{
    int i;

    /*
     * Each line of the save file contains three components. First
     * exactly 8 characters of header word indicating what type of
     * data is contained on the line; then a colon followed by a
     * decimal integer giving the length of the main string on the
     * line; then a colon followed by the string itself (exactly as
     * many bytes as previously specified, no matter what they
     * contain). Then a newline (of reasonably flexible form).
     */
#define wr(h,s) do { \
    char hbuf[80]; \
    char *str = (s); \
    sprintf(hbuf, "%-8.8s:%d:", (h), (int)strlen(str)); \
    write(wctx, hbuf, strlen(hbuf)); \
    write(wctx, str, strlen(str)); \
    write(wctx, "\n", 1); \
} while (0)

    /*
     * Magic string identifying the file, and version number of the
     * file format.
     */
    wr("SAVEFILE", SERIALISE_MAGIC);
    wr("VERSION", SERIALISE_VERSION);

    /*
     * The game name. (Copied locally to avoid const annoyance.)
     */
    {
        char *s = dupstr(me->ourgame->name);
        wr("GAME", s);
        sfree(s);
    }

    /*
     * The current long-term parameters structure, in full.
     */
    if (me->params) {
        char *s = me->ourgame->encode_params(me->params, TRUE);
        wr("PARAMS", s);
        sfree(s);
    }

    /*
     * The current short-term parameters structure, in full.
     */
    if (me->curparams) {
        char *s = me->ourgame->encode_params(me->curparams, TRUE);
        wr("CPARAMS", s);
        sfree(s);
    }

    /*
     * The current game description, the privdesc, and the random seed.
     */
    if (me->seedstr)
        wr("SEED", me->seedstr);
    if (me->desc)
        wr("DESC", me->desc);
    if (me->privdesc)
        wr("PRIVDESC", me->privdesc);

    /*
     * The game's aux_info. We obfuscate this to prevent spoilers
     * (people are likely to run `head' or similar on a saved game
     * file simply to find out what it is, and don't necessarily
     * want to be told the answer to the puzzle!)
     */
    if (me->aux_info) {
        unsigned char *s1;
        char *s2;
        int len;

        len = strlen(me->aux_info);
        s1 = snewn(len, unsigned char);
        memcpy(s1, me->aux_info, len);
        obfuscate_bitmap(s1, len*8, FALSE);
        s2 = bin2hex(s1, len);

        wr("AUXINFO", s2);

        sfree(s2);
        sfree(s1);
    }

    /*
     * Any required serialisation of the game_ui.
     */
    if (me->ui) {
        char *s = me->ourgame->encode_ui(me->ui);
        if (s) {
            wr("UI", s);
            sfree(s);
        }
    }

    /*
     * The game time, if it's a timed game.
     */
    if (me->ourgame->is_timed) {
        char buf[80];
        sprintf(buf, "%g", me->elapsed);
        wr("TIME", buf);
    }

    /*
     * The length of, and position in, the states list.
     */
    {
        char buf[80];
        sprintf(buf, "%d", me->nstates);
        wr("NSTATES", buf);
        sprintf(buf, "%d", me->statepos);
        wr("STATEPOS", buf);
    }

    /*
     * For each state after the initial one (which we know is
     * constructed from either privdesc or desc), enough
     * information for execute_move() to reconstruct it from the
     * previous one.
     */
    for (i = 1; i < me->nstates; i++) {
        assert(me->states[i].movetype != NEWGAME);   /* only state 0 */
        switch (me->states[i].movetype) {
          case MOVE:
            wr("MOVE", me->states[i].movestr);
            break;
          case SOLVE:
            wr("SOLVE", me->states[i].movestr);
            break;
          case RESTART:
            wr("RESTART", me->states[i].movestr);
            break;
        }
    }

#undef wr
}