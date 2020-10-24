void
releaseAtom(AtomPtr atom)
{
    if(atom == NULL)
        return;

    do_log(D_ATOM_REFCOUNT, "A 0x%lx %d--\n",
           (unsigned long)atom, atom->refcount);
    assert(atom->refcount >= 1 && atom->refcount < LARGE_ATOM_REFCOUNT);

    atom->refcount--;

    if(atom->refcount == 0) {
        int h = hash(0, atom->string, atom->length, LOG2_ATOM_HASH_TABLE_SIZE);
        assert(atomHashTable[h] != NULL);

        if(atom == atomHashTable[h]) {
            atomHashTable[h] = atom->next;
            free(atom);
        } else {
            AtomPtr previous = atomHashTable[h];
            while(previous->next) {
                if(previous->next == atom)
                    break;
                previous = previous->next;
            }
            assert(previous->next != NULL);
            previous->next = atom->next;
            free(atom);
        }
        used_atoms--;
    }
}