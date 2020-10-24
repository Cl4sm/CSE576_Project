static char *encode_ui(const game_ui *ui)
{
    char *ret, *p, *sep;
    int i;

    /*
     * For this game it's worth storing the contents of the current
     * guess, and the current set of holds.
     */
    ret = snewn(40 * ui->curr_pegs->npegs, char);
    p = ret;
    sep = "";
    for (i = 0; i < ui->curr_pegs->npegs; i++) {
        p += sprintf(p, "%s%d%s", sep, ui->curr_pegs->pegs[i],
                     ui->holds[i] ? "_" : "");
        sep = ",";
    }
    *p++ = '\0';
    assert(p - ret < 40 * ui->curr_pegs->npegs);
    return sresize(ret, p - ret, char);
}