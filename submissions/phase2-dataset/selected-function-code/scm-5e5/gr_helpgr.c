SCM     gr_helpgr()
{
        fputs( "\
Ret   Name               nargs    args        returns\n\
---------------------------------------------------------\n\
B  graphics-avail?         0       -          #t if graphics available\n\
B  graphics-mode!          0       -          #f if no graphics\n\
B  text-mode!              0       -          #t on success\n\
B  clear-graphics!         0       -          #f if not in graphics mode\n\
i  max-x                   0       -          maximum value of x\n\
i  max-y                   0       -          maximum value of y\n\
i  max-color               0       -          maximum value of color\n\
B  valid-xyc?              3       x y color  #t if valid\n\
B  set-dot!                3       x y color  #t on success\n\
i  get-dot                 2       x y        color of the dot in (x,y)\n\
                                              or #f if (x,y) not legal\n\
\n\
NOTE: Origin (0,0) is in the upper left corner.\n\n\
", stdout );
        return  BOOL_T;
} /* gr_helpgr() */
