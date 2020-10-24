SCM     gr_helpturtlegr()
{
        fputs( "\
Ret   Name               nargs    args        returns\n\
---------------------------------------------------------\n\
B  goto-home!              0       -          #f if not in graphics mode\n\
B  goto-center!            0       -          #f if not in graphics mode\n\
B  goto-nw!                0       -          #f if not in graphics mode\n\
B  goto-ne!                0       -          #f if not in graphics mode\n\
B  goto-sw!                0       -          #f if not in graphics mode\n\
B  goto-se!                0       -          #f if not in graphics mode\n\
B  draw                    1       length     #t if target within drawing area\n\
B  draw-to                 2       x y        #t if (x,y) within drawing area\n\
B  draw-to!                2       x y        #t if (x,y) within drawing area\n\
B  move                    1       length     #t if target within drawing area\n\
B  move-to!                2       x y        #t if (x,y) within drawing area\n\
i  where-x                 0       -          current x-coordinate\n\
i  where-y                 0       -          current y-coordinate\n\
i  turn-right              1       angle      drawing direction in degrees\n\
i  turn-left               1       angle      drawing direction in degrees\n\
i  turn-to!                1       angle      drawing direction in degrees\n\
i  what-direction          0       -          drawing direction in degrees\n\
B  set-color!              1       color      #t if color valid\n\
i  what-color              0       -          current drawing color\n\n\
", stdout );
        return  BOOL_T;
} /* gr_helpturtlegr() */
