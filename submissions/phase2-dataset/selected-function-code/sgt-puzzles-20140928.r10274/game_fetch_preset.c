static int game_fetch_preset(int i, char **name, game_params **params)
{
    game_params *ret;
    int n;
    char buf[80];

    switch (i) {
      case 0: n = 3; break;
      case 1: n = 4; break;
      case 2: n = 5; break;
      case 3: n = 6; break;
      case 4: n = 7; break;
      case 5: n = 8; break;
      case 6: n = 9; break;
      default: return FALSE;
    }

    sprintf(buf, "Up to double-%d", n);
    *name = dupstr(buf);

    *params = ret = snew(game_params);
    ret->n = n;
    ret->unique = TRUE;

    return TRUE;
}