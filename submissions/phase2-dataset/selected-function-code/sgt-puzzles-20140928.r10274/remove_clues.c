static game_state *remove_clues(game_state *state, random_state *rs,
                                int diff)
{
    int *face_list;
    int num_faces = state->game_grid->num_faces;
    game_state *ret = dup_game(state), *saved_ret;
    int n;

    /* We need to remove some clues.  We'll do this by forming a list of all
     * available clues, shuffling it, then going along one at a
     * time clearing each clue in turn for which doing so doesn't render the
     * board unsolvable. */
    face_list = snewn(num_faces, int);
    for (n = 0; n < num_faces; ++n) {
        face_list[n] = n;
    }

    shuffle(face_list, num_faces, sizeof(int), rs);

    for (n = 0; n < num_faces; ++n) {
        saved_ret = dup_game(ret);
        ret->clues[face_list[n]] = -1;

        if (game_has_unique_soln(ret, diff)) {
            free_game(saved_ret);
        } else {
            free_game(ret);
            ret = saved_ret;
        }
    }
    sfree(face_list);

    return ret;
}