/* NRSS - ncurses RSS reader
   Copyright (C) 2007 Jack Miller <jjm2n4@umr.edu>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License version 2 as 
   published by the Free Software Foundation.
*/
#include "nrss.h"
#include "conf.h"

#define ret_key_const(_str)    \
        else if(!strcmp( #_str, tmp))   \
           key =  _str;

int get_key(char *data)
{
    char *tmp = data;
    int i = 0;
    int mod = 0;
    int key = 0;

    for (i = 0; i < 2; i++) {
        if (!strncmp(tmp, "C-", 2)) {
            mod |= 1;
            tmp += 2;
        } else if (!strncmp(tmp, "M-", 2)) {
            mod |= 2;
            tmp += 2;
        }
    }

    if (strlen(tmp) == 1)
        key = (int) tmp[0];
    else if ((!strncmp(tmp, "KEY_F", 5)) && (tmp[5] >= '0')
             && (tmp[5] <= '9')) {
        int num = 0;
        if ((num = atoi(&tmp[5])) > 0)
            key = KEY_F0 + num;
    } else if (!strncmp(data, "KEY_RETURN", 11))
        key = 10;
    ret_key_const(KEY_DOWN)     /* down-arrow key */
        ret_key_const(KEY_UP)   /* up-arrow key */
        ret_key_const(KEY_LEFT) /* left-arrow key */
        ret_key_const(KEY_RIGHT)        /* right-arrow key */
        ret_key_const(KEY_HOME) /* home key */
        ret_key_const(KEY_BACKSPACE)    /* backspace key */
        ret_key_const(KEY_DL)   /* delete-line key */
        ret_key_const(KEY_IL)   /* insert-line key */
        ret_key_const(KEY_DC)   /* delete-character key */
        ret_key_const(KEY_IC)   /* insert-character key */
        ret_key_const(KEY_EIC)  /* sent by rmir or smir in insert mode */
        ret_key_const(KEY_CLEAR)        /* clear-screen or erase key */
        ret_key_const(KEY_EOS)  /* clear-to-end-of-screen key */
        ret_key_const(KEY_EOL)  /* clear-to-end-of-line key */
        ret_key_const(KEY_SF)   /* scroll-forward key */
        ret_key_const(KEY_SR)   /* scroll-backward key */
        ret_key_const(KEY_NPAGE)        /* next-page key */
        ret_key_const(KEY_PPAGE)        /* previous-page key */
        ret_key_const(KEY_STAB) /* set-tab key */
        ret_key_const(KEY_CTAB) /* clear-tab key */
        ret_key_const(KEY_CATAB)        /* clear-all-tabs key */
        ret_key_const(KEY_ENTER)        /* enter/send key */
        ret_key_const(KEY_PRINT)        /* print key */
        ret_key_const(KEY_LL)   /* lower-left key (home down) */
        ret_key_const(KEY_A1)   /* upper left of keypad */
        ret_key_const(KEY_A3)   /* upper right of keypad */
        ret_key_const(KEY_B2)   /* center of keypad */
        ret_key_const(KEY_C1)   /* lower left of keypad */
        ret_key_const(KEY_C3)   /* lower right of keypad */
        ret_key_const(KEY_BTAB) /* back-tab key */
        ret_key_const(KEY_BEG)  /* begin key */
        ret_key_const(KEY_CANCEL)       /* cancel key */
        ret_key_const(KEY_CLOSE)        /* close key */
        ret_key_const(KEY_COMMAND)      /* command key */
        ret_key_const(KEY_COPY) /* copy key */
        ret_key_const(KEY_CREATE)       /* create key */
        ret_key_const(KEY_END)  /* end key */
        ret_key_const(KEY_EXIT) /* exit key */
        ret_key_const(KEY_FIND) /* find key */
        ret_key_const(KEY_HELP) /* help key */
        ret_key_const(KEY_MARK) /* mark key */
        ret_key_const(KEY_MESSAGE)      /* message key */
        ret_key_const(KEY_MOVE) /* move key */
        ret_key_const(KEY_NEXT) /* next key */
        ret_key_const(KEY_OPEN) /* open key */
        ret_key_const(KEY_OPTIONS)      /* options key */
        ret_key_const(KEY_PREVIOUS)     /* previous key */
        ret_key_const(KEY_REDO) /* redo key */
        ret_key_const(KEY_REFERENCE)    /* reference key */
        ret_key_const(KEY_REFRESH)      /* refresh key */
        ret_key_const(KEY_REPLACE)      /* replace key */
        ret_key_const(KEY_RESTART)      /* restart key */
        ret_key_const(KEY_RESUME)       /* resume key */
        ret_key_const(KEY_SAVE) /* save key */
        ret_key_const(KEY_SBEG) /* shifted begin key */
        ret_key_const(KEY_SCANCEL)      /* shifted cancel key */
        ret_key_const(KEY_SCOMMAND)     /* shifted command key */
        ret_key_const(KEY_SCOPY)        /* shifted copy key */
        ret_key_const(KEY_SCREATE)      /* shifted create key */
        ret_key_const(KEY_SDC)  /* shifted delete-character key */
        ret_key_const(KEY_SDL)  /* shifted delete-line key */
        ret_key_const(KEY_SELECT)       /* select key */
        ret_key_const(KEY_SEND) /* shifted end key */
        ret_key_const(KEY_SEOL) /* shifted clear-to-end-of-line key */
        ret_key_const(KEY_SEXIT)        /* shifted exit key */
        ret_key_const(KEY_SFIND)        /* shifted find key */
        ret_key_const(KEY_SHELP)        /* shifted help key */
        ret_key_const(KEY_SHOME)        /* shifted home key */
        ret_key_const(KEY_SIC)  /* shifted insert-character key */
        ret_key_const(KEY_SLEFT)        /* shifted left-arrow key */
        ret_key_const(KEY_SMESSAGE)     /* shifted message key */
        ret_key_const(KEY_SMOVE)        /* shifted move key */
        ret_key_const(KEY_SNEXT)        /* shifted next key */
        ret_key_const(KEY_SOPTIONS)     /* shifted options key */
        ret_key_const(KEY_SPREVIOUS)    /* shifted previous key */
        ret_key_const(KEY_SPRINT)       /* shifted print key */
        ret_key_const(KEY_SREDO)        /* shifted redo key */
        ret_key_const(KEY_SREPLACE)     /* shifted replace key */
        ret_key_const(KEY_SRIGHT)       /* shifted right-arrow key */
        ret_key_const(KEY_SRSUME)       /* shifted resume key */
        ret_key_const(KEY_SSAVE)        /* shifted save key */
        ret_key_const(KEY_SSUSPEND)     /* shifted suspend key */
        ret_key_const(KEY_SUNDO)        /* shifted undo key */
        ret_key_const(KEY_SUSPEND)      /* suspend key */
        ret_key_const(KEY_UNDO)
        /* undo key */
        if (key == 0)
        return 0;

    if (mod & 1)
        key = CTRL(key);
    if (mod & 2)
        key = ALT(key);

    return key;
}

int check_char(void *data)
{
    int len = strlen((char *) data);

    if ((len == 1) || (get_key((char *) data)))
        return 0;

    return 1;
}

void clear_bind(int key)
{
    for (int i = 0; i < NUM_FBINDINGS; i++) {
        if (nc->keys[i].bind == key) {
            logit("Key bound to %s overridden.\n", nc->keys[i].descr);
            nc->keys[i].bind = 0;
        }
    }
}

int set_key(void *data)
{
    for (int i = 0; i < NUM_FBINDINGS; i++) {
        if (!strcmp(((char **) data)[0], nc->keys[i].descr)) {
            int key = get_key(((char **) data)[1]);
            clear_bind(key);
            nc->keys[i].bind = key;
            nc->keys[i].printable = ((char **) data)[1];
            return 0;
        }
    }

    logit("Unknown function \"%s\"\n", ((char **) data)[0]);

    return 1;
}
