static game_state *new_game_sub(const game_params *params, const char *desc)
{
    game_state *state = new_state(params);
    int x, y, run = 0;

    debug(("new_game[_sub]: desc = '%s'.\n", desc));

    for (y = 0; y < params->h; y++) {
        for (x = 0; x < params->w; x++) {
            char c = '\0';

            if (run == 0) {
                c = *desc++;
                assert(c != 'S');
                if (c >= 'a' && c <= 'z')
                    run = c - 'a' + 1;
            }

            if (run > 0) {
                c = 'S';
                run--;
            }

            switch (c) {
            case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
                island_add(state, x, y, (c - '0'));
                break;

            case 'A': case 'B': case 'C': case 'D':
            case 'E': case 'F': case 'G':
                island_add(state, x, y, (c - 'A') + 10);
                break;

            case 'S':
                /* empty square */
                break;

            default:
                assert(!"Malformed desc.");
                break;
            }
        }
    }
    if (*desc) assert(!"Over-long desc.");

    map_find_orthogonal(state);
    map_update_possibles(state);

    return state;
}