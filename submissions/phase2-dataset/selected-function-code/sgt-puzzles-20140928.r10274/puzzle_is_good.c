static int puzzle_is_good(game_state *state, int difficulty)
{
    int nsol, mdepth = 0;
    unsigned int sflags = flags_from_difficulty(difficulty);

    unplace_lights(state);

#ifdef SOLVER_DIAGNOSTICS
    debug(("Trying to solve with difficulty %d (0x%x):\n",
           difficulty, sflags));
    if (verbose) debug_state(state);
#endif

    nsol = dosolve(state, sflags, &mdepth);
    /* if we wanted an easy puzzle, make sure we didn't need recursion. */
    if (!(sflags & F_SOLVE_ALLOWRECURSE) && mdepth > 0) {
        debug(("Ignoring recursive puzzle.\n"));
        return 0;
    }

    debug(("%d solutions found.\n", nsol));
    if (nsol <= 0) return 0;
    if (nsol > 1) return 0;
    return 1;
}