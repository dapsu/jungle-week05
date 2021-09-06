#include "rbtree.h"

#include <malloc.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(sizeof(rbtree), 1);
  return p;
}
#include "rbtree.h"
#include <malloc.h>

rbtree *new_rbtree(void) {  // void 함수로 만드는 것하고 무슨 차이?
  rbtree *p = (rbtree *)calloc(sizeof(rbtree), 1);  // rbtree의 인자를 1개 메모리 할당(할당된 공간의 값을 모두 0으로 초기화)
  return p;
}

// nil 생성
node_t *nil() {
  node_t *tmp;
  tmp = (node_t *)malloc(sizeof(node_t));
  tmp->key = 0;
  tmp->left = NULL;
  tmp->right = NULL;
  tmp->parent = NULL;
  tmp->color = RBTREE_BLACK;
  return tmp;
};

// 좌회전 함수
node_t *left_rotate(rbtree *t, node_t *base) {
  node_t *y = base->right;
  base->right = y->left;

  if (y->left != nil) {
    y->left->parent = base;  // 왼쪽으로 돌리기 때문에 y의 작은 수는 base 자식으로 옮겨짐
  }

  y->parent = base->parent;  // 부모 노드 옮기기

  if (base->parent == nil) {  // base->parent가 없다면 y가 root
    t->root = y;
  }
  else if (base == base->parent->left) {  // base가 왼쪽에 위치한다면
    base->parent->left = y;
  }
  else {
    base->parent->right = y;
  }

  y->left = base;
  base->parent = y;

  return t->root;
}

// 우회전 함수
node_t *right_rotate(rbtree *t, node_t *base) {
  node_t *y = base->left;
  base->left = y->right;

  if (y->right != nil) {
    y->right->parent = base;
  }

  y->parent = base->parent;

  if (base->parent == nil) {
    t->root = y;
  }
  else if (base == base->parent->right) {
    base->parent->right =y;
  }
  else {
    base->parent->left = y;
  }
  y->right = base;
  base->parent = y;

  return t->root;
}

void delete_rbtree(rbtree *t) {
  

  free(t);
}

node_t *rbtree_insert(rbtree *t, key_t key) {
  node_t *y = nil;
  node_t *x = t->root;
  
  // 새로 들어올 노드 생성
  node_t *n = (node_t *)calloc(1, sizeof(node_t));
  n->color = RBTREE_RED;
  n->key = key;
  n->left = nil;
  n->right = nil;

  while (x != nil) {  // 노드 n이 들어갈 위치 찾는 과정
    y = x;  // y는 n의 부모 노드
    if (key < x->key) {
      x = x->left;
    }
    else {
      x = x->right;
    }
  }
  n->parent = y;
  
  if (y == nil) {  // x가 0이었다면(트리가 없었다면)
    t->root = n;
  }
  else if (key < y->key) {
    y->left = n;
  }
  else {
    y->right = n;
  }

  return insert_fixup(t->root, n);
}

node_t *insert_fixup(rbtree *t, node_t *n) {
  node_t *y;

  while (n->parent->color == RBTREE_RED) {  // 부모 노드가 RED면
    if ((n->parent) == (n->parent->parent->left)) {  // 부모 노드가 조부모 노드의 왼쪽에 있다면
      y = (n->parent->parent)->right;  // 삼촌 노드
      
      if (y->color == RBTREE_RED) {  // 부모, 삼촌 다 RED
        n->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        n->parent->parent->color = RBTREE_RED;
        n = n->parent->parent;  // 균열이 생기는 지점(조부모 노드가 레드로 변하면서 조건을 깰 수 있기 때문!)
      }
      else {  // 부모 노드 RED, 삼촌 노드 BLACK
        if (n == n->parent->right) {  // 현재 노드가 오른쪽에 위치한다면(부모보다 큰 수)
          n = n->parent;
          t->root = left_rotate(t->root, n);
        }
        n->parent->color = RBTREE_BLACK;
        n->parent->parent->color = RBTREE_RED;
        t->root = right_rotate(t->root, (n->parent->parent));
      }
    }
    else {  // 부모 노드가 조부모 노드의 오른쪽에 있다면
      y = n->parent->parent->left;  // 삼촌 노드

      if (y->parent == RBTREE_RED) {  // 부모, 삼촌 다 RED
        n->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        n->parent->parent->color = RBTREE_RED;
        n = n->parent->parent;
      }
      else {
        if (n == n->parent->left) {  // 현재 노드가 왼쪽에 위치한다면
          n = n->parent;  // 로테이션 할 기준 변경
          t->root = right_rotate(t->root, n);
        }
        n->parent->color = RBTREE_BLACK;
        n->parent->parent->color = RBTREE_RED;
        t->root = left_rotate(t->root, n->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;  // 루트 색은 검정!
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *find_node;
  find_node = t->root;

  while ((find_node != nil) && (key != find_node->key)) {
    if (key < find_node->key) {
      find_node = find_node->left;
    }
    else {
      find_node = find_node->right;
    }
  }
  return find_node;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(const rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

node_t *rbtree_insert(const rbtree *t, const key_t key) {
  // TODO: implement insert
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(const rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
