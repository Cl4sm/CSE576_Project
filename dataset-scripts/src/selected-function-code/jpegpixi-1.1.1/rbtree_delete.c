void
rbtree_delete (struct rbtree *const tree, void *const data)
{
    struct rbtree_node *const node = DATA2NODE (data);
    struct rbtree_node *del_node, *tmp, *tmp_p;


    del_node = node;

    if (del_node->r == 0)
        tmp = del_node->l;
    else
    {
        if (del_node->l != 0)
        {
            del_node = del_node->r;
            while (del_node->l != 0)
                del_node = del_node->l;
        }

        tmp = del_node->r;
    }

    if (del_node != node)
    {
        node->l->p = del_node;
        del_node->l = node->l;

        if (del_node != node->r)
        {
            tmp_p = del_node->p;
            if (tmp != 0)
                tmp->p = del_node->p;
            del_node->p->l = tmp;
            del_node->r = node->r;
            node->r->p = del_node;
        }
        else
            tmp_p = del_node;

        if (tree->root == node)
            tree->root = del_node;
        else if (node->p->l == node)
            node->p->l = del_node;
        else
            node->p->r = del_node;

        del_node->p = node->p;

        {
            const enum rbtree_color tmp_color = del_node->color;
            del_node->color = node->color;
            node->color = tmp_color;
        }

        del_node = node;
    }
    else
    {
        tmp_p = node->p;

        if (tmp != 0)
            tmp->p = node->p;

        if (tree->root == node)
            tree->root = tmp;
        else if (node->p->l == node)
            node->p->l = tmp;
        else
            node->p->r = tmp;
    }

    if (del_node->color == BLACK)
    {
        while (tmp != tree->root && (tmp == 0 || tmp->color == BLACK))
            if (tmp == tmp_p->l)
            {
                struct rbtree_node *tmp_p_r = tmp_p->r;

                if (tmp_p_r->color != BLACK)
                {
                    tmp_p_r->color = BLACK;
                    tmp_p->color = RED;
                    rbtree_rot_l (tree, tmp_p);
                    tmp_p_r = tmp_p->r;
                }

                if ((tmp_p_r->l == 0 || tmp_p_r->l->color == BLACK)
                    && (tmp_p_r->r == 0 || tmp_p_r->r->color == BLACK))
                {
                    tmp_p_r->color = RED;
                    tmp = tmp_p;
                    tmp_p = tmp->p;
                }
                else
                {
                    if (tmp_p_r->r == 0 || tmp_p_r->r->color == BLACK)
                    {
                        tmp_p_r->l->color = BLACK;
                        tmp_p_r->color = RED;
                        rbtree_rot_r (tree, tmp_p_r);
                        tmp_p_r = tmp_p->r;
                    }

                    tmp_p_r->color = tmp_p->color;
                    tmp_p->color = BLACK;
                    if (tmp_p_r->r != 0)
                        tmp_p_r->r->color = BLACK;
                    rbtree_rot_l (tree, tmp_p);
                    break;
                }
            }
            else
            {
                struct rbtree_node *tmp_p_l = tmp_p->l;

                if (tmp_p_l->color != BLACK)
                {
                    tmp_p_l->color = BLACK;
                    tmp_p->color = RED;
                    rbtree_rot_r (tree, tmp_p);
                    tmp_p_l = tmp_p->l;
                }

                if ((tmp_p_l->l == 0 || tmp_p_l->l->color == BLACK)
                    && (tmp_p_l->r == 0 || tmp_p_l->r->color == BLACK))
                {
                    tmp_p_l->color = RED;
                    tmp = tmp_p;
                    tmp_p = tmp->p;
                }
                else
                {
                    if (tmp_p_l->l == 0 || tmp_p_l->l->color == BLACK)
                    {
                        tmp_p_l->r->color = BLACK;
                        tmp_p_l->color = RED;
                        rbtree_rot_l (tree, tmp_p_l);
                        tmp_p_l = tmp_p->l;
                    }

                    tmp_p_l->color = tmp_p->color;
                    tmp_p->color = BLACK;
                    if (tmp_p_l->l != 0)
                        tmp_p_l->l->color = BLACK;
                    rbtree_rot_r (tree, tmp_p);
                    break;
                }
            }

        if (tmp != 0)
            tmp->color = BLACK;
    }

    free (del_node);
}