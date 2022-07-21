//
// Created by qiuzelin1 on 2022/7/18.
//

#ifndef LEARN_C_RB_TREE_H
#define LEARN_C_RB_TREE_H

#define NULL 0

struct rb_node {

    //左子节点
    struct rb_node *left;

    //右子节点
    struct rb_node *right;

    /*
     *  颜色
     *  黑色为 1
     *  红色为 0
     *  默认为黑色
     */
#define RB_RED 0
#define RB_BLACK 1

    //此颜色是父节点指向本节点的树根的颜色
    unsigned long rb_parent_color;// 使用该成员rb_parent_color同时存储两种数据,一是父结点的地址,另一是此结点的着色
};

/**
 * 根结点
 */
struct rb_root{
    struct rb_node *root;
};

/**
 * 宏定义
 */
/**
 * 获取父节点地址
 */
#define rb_parent(r) ((struct rb_node *)((r)->rb_parent_color & ~3))
/**
 * 查看是红是黑
 */
#define rb_color(r) ((r)->rb_parent_color & 1)
/**
 * 判断是否为红色
 */
#define rb_is_red(r) (!rb_color(r))
/**
 * 判断是否为黑色
 */
#define rb_is_black(r) rb_color(r)

/**
 * 将颜色设置为红色
 */
#define rb_set_red(r) do{ (r)->rb_parent_color &= ~1; } while(0)

/**
 * 将颜色设置为黑色
 */
#define rb_set_black(r) do{  (r)->rb_parent_color |= 1; } while(0)

/**
 * 设置父节点位置
 * @param rb
 * @param p
 */
static inline void rb_set_parent(struct rb_node *rb,struct rb_node *p){
    rb->rb_parent_color = (rb->rb_parent_color & 3) | (unsigned  long)p;
}

/**
 * 设置颜色
 * @param rb
 * @param color
 */
static inline void rb_set_color(struct rb_node *rb,int color){
    rb->rb_parent_color = (rb->rb_parent_color & ~1) | color;
}

/**
 * 初始化红黑树结构
 */
#define RB_ROOT (struct rb_root){NULL,}

/**
 * 使得根结点为NULL
 */
#define RB_EMPTY_ROOT(root) ((root)->rb_node == NULL)

/**
 *
 */
#define RB_EMPTY_NODE(node) (rb_parent(node) == (node))

/**
 *
 */
#define RB_CLEAR_NODE(node) (rb_set_parent(node,node))


static inline void rb_link_node(struct rb_node *node,struct  rb_node *parent,struct rb_node ** rb_link){
    node->rb_parent_color = (unsigned long )parent;
    node->left = node->right = NULL;
    *rb_link = node;
}

typedef void (*rb_augment_f)(struct rb_node *node,void *data);


#endif //LEARN_C_RB_TREE_H
