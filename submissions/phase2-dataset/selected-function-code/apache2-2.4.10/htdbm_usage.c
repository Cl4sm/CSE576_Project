static void htdbm_usage(void)
{
    fprintf(stderr,
        "htdbm -- program for manipulating DBM password databases.\n\n"
        "Usage: htdbm   [-cimBdpstvx] [-C cost] [-TDBTYPE] database username\n"
        "                -b[cmBdptsv] [-C cost] [-TDBTYPE] database username password\n"
        "                -n[imBdpst]  [-C cost] username\n"
        "                -nb[mBdpst]  [-C cost] username password\n"
        "                -v[imBdps]   [-C cost] [-TDBTYPE] database username\n"
        "                -vb[mBdps]   [-C cost] [-TDBTYPE] database username password\n"
        "                -x                     [-TDBTYPE] database username\n"
        "                -l                     [-TDBTYPE] database\n"
        "Options:\n"
        "   -c   Create a new database.\n"
        "   -n   Don't update database; display results on stdout.\n"
        "   -b   Use the password from the command line rather than prompting for it.\n"
        "   -i   Read password from stdin without verification (for script usage).\n"
        "   -m   Force MD5 encryption of the password (default).\n"
        "   -B   Force BCRYPT encryption of the password (very secure).\n"
        "   -C   Set the computing time used for the bcrypt algorithm\n"
        "        (higher is more secure but slower, default: %d, valid: 4 to 31).\n"
        "   -d   Force CRYPT encryption of the password (8 chars max, insecure).\n"
        "   -s   Force SHA encryption of the password (insecure).\n"
        "   -p   Do not encrypt the password (plaintext, insecure).\n"
        "   -T   DBM Type (SDBM|GDBM|DB|default).\n"
        "   -l   Display usernames from database on stdout.\n"
        "   -v   Verify the username/password.\n"
        "   -x   Remove the username record from database.\n"
        "   -t   The last param is username comment.\n"
        "The SHA algorithm does not use a salt and is less secure than the "
        "MD5 algorithm.\n",
        BCRYPT_DEFAULT_COST);
    exit(ERR_SYNTAX);
}