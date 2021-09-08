#include "rbtree.h"
#include <malloc.h>


void *insert_fixup(rbtree *, node_t *);
void *erase_fixup(rbtree *, node_t *);

// nil 생성
node_t make_nil = {
    .color = RBTREE_BLACK,
    .left = NULL,
    .right = NULL,
    .parent = NULL
};
node_t *nil = &make_nil;

node_t _NIL = {
    .color = RBTREE_BLACK,
    .left = NULL, // 변경되는 상황 없음
    .right = NULL, // 변경되는 상황 없음
    .parent = NULL, // 변경될 수 있음!
};
node_t *NIL = &_NIL;

// 트리 생성
rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));  // rbtree의 인자를 1개 메모리 할당(할당된 공간의 값을 모두 0으로 초기화)
  return p;
}

// RBtree 구조체가 차지했던 메모리 반환
void del_node(node_t * t) {
  if (t != NULL) {
    del_node(t->left);
    del_node(t->right);
  }
  free(t);
}

void delete_rbtree(rbtree *t) {
  del_node(t->root);
  free(t);
}

// 좌회전 함수
node_t *left_rotate(node_t *t, node_t *base) {
  node_t *y = base->right;
  base->right = y->left;

  if (y->left != NULL) {
    y->left->parent = base;  // 왼쪽으로 돌리기 때문에 y의 작은 수는 base 자식으로 옮겨짐
  }

  y->parent = base->parent;  // 부모 노드 옮기기

  if (base->parent == NULL) {  // base->parent가 없다면 y가 root
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

  if (y->right != NULL) {
    y->right->parent = base;
  }

  y->parent = base->parent;

  if (base->parent == NULL) {
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

void *insert_fixup(rbtree *t, node_t *n) {
  node_t *y;
  while (n->parent != NULL && n->parent->color == RBTREE_RED) {  // 부모 노드가 RED면
    if ((n->parent) == (n->parent->parent->left)) {  // 부모 노드가 조부모 노드의 왼쪽에 있다면
      y = (n->parent->parent)->right;  // 삼촌 노드
      
      if (y != NULL && y->color == RBTREE_RED) {  // 부모, 삼촌 다 RED
        n->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        n->parent->parent->color = RBTREE_RED;
        n = n->parent->parent;  // 균열이 생기는 지점(조부모 노드가 레드로 변하면서 조건을 깰 수 있기 때문!)
      }
      else {  // 부모 노드 RED, 삼촌 노드 BLACK
        if (n == n->parent->right) {  // 현재 노드가 오른쪽에 위치한다면(부모보다 큰 수)
          n = n->parent;
          left_rotate(t->root, n);
        }
        n->parent->color = RBTREE_BLACK;
        n->parent->parent->color = RBTREE_RED;
        right_rotate(t->root, n->parent->parent);
      }
    }
    else {  // 부모 노드가 조부모 노드의 오른쪽에 있다면
      y = n->parent->parent->left;  // 삼촌 노드
      if (y != NULL && y->parent == RBTREE_RED) {  // 부모, 삼촌 다 RED
        n->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        n->parent->parent->color = RBTREE_RED;
        n = n->parent->parent;
      }
      else {
        if (n == n->parent->left) {  // 현재 노드가 왼쪽에 위치한다면
          n = n->parent;  // 로테이션 할 기준 변경
          right_rotate(t->root, n);
        }
        n->parent->color = RBTREE_BLACK;
        n->parent->parent->color = RBTREE_RED;
        left_rotate(t->root, n->parent->parent);
      }
    }
  }
  // t->root->parent = NULL;
  t->root->color = RBTREE_BLACK;  // 루트 색은 검정!
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t *y = NULL;
  node_t *x = t->root;

  // 새로 들어올 노드 생성
  node_t *n = (node_t *)malloc(sizeof(node_t));
  n->color = RBTREE_RED;
  n->key = key;
  n->left = NULL;
  n->right = NULL;

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
  
  if (y == NULL) {  // x가 0이었다면(트리가 없었다면)
    t->root = n;
  }
  else if (key < y->key) {
    y->left = n;
  }
  else {
    y->right = n;
  }
  insert_fixup(t, n);

  return n;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *find_node;
  find_node = t->root;

  while (find_node != nil) {
    if (key == find_node->key) {
      return find_node;
    }
    if (key < find_node->key) {
      find_node = find_node->left;
    }
    else if (key > find_node->key) {
      find_node = find_node->right;
    }
    return NULL;
  }
}

node_t *rbtree_min(const rbtree *t) {
  node_t *tmp = t->root;
  
  while (tmp->left != NULL) {
    tmp = tmp->left;
  }
  return tmp;
}

node_t *rbtree_max(const rbtree *t) {
  node_t *tmp = t->root;

  while (tmp->right != NULL) {
    tmp = tmp->right;
  }
  return tmp;
}

/*rbtree의 특성 유지
  노드의 위치를 이동시키는 함수*/
void tree_transplant(rbtree *t, node_t *n1, node_t *n2) {
  if (n1->parent == NULL) {
    t->root = n2;
  }
  else if (n1 == n1->parent->left) {
    n1->parent->left = n2;
  }
  else {
    n1->parent->right = n2;
  }
  if (n2 != NULL){
    n2->parent = n1->parent;  // n1의 부모 노드와 n2의 부모 노드 연결
  }
}

// node_nil: x가 없을 때, 더미노드 nil 반환
node_t *node_nil(node_t *x) {
  if (x == NULL) {
    return nil;
  }
  else {
    return x;
  }
}

// successor
node_t *successor(rbtree *t, node_t *x) {
  if (x->right != NULL) {  // x의 우노드가 존재할 때, 서브트리의 최소값 찾기
    node_t *c = x->right;
    while (c->left != NULL) {
      c = c->left;
    }
    return c;
  }
  node_t *p = x->parent;
  while (p != NULL && x == p->right) {  // x의 우노드가 없을 때, 위로 올라가다 처음 오른쪽 위로 꺾였을 때 첫 노드
    x = p;
    p = p->parent;
  }
  return p;
}


// 노드 삭제 시 rbtree 특성 유지시키는 함수
void *erase_fixup(rbtree *t, node_t * x) {
  node_t *s;  // x노드의 형제 노드
  while (x != t->root && x->color == RBTREE_BLACK) {
    if (x == x->parent->left) {  // x노드가 왼쪽에 있다면
      s = x->parent->right;
      if (s->color == RBTREE_RED) {  //  case1: 형제 노드가 red일 경우
        s->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t->root, x->parent);
        s = x->parent->right;
      }
      if ((s->left->color == RBTREE_BLACK) && (s->right->color == RBTREE_BLACK)) {  // case2: 형제 노드 'black', 형제 노드의 자식 노드 모두 'black'
        s->color = RBTREE_RED;  // 색상 전환
        x = x->parent;
      }
      else {
        if (s->right->color == RBTREE_BLACK) {  // case3: 형제 노드 'black', 형제 좌노드 'red', 형제 우노드 'black'
          s->left->color = RBTREE_BLACK;
          s->color = RBTREE_RED;
          right_rotate(t->root, s);  // 단일 회전
          s = x->parent->right;
        }
        s->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        s->right->color = RBTREE_BLACK;
        left_rotate(t->root, x->parent);
        x = t->root;
      }
    }
    else {
      s = x->parent->left;
      if (s->color == RBTREE_RED) {
        s->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t->root, x->parent);
        s = x->parent->left;
      }
      if ((s->right->color == RBTREE_BLACK) && (s->left->color == RBTREE_BLACK)) {
        s->color = RBTREE_RED;
        x = x->parent;
      }
      else {
        if (s->left->color == RBTREE_BLACK) {
          s->right->color = RBTREE_BLACK;
          s->color = RBTREE_RED;
          left_rotate(t->root, s);
          s= x->parent->left;
        }
        s->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        s->left->color = RBTREE_BLACK;
        right_rotate(t->root, x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *p) {
  node_t *y = p;  // 삭제되거나 이동될 노드
  node_t *x;  // y의 원래 위치(삭제 or 이동되기 전 위치)
  color_t y_or_color = y->color;  // y의 원래 색

  if (p->left == NULL) {  // p노드의 좌노드가 없을 때
    x = node_nil(p->right);
    tree_transplant(t, p, x);
    free(p);
  }
  else if (p->right == NULL) {  // p노드의 우노드가 없을 때
    x = node_nil(p->left);
    tree_transplant(t, p, x);
    free(p);
  }
  else {  // p노드가 자식노드 모두를 가지고 있을 때
    y = successor(t, p);
    y_or_color = y->color;
    x = node_nil(y->right);

    if (y->parent == p) {
      x->parent = y;
    }
    else {
      tree_transplant(t, y, x);
      y->right = p->right;
      y->right->parent = y;
    }
    tree_transplant(t, p, y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
    free(p);
  }

  if (y_or_color == RBTREE_BLACK) {
    erase_fixup(t, x);
  }

  // 루트가 nil이 될 경우가 발생하기 때문에 nil과 연결 끊기
  if (t->root == nil) {
    t->root = NULL;
  }
  else if (nil->parent != NULL) {
    if (nil->parent->left == nil) {
      nil->parent->left = NULL;
    }
    else {
      nil->parent->right = NULL;
    }
  }
  nil->parent = NULL;

  return 1;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array

  return 0;
}