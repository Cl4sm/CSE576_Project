static char *encode_move(const game_state *from, game_ui *ui)
{
    char *buf, *p, *sep;
    int len, i;

    len = ui->curr_pegs->npegs * 20 + 2;
    buf = snewn(len, char);
    p = buf;
    *p++ = 'G';
    sep = "";
    for (i = 0; i < ui->curr_pegs->npegs; i++) {
	p += sprintf(p, "%s%d%s", sep, ui->curr_pegs->pegs[i],
                     ui->holds[i] ? "_" : "");
	sep = ",";
    }
    *p++ = '\0';
    assert(p - buf <= len);
    buf = sresize(buf, len, char);

    return buf;
}