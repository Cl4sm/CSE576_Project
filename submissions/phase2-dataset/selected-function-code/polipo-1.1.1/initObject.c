void
initObject()
{
    int q;
    if(objectHighMark < 16) {
        objectHighMark = 16;
        do_log(L_WARN, "Impossibly low objectHighMark -- setting to %d.\n",
               objectHighMark);
    }

    q = 0;
    if(publicObjectLowMark == 0) q = 1;
    if(publicObjectLowMark < 8 || publicObjectLowMark >= objectHighMark - 4) {
        publicObjectLowMark = objectHighMark / 2;
        if(!q)
            do_log(L_WARN, "Impossible publicObjectLowMark value -- "
                   "setting to %d.\n", publicObjectLowMark);
    }

    q = 1;
    if(objectHashTableSize <= objectHighMark / 2 ||
       objectHashTableSize > objectHighMark * 1024) {
        if(objectHashTableSize != 0) q = 0;
        objectHashTableSize = objectHighMark * 16;
    }
    log2ObjectHashTableSize = log2_ceil(objectHashTableSize);
    objectHashTableSize = 1 << log2ObjectHashTableSize;
    if(!q)
        do_log(L_WARN, "Suspicious objectHashTableSize value -- "
               "setting to %d.\n", objectHashTableSize);

    object_list = NULL;
    object_list_end = NULL;
    publicObjectCount = 0;
    privateObjectCount = 0;
    objectHashTable = calloc(1 << log2ObjectHashTableSize,
                             sizeof(ObjectPtr));
    if(!objectHashTable) {
        do_log(L_ERROR, "Couldn't allocate object hash table.\n");
        exit(1);
    }
}