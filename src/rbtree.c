#include "rbtree.h"
#include <malloc.h>

// nil 생성
node_t make_nil = {
    .color = RBTREE_BLACK,
    .left = NULL,
    .right = NULL,
    .parent = NULL
};
node_t *nil = &make_nil;

// 트리 생성
rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(sizeof(rbtree), 1);  // rbtree의 인자를 1개 메모리 할당(할당된 공간의 값을 모두 0으로 초기화)
  return p;
}

// RBtree 구조체가 차지했던 메모리 반환
void delete_rbtree(rbtree *t) {
  if (t->root != nil) {
    delete_rbtree(t->root->left);
    delete_rbtree(t->root->right);
    free(t);
  }
}

// 좌회전 함수
node_t *left_rotate(node_t *t, node_t *base) {
  node_t *y = base->right;
  base->right = y->left;

  if (y->left != nil) {
    y->left->parent = base;  // 왼쪽으로 돌리기 때문에 y의 작은 수는 base 자식으로 옮겨짐
  }

  y->parent = base->parent;  // 부모 노드 옮기기

  if (base->parent == nil) {  // base->parent가 없다면 y가 root
    t = y;
  }
  else if (base == base->parent->left) {  // base가 왼쪽에 위치한다면
    base->parent->left = y;
  }
  else {
    base->parent->right = y;
  }

  y->left = base;
  base->parent = y;

  return t;
}

// 우회전 함수
node_t *right_rotate(node_t *t, node_t *base) {
  node_t *y = base->left;
  base->left = y->right;

  if (y->right != nil) {
    y->right->parent = base;
  }

  y->parent = base->parent;

  if (base->parent == nil) {
    t = y;
  }
  else if (base == base->parent->right) {
    base->parent->right =y;
  }
  else {
    base->parent->left = y;
  }
  y->right = base;
  base->parent = y;

  return t;
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
  t->root->parent = NULL;
  t->root->color = RBTREE_BLACK;  // 루트 색은 검정!
  return t->root;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t *y = nil;
  node_t *x = t->root;

  // 새로 들어올 노드 생성
  node_t *n = (node_t *)calloc(1, sizeof(node_t));

  while (x != NULL) {  // 노드 n이 들어갈 위치 찾는 과정
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
  n->color = RBTREE_RED;
  n->key = key;
  n->left = NULL;
  n->right = NULL;

  return insert_fixup(t, n);
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

node_t *rbtree_min(const node_t *t) {
  node_t *tmp;
  
  while (tmp->left != nil) {
    tmp = tmp->left;
  }
  return tmp;
}

node_t *rbtree_max(const rbtree *t) {
  node_t *tmp = t->root;

  while (tmp->right != nil) {
    tmp = tmp->right;
  }

  return tmp;
}

node_t *tree_successor(node_t *p) {
  node_t *tmp;
  if (p->right != nil) {
    return rbtree_min(p->right);  // tmp를 오른쪽 서브트리 중 제일 작은 값 반환
  }

  tmp = p->parent;

  while ((tmp != nil) && (p == tmp->right)) {
    p = tmp;
    tmp = tmp->parent;
  }
  return tmp;
}

node_t *erase_fixup(rbtree *t, node_t * p) {
  node_t *w;  // p노드의 형제 노드
  while ((p != t->root) && (p->color == RBTREE_BLACK)) {
    if (p == p->parent->left) {  // p노드가 왼쪽에 있다면
      w = p->parent->right;
      if (w->color == RBTREE_RED) {  //  case1: 형제 노드가 red일 경우
        w->color = RBTREE_BLACK;
        p->parent->color = RBTREE_RED;
        t->root = left_rotate(t->root, p->parent);
        w = p->parent->right;
      }
      if ((w->left->color == RBTREE_BLACK) && (w->right->color == RBTREE_BLACK)) {  // case2: 형제 노드 'black', 형제 노드의 자식 노드 모두 'black'
        w->color = RBTREE_RED;  // 색상 전환
        p = p->parent;
      }
      else {
        if (w->right->color == RBTREE_BLACK) {  // case3: 형제 노드 'black', 형제 좌노드 'red', 형제 우노드 'black'
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          t->root = right_rotate(t->root, w);  // 단일 회전
          w = p->parent->right;
        }
        w->color = p->parent->color;
        p->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        t->root = left_rotate(t->root, p->parent);
        p = t->root;
      }
    }
    else {
      w = p->parent->left;
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        p->parent->color = RBTREE_RED;
        t->root = right_rotate(t->root, p->parent);
        w = p->parent->left;
      }
      if ((w->right->color == RBTREE_BLACK) && (w->left->color == RBTREE_BLACK)) {
        w->color = RBTREE_RED;
        p = p->parent;
      }
      else {
        if (w->left->color == RBTREE_BLACK) {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          t->root = left_rotate(t->root, w);
          w= p->parent->left;
        }
        w->color = p->parent->color;
        p->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        t->root = right_rotate(t->root, p->parent);
        p = t->root;
      }
    }
  }
  p->color = RBTREE_BLACK;
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  node_t *y;  // 삭제되거나 이동될 노드
  node_t *x;  // y의 원래 위치(삭제 or 이동되기 전 위치)

  if ((p->left == nil) || (p->right == nil)) {  // 자식 노드 하나만 가지고 있거나 둘 다 nil일 때
    y = p;  // 자식이 하나라도 nil이면 삭제할 노드 p 대입
  }
  else {  // 자식 노드 둘 다 있을 때
    y = tree_successor(p);
  }
  if (y->left != nil) {
    x = y->left;
  }
  else {
    x = y->right;
  }

  x->parent = y->parent;  // x의 부모 노드를 y의 부모 노드로 대입
  if (y->parent == nil) {
    t->root = x;  // y의 부모 노드가 없다면 x가 루트가 됨
  }
  else {  // 원래 y의 자리에 x 넣기
    if (y == y->parent->left) {
      y->parent->left = x;
    }
    else {
      y->parent->right = x;
    }
  }

  if (y != p) {  // 석세서로 p와 y가 같지 않다면, p값에 y값 대입
    p->key = y->key;
  }
  if (y->color == RBTREE_BLACK) {
    t->root = erase_fixup(t, x);
  }
  
  free(p);
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
