void latin_solver_debug(unsigned char *cube, int o)
{
#ifdef STANDALONE_SOLVER
    if (solver_show_working > 1) {
        struct latin_solver ls, *solver = &ls;
        char *dbg;
        int x, y, i, c = 0;

        ls.cube = cube; ls.o = o; /* for cube() to work */

        dbg = snewn(3*o*o*o, char);
        for (y = 0; y < o; y++) {
            for (x = 0; x < o; x++) {
                for (i = 1; i <= o; i++) {
                    if (cube(x,y,i))
                        dbg[c++] = i + '0';
                    else
                        dbg[c++] = '.';
                }
                dbg[c++] = ' ';
            }
            dbg[c++] = '\n';
        }
        dbg[c++] = '\n';
        dbg[c++] = '\0';

        printf("%s", dbg);
        sfree(dbg);
    }
#endif
}