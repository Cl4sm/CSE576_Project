int
discardObjects(int all, int force)
{
    ObjectPtr object;
    int i;
    static int in_discardObjects = 0;
    TimeEventHandlerPtr event;

    if(in_discardObjects)
        return 0;

    in_discardObjects = 1;
    
    if(all || force || used_chunks >= CHUNKS(chunkHighMark) ||
       publicObjectCount >= publicObjectLowMark ||
       publicObjectCount + privateObjectCount >= objectHighMark) {
        object = object_list_end;
        while(object && 
              (all || force || used_chunks >= CHUNKS(chunkLowMark))) {
            if(force || ((object->flags & OBJECT_PUBLIC) &&
                         object->numchunks > CHUNKS(chunkLowMark) / 4)) {
                int j;
                for(j = 0; j < object->numchunks; j++) {
                    if(object->chunks[j].locked) {
                        break;
                    }
                    if(object->chunks[j].size < CHUNK_SIZE) {
                        continue;
                    }
                    writeoutToDisk(object, (j + 1) * CHUNK_SIZE, -1);
                    dispose_chunk(object->chunks[j].data);
                    object->chunks[j].data = NULL;
                    object->chunks[j].size = 0;
                }
            }
            object = object->previous;
        }
        
        i = 0;
        object = object_list_end;
        while(object && 
              (all || force ||
               used_chunks - i > CHUNKS(chunkLowMark) ||
               used_chunks > CHUNKS(chunkCriticalMark) ||
               publicObjectCount > publicObjectLowMark)) {
            ObjectPtr next_object = object->previous;
            if(object->refcount == 0) {
                i += object->numchunks;
                writeoutToDisk(object, object->size, -1);
                privatiseObject(object, 0);
            } else if(all || force) {
                writeoutToDisk(object, object->size, -1);
                destroyDiskEntry(object, 0);
            }
            object = next_object;
        }

        object = object_list_end;
        if(force || used_chunks > CHUNKS(chunkCriticalMark)) {
            if(used_chunks > CHUNKS(chunkCriticalMark)) {
                do_log(L_WARN, 
                       "Short on chunk memory -- "
                       "attempting to punch holes "
                       "in the middle of objects.\n");
            }
            while(object && 
                  (force || used_chunks > CHUNKS(chunkCriticalMark))) {
                if(force || (object->flags & OBJECT_PUBLIC)) {
                    int j;
                    for(j = object->numchunks - 1; j >= 0; j--) {
                        if(object->chunks[j].locked)
                            continue;
                        if(object->chunks[j].size < CHUNK_SIZE)
                            continue;
                        writeoutToDisk(object, (j + 1) * CHUNK_SIZE, -1);
                        dispose_chunk(object->chunks[j].data);
                        object->chunks[j].data = NULL;
                        object->chunks[j].size = 0;
                    }
                }
                object = object->previous;
            }
        }
        event = scheduleTimeEvent(2, discardObjectsHandler, 0, NULL);
        if(event) {
            objectExpiryScheduled = 1;
        } else {
            objectExpiryScheduled = 0;
            do_log(L_ERROR, "Couldn't schedule object expiry.\n");
        }
    } else {
        objectExpiryScheduled = 0;
    }

    if(all) {
        if(privateObjectCount + publicObjectCount != 0) {
            do_log(L_WARN,
                   "Discarded all objects, "
                   "%d + %d objects left (%d chunks and %d atoms used).\n",
                   publicObjectCount, privateObjectCount,
                   used_chunks, used_atoms);
        } else if(used_chunks != 0) {
            do_log(L_WARN,
                   "Discarded all objects, "
                   "%d chunks and %d atoms left.\n",
                   used_chunks, used_atoms);
        }
        diskIsClean = 1;
    }

    in_discardObjects = 0;
    return 1;
}