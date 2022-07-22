//
// rb_tree --> 红黑树
// Created by qiuzelin1 on 2022/7/15.
//

#include "rb_tree.h"

/**
 * 左翻转
 * @param node
 * @param root
 */
static void rb_rotate_left(struct rb_node *node, struct rb_root *root) {
    //获取该节点的右节点
    struct rb_node *right = node->right;
    //获取该节点的父节点
    struct rb_node *parent = rb_parent(node);

    if ((node->right = right->left))
        rb_set_parent(right->left, node);

    right->left = node;
    rb_set_parent(node, right);

    if (parent) {
        if (parent->left == node)
            parent->left = right;
        else
            parent->right = right;
    } else {
        root->root = right;
    }
    rb_set_parent(right, parent);
}


/**
 * 右翻转
 * @param node
 * @param root
 */
static void rb_rotate_right(struct rb_node *node, struct rb_root *root) {
    //获取该节点的左节点
    struct rb_node *left = node->left;
    //获取该节点的父节点
    struct rb_node *parent = rb_parent(node);

    if ((node->left = left->right))
        rb_set_parent(left->right, node);

    left->right = node;
    rb_set_parent(node, left);

    if (parent) {
        if (parent->left == node)
            parent->left = left;
        else
            parent->right = left;
    } else {
        root->root = left;
    }
    rb_set_parent(left, parent);
}


/**
 *
 * @param node
 * @param root
 */
void rb_insert_color(struct rb_node *node, struct rb_root *root) {

    //父亲节点
    struct rb_node *parent;
    //祖父节点
    struct rb_node *gparent;

    while ((parent = rb_parent(node)) && rb_is_red(parent)) {
        gparent = rb_parent(parent);

        if (gparent->left == parent) {

            {
                register struct rb_node *uncle = gparent->right;
                if (uncle && rb_is_red(uncle)) {
                    rb_set_black(parent);
                    rb_set_black(uncle);
                    rb_set_red(gparent);
                    node = gparent;
                    continue;
                }
            }

            if (parent->right == node) {
                register struct rb_node *tmp;
                rb_rotate_left(parent, root);
                tmp = parent;
                parent = node;
                node = parent;
            }

            rb_set_black(parent);
            rb_set_red(gparent);
            rb_rotate_right(gparent, root);
        } else {
            /* gparent->right == parent */
            {
                register struct rb_node *uncle = gparent->left;
                if (uncle && rb_is_red(uncle)) {
                    rb_set_black(uncle);
                    rb_set_black(parent);
                    rb_set_red(gparent);
                    node = gparent;
                }
            }

            if (parent->left == node) {
                register struct rb_node *tmp;
                rb_rotate_right(parent, root);
                tmp = parent;
                parent = node;
                node = parent;
            }

            rb_set_black(parent);
            rb_set_red(gparent);
            rb_rotate_left(gparent, root);
        }
    }

    rb_set_black(root->root);
}

/**
 * 替换的节点是黑色节点时,删除之后破环了红黑树,这个函数是让红黑树重新回到平衡
 */
static void rb_erase_color(struct rb_node *node, struct rb_node *parent, struct rb_root *root) {

    struct rb_node *other;

    while ((!node || rb_is_black(node)) && node != root->root) {

        if (parent->left == node) {
            other = parent->right;
            if(rb_is_red(other)){
                rb_set_black(other);
                rb_set_red(parent);
                rb_rotate_left(parent,root);
                other = parent->right;
            }
            if((!other->left || rb_is_black(other->left)) && (!other->right || rb_is_black(other->right))){
                rb_set_black(other);
                node = parent;
                parent = rb_parent(node);
            }else{
                if(!other->right || rb_is_black(other->right)){
                    rb_set_black(other->left);
                    rb_set_red(other);
                    rb_rotate_right(other,root);
                    other = parent->right;
                }
                rb_set_black(other->right);
                rb_set_color(other, rb_color(parent));
                rb_set_black(parent);
                rb_rotate_left(parent,root);
                break;
            }
        } else {
            /* parent->right == node  */
            other = parent->left;
            if(rb_is_red(other)){
                rb_set_black(other);
                rb_set_red(parent);
                rb_rotate_right(parent, root);
                other = parent->left;
            }
            if((!other->left || rb_is_black(other->left)) && (!other->right || rb_is_black(other->right))){
                rb_set_red(other);
                node = parent;
                parent = rb_parent(node);
            }else{
                if(!other->left || rb_is_black(other->left)){
                    rb_set_black(other->right);
                    rb_set_red(other);
                    rb_rotate_left(other,root);
                    other = parent->left;
                }
                rb_set_black(other->left);
                rb_set_color(other, rb_color(parent));
                rb_set_black(other);
                rb_rotate_right(parent,root);
                break;
            }
        }
    }

    if(node){
        rb_set_black(root->root);
    }
}

/**
 * 删除节点
 * @param node
 * @param root
 */
void rb_erase(struct rb_node *node, struct rb_root *root) {
    //被删除的节点的父节点
    struct rb_node *parent;
    //替代的节点
    struct rb_node *child;
    //替代的节点的颜色
    int color;

    if (!node->left) {
        child = node->right;
    } else if (!node->right) {
        child = node->left;
    } else {
        struct rb_node *old = node, *left;
        node = node->right;
        while ((left = node->left))
            node = left;

        if (rb_parent(node)->left == old)
            rb_parent(node)->left = node;
        else
            rb_parent(node)->right == node;

        parent = rb_parent(node);
        child = node->left;
        color = rb_color(node);

        if (parent == old) {
            parent = node;
        } else {
            if (child) {
                parent->left = child;
            }
            rb_set_parent(child, parent);

            node->right = old->right;
            rb_set_parent(old->right, node);
        }
        node->rb_parent_color = old->rb_parent_color;
        node->left = old->left;
        rb_set_parent(old->left, node);

        goto color;
    }

    parent = rb_parent(node);

    color = rb_color(node);

    if (child) {
        rb_set_parent(child, parent);
    }

    if (parent) {
        if (parent->left == node)
            parent->left = child;
        else
            parent->right = child;
    } else {
        root->root = child;
    }
    color:
    if (color == RB_BLACK) {
        rb_erase_color(child, parent, root);
    }
}


/**
 * 找到最小的结点
 * @return
 */
struct rb_node *rb_first(const struct rb_root *root) {

    struct rb_node *n;
    n = root->root;
    if (!n) {
        return NULL;
    }
    while (n->left)
        n = n->left;

    return n;
}

/**
 * 找到最大的rb_node
 * @param root
 * @return
 */
struct rb_node *rb_last(const struct rb_root *root) {
    struct rb_node *n;
    n = root->root;
    if (!n) {
        return NULL;
    }
    while (n->right)
        n = n->right;
    return n;
}








