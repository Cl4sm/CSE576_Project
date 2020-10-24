static void set_light(game_state *state, int ox, int oy, int on)
{
    ll_data lld;
    int diff = 0;

    assert(!(GRID(state,flags,ox,oy) & F_BLACK));

    if (!on && GRID(state,flags,ox,oy) & F_LIGHT) {
        diff = -1;
        GRID(state,flags,ox,oy) &= ~F_LIGHT;
        state->nlights--;
    } else if (on && !(GRID(state,flags,ox,oy) & F_LIGHT)) {
        diff = 1;
        GRID(state,flags,ox,oy) |= F_LIGHT;
        state->nlights++;
    }

    if (diff != 0) {
        list_lights(state,ox,oy,1,&lld);
        FOREACHLIT(&lld, GRID(state,lights,lx,ly) += diff; );
    }
}