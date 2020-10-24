void *
rbtree_insert (struct rbtree *const tree, const void *const data, size_t data_len,
               int (*const compare) (const void *a, const void *b), int *const exists_ptr)
{
    struct rbtree_node *node;
    void *data_copy;


    {
        struct rbtree_node *node_p = 0, *tmp = tree->root;

        while (tmp != 0)
        {
            const int cmp = compare (data, NODE2DATA (tmp));

            node_p = tmp;

            if (cmp == 0)
            {
                if (exists_ptr != 0)
                    *exists_ptr = 1;
                return NODE2DATA (tmp);
            }
            else if (cmp < 0)
                tmp = tmp->l;
            else
                tmp = tmp->r;
        }

        node = malloc (SIZEOF_RBTREE_NODE + data_len);
        if (node == 0)
            return 0;

        data_copy = NODE2DATA (node);
        node->l = 0;
        node->r = 0;
        node->p = node_p;
        node->color = RED;
        memcpy (data_copy, data, data_len);
    }

    if (node->p == 0)
        tree->root = node;
    else if (compare (data, NODE2DATA (node->p)) < 0)
        node->p->l = node;
    else
        node->p->r = node;

    while (node != tree->root && node->p->color != BLACK)
        if (node->p == node->p->p->l)
        {
            if (node->p->p->r != 0 && node->p->p->r->color != BLACK)
            {
                node->p->p->r->color = BLACK;
                node->p->p->color = RED;
                node->p->color = BLACK;
                node = node->p->p;
            }
            else
            {
                if (node == node->p->r)
                {
                    node = node->p;
                    rbtree_rot_l (tree, node);
                }

                node->p->p->color = RED;
                node->p->color = BLACK;
                rbtree_rot_r (tree, node->p->p);
            }
        }
        else
        {
            if (node->p->p->l != 0 && node->p->p->l->color != BLACK)
            {
                node->p->p->l->color = BLACK;
                node->p->p->color = RED;
                node->p->color = BLACK;
                node = node->p->p;
            }
            else
            {
                if (node == node->p->l)
                {
                    node = node->p;
                    rbtree_rot_r (tree, node);
                }

                node->p->p->color = RED;
                node->p->color = BLACK;
                rbtree_rot_l (tree, node->p->p);
            }
        }

    tree->root->color = BLACK;
    if (exists_ptr != 0)
        *exists_ptr = 0;
    return data_copy;
}