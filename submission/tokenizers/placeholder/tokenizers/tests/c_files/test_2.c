void clear_bind(int key)
{
    for (int i = 0; i < NUM_FBINDINGS; i++) {
        if (nc->keys[i].bind == key) {
            logit("Key bound to %s overridden.\n", nc->keys[i].descr);
            nc->keys[i].bind = 0;
        }
    }
}
