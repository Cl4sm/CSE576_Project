AtomPtr
internAtomN(const char *string, int n)
{
    AtomPtr atom;
    int h;

    if(n < 0 || n >= (1 << (8 * sizeof(unsigned short))))
        return NULL;

    h = hash(0, string, n, LOG2_ATOM_HASH_TABLE_SIZE);
    atom = atomHashTable[h];
    while(atom) {
        if(atom->length == n &&
           (n == 0 || memcmp(atom->string, string, n) == 0))
            break;
        atom = atom->next;
    }

    if(!atom) {
        atom = malloc(sizeof(AtomRec) - 1 + n + 1);
        if(atom == NULL) {
            return NULL;
        }
        atom->refcount = 0;
        atom->length = n;
        /* Atoms are used both for binary data and strings.  To make
           their use as strings more convenient, atoms are always
           NUL-terminated. */
        memcpy(atom->string, string, n);
        atom->string[n] = '\0';
        atom->next = atomHashTable[h];
        atomHashTable[h] = atom;
        used_atoms++;
    }
    do_log(D_ATOM_REFCOUNT, "A 0x%lx %d++\n",
           (unsigned long)atom, atom->refcount);
    atom->refcount++;
    return atom;
}