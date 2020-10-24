static void game_changed_state(game_ui *ui, const game_state *oldstate,
                               const game_state *newstate)
{
    int i;

    /* Implement holds, clear other pegs.
     * This does something that is arguably the Right Thing even
     * for undo. */
    for (i = 0; i < newstate->solution->npegs; i++) {
        if (newstate->solved)
            ui->holds[i] = 0;
        else
            ui->holds[i] = newstate->holds[i];
	if (newstate->solved || (newstate->next_go == 0) || !ui->holds[i]) {
	    ui->curr_pegs->pegs[i] = 0;
	} else
            ui->curr_pegs->pegs[i] =
                newstate->guesses[newstate->next_go-1]->pegs[i];
    }
    ui->markable = is_markable(&newstate->params, ui->curr_pegs);
    /* Clean up cursor position */
    if (!ui->markable && ui->peg_cur == newstate->solution->npegs)
	ui->peg_cur--;
}