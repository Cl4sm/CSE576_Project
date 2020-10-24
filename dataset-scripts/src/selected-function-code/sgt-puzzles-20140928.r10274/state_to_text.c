static char *state_to_text(const game_state *state)
{
    grid *g = state->game_grid;
    char *retval;
    int num_faces = g->num_faces;
    char *description = snewn(num_faces + 1, char);
    char *dp = description;
    int empty_count = 0;
    int i;

    for (i = 0; i < num_faces; i++) {
        if (state->clues[i] < 0) {
            if (empty_count > 25) {
                dp += sprintf(dp, "%c", (int)(empty_count + 'a' - 1));
                empty_count = 0;
            }
            empty_count++;
        } else {
            if (empty_count) {
                dp += sprintf(dp, "%c", (int)(empty_count + 'a' - 1));
                empty_count = 0;
            }
            dp += sprintf(dp, "%c", (int)CLUE2CHAR(state->clues[i]));
        }
    }

    if (empty_count)
        dp += sprintf(dp, "%c", (int)(empty_count + 'a' - 1));

    retval = dupstr(description);
    sfree(description);

    return retval;
}