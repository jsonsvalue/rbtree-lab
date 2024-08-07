#include "rbtree.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void rb_insert_fixup(rbtree *t, node_t *z);
void rb_erase_fixup(rbtree *t, node_t *x);
void rbtree_transplant(rbtree* t, node_t* u, node_t *v);
node_t *find_min_successor(rbtree* t, node_t *y);
void delete_rbtree(rbtree *t);
void postorder_delete_rbtree(rbtree *t, node_t *x);

// #1 메모리: 트리 생성/삭제
rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  

  // TODO: initialize struct if needed
  p->nil = (node_t* )calloc(1, sizeof(node_t));
  p->root = p->nil;
  p->nil->color = RBTREE_BLACK;

  return p;
}

void delete_rbtree(rbtree *t) {
  postorder_delete_rbtree(t, t->root);
  free(t->nil);
  free(t);
  // TODO: reclaim the tree nodes's memory
}

void postorder_delete_rbtree(rbtree *t, node_t *x) {
  if (x != t->nil) {
    postorder_delete_rbtree(t, x->left);
    postorder_delete_rbtree(t, x->right);
    free(x);
  }
}

// #2 왼쪽 회전 / 오른쪽 회전
void left_rotate(rbtree* t, node_t* x){
  // 왼쪽으로 회전 시켜서 부모로 만들고자하는 y를 지정
  node_t* y = x->right;
  
  //x의 오른쪽 노드에, 회전하고자 하는 y의 왼쪽 노드를 삽입 
  x->right = y->left;

  //y의 왼쪽 자식이 닐이 아니라면, y의 왼쪽 자식의 부모를 x로 바꾼다.
  if(y->left != t->nil){
    y -> left -> parent = x ;
  }

  //y의 부모를 x의 부모로 지정해주는 작업
  //y가 위로 올라오면서, x의 부모를 y의 부모로 지정해준다.
  y->parent = x->parent;

  //x의 부모가 닐이라면, y는 루트 노드로 지정
  if( x-> parent == t->nil){
    t->root = y;
  }
  //x의 부모의 왼쪽 / 오른쪽 자식에 y가 오도록 한다.
  //x가 부모의 왼쪽 자식이었다면, y를 왼쪽 자식으로 선언
  //x가 부모의 오른쪽 자식이었다면, y를 오른쪽 자식으로 선언
  else if( x == x->parent->left){
    x->parent->left = y;
  }else{
    x->parent->right = y;
  }

  //y의 왼쪽 자식 노드에 x를 선언했다면, x의 부모 노드로 y를 따로 선언해야 하는가?
  //한 단계 올라간 y의 왼쪽 자식 노드에 x를 선언
  y->left = x;
  
  //x의 부모를 y로 선언
  x->parent = y;

}


void right_rotate(rbtree* t, node_t* x){
  //x의 왼쪽 자식을 y로 선언
  node_t* y = x->left;

  //x의 왼쪽 자식에 y의 오른쪽 자식을 삽입
  x->left = y->right;
  
  //y의 오른쪽 자식이 닐 노드가 아니라면, 해당 노드의 부모를 x로 지정한다. 
  if(y->right != t->nil){
    y->right->parent = x;
  }

  //y의 부모를 x의 부모로 지정해주는 작업
  //y가 위로 올라오면서, x의 부모를 y의 부모로 지정해준다.
  y->parent = x->parent;

  if(x->parent == t->nil){
    t->root = y;
  }else if(x == x->left){
    x->parent->left = y;
  }else{
    x->parent->right = y;
  }

  y->right = x;

  x->parent = y;

} 


// #3 노드 삽입
// node_t* rbtree_insert(rbtree *t, const key_t key) {
//   node_t *y = t->nil;
//   node_t *x = t->root;

//   // 1. 새로운 노드 z 를 위한 메모리 할당
//   node_t *z = (node_t *)calloc(1, sizeof(node_t));
//   if (z == NULL) {
//     return NULL; // Memory allocation failed
//   }
//   z->key = key;
//   z->left = t->nil;
//   z->right = t->nil;
//   z->color = RBTREE_RED;

//   // 2. 삽입할 위치를 찾음
//   while (x != t->nil) {
//     y = x;
//     if (key < x->key) {
//       x = x->left;
//     } else {
//       x = x->right;
//     }
//   }

//   z->parent = y;
//   if (y == t->nil) {
//     t->root = z;
//   } else if (z->key < y->key) {
//     y->left = z;
//   } else {
//     y->right = z;
//   }

//   // 3. 삽입 후 속성을 유지하도록 수정
//   rb_insert_fixup(t, z);

//   return z; // Return the inserted node instead of the root
// }



node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t* y = t->nil;
  node_t* x = t->root;
  node_t* z = (node_t *)calloc(1, sizeof(node_t));
  if (z == NULL) {
    return NULL; // Memory allocation failed
  }

  // 반복문을 통해서 닐 노드에 도달했다면 y는 닐 노드를 가리키고 있고, 새로운 노드 z 를 삽입하기 위한 메모리 동적 할당하면 됨
  // 그리고 해당 노드 y에 새로운 노드인 z를 자식 노드로 삽입
  z -> key = key;

  // 리프 노드에 z를 삽입하면, 왼쪽/오른쪽 노드는 모두 nil로 설정하고 색깔은 빨간색으로 지정한다.
  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;

  // 닐 노드에 도달할 때까지 반복문 실행
  while( x != t->nil){
    // 첫 닐 노드에 루트 노드 삽입
    y = x;
    // 삽입하려는 노드의 값이 루트 노드보다 작다면 루트 노드의 왼쪽 자식을 확인
    if( x->key > key ){
      x = x->left;
    }else{
      x = x->right;
    }
  }

  z -> parent = y;
  // While문을 한번도 돌지 않으면, 루트 노드에 바로 z를 삽입
  if(y== t->nil){
    t->root = z;
  // 삽입하려는 노드의 값이 y의 값보다 더 작으면, 왼쪽에 삽입
  }else if( z->key < y->key ){
    y -> left = z;
  // 삽입하려는 노드의 값이 y의 값보다 더 크다면, 오른쪽에 삽입
  }else{
    y -> right = z;
  }

  rb_insert_fixup(t,z);

  return z;
}


//#4 노드 삭제 / 이식
int rbtree_erase(rbtree *t, node_t *z) {
  // TODO: implement erase
  printf("Erasing node with key %d\n", z->key);
  node_t *y = z;                      //z는 삭제할 노드, y는 z 대신 들어갈 노드의 주소

  color_t y_org_color = y->color;     //자녀의 개수가 없거나 하나인 경우, 대체할 노드 y의 색깔이 z 대신에 들어갈 수 있기 때문에 따로 저장해둔다

  node_t *x;                          //x는 y의 자식으로, 후임자 y의 자식을 저장한 것

  if(z->left == t->nil){              //z의 자식이 오른쪽 자식 1개뿐일 경우, z 대신에 z의 오른쪽 자식을 이식
    x = z->right;
    rbtree_transplant(t,z,z->right);
  }else if(z->right == t->nil){       //z의 자식이 왼쪽 자식 1개뿐일 경우, z 대신에 z의 왼쪽 자식을 이식
    x = z->left;
    rbtree_transplant(t,z,z->left);
  }else{
    y = find_min_successor(t, z->right);//z 의 자식이 2개일 경우 확인
    y_org_color = y->color;             //삭제되는 후계자 y의 색을 저장 
    x = y->right;                       //y가 삭제될 것이기 때문에, y의 자리에 x가 대체 노드로 올라와야 한다

    if(y->parent == z){                 //z가 삭제되면, y의 부모가 z라면 x의 부모가 y인 것을 선언
      x->parent = y;
    }else{
      rbtree_transplant(t, y, y->right);//z에 y 를 심기 전에, y의 자리에 y의 자식을 삽입
      y->right = z->right;              
      y->right->parent = y;             
    }

    rbtree_transplant(t, z, y);         //z에 y를 넣는다. 코드 상 z의 부모가 y를 가리키도록 한다
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;                //y의 왼쪽 자리에 z의 색깔을 삽입
                                        //자녀가 2개일 경우 삭제할 때, 삭제하려는 노드의 색깔에 후계자의 데이터를 삽입
  }

  // 만약 위에서 삭제한 노드의 색깔이 빨간색이라면, 닐 노드까지 검은색 노드의 수가 똑같기 때문에 따로 조정이 필요 없다.
  // 하지만 검은색 노드를 삭제한 경우에는 닐노드까지 검은색 노드의 수가 달라지기 때문에, 노드 자리에 대한 조정이 필요하다.
  if (y_org_color == RBTREE_BLACK){   
    rb_erase_fixup(t,x);
  }

  free(z);                                // z가 삭제되고 y가 그 자리를 대체하기 때문에, 동적할당된 z를 할당 해제 해준다.
                           

  return 0;
}

// 이식
// 삭제하고자 하는 노드를 대체할 노드를 찾아서, 삭제한 자리에 해당 노드를 삽입하는 함수
// 노드 u 자리에 노드 v를 삽입
// u의 부모가 v를 가리키게 됨

void rbtree_transplant(rbtree* t, node_t* u, node_t *v){
  printf("Transplanting node with key %d to node with key %d\n", u->key, v->key);
  if (u->parent == t->nil){
    t->root = v;
  }
  //부모 기준 u가 왼쪽/오른쪽 자식인 경우를 확인해서, 부모 노드를 노드 v의 부모로 지정 
  else if(u == u->parent->left){
    u->parent->left = v;
  }else{
    u->parent->right = v;
  }

  v->parent = u->parent;

}

// 해당 트리에서 오른쪽 서브트리의 최소값을 구하기 위해서 쓰는 함수, node 매개변수에 오른쪽 트리 노드를 인자로 넣으면 된다.
// 자식이 2개인 경우, 오른쪽 서브 트리의 최소값인 후계자를 찾아서 z에 넣기 위해서 쓰는 함수이다.
node_t *find_min_successor(rbtree* t, node_t *y){
  
  while(y->left != t->nil){
    y = y->left;
  }

  return y;
}



// #5 노드 탐색
node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  if(t->root == t ->nil){
    return NULL;
  }

  node_t* x = t->root;
  
  while(x != t->nil){
    if(x->key > key){
      x = x->left;
    }
    else if( x->key < key){
      x = x->right;
    }
    else{
      return x;
    }

  }

  return NULL;
}


//#6 전체 트리의 min/max 노드 찾기
// 최소값을 가진 노드를 탐색 후 해당 노드 반환
node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t* x = t->root;

  while(x->left != t->nil){
    x = x->left;
  }

  return x;
}

// 최대값을 가진 노드를 탐색 후 해당 노드 반환
node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t* x = t->root;

  while(x->right != t->nil){
    x = x -> right;
  }

  return x;
}


// #7 삽입/삭제 fixup
// 노드 z를 삽입하고 나서, 노드 z의 위치를 재조정하기 위한 rb_insert_fixup 함수를 실행함. 
void rb_insert_fixup(rbtree *t, node_t *z){
// 특성 4. 적색 노드는 적색 노드를 자식으로 가질 수 없다는 조건을 만족해야 함.
  //z의 부모가 빨간색이 아닐때까지 반복문 실행
  while(z->parent->color == RBTREE_RED){
    //1) z의 부모가 z 조부모의 왼쪽 자식인 경우를 확인
    if(z->parent == z->parent->parent->left){
      node_t* uncle = z->parent->parent->right;

      //A. 삼촌의 색깔이 빨간색일 경우, 부모와 삼촌의 색깔을 모두 검은색으로 바꾸고 조부모의 색깔을 빨간색으로 바꾼다.
      if (uncle->color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;

        z = z->parent->parent;
      }
      //B. 삼촌의 색깔이 검은색일 경우, z, z의 부모, z의 조부모가 일직선 상에 위치하는지 확인해야 한다.
      else{
        //z, z의 부모, z의 조부모가 일직선 상에 위치하지 않는다면, 일직선 상에 위치하도록 회전시킨다.
        if(z==z->parent->right){
          z = z->parent;
          left_rotate(t, z); // z의 부모를 기준으로 왼족 회전을 해서, z, z의 부모, z의 조부모를 일직선상으로 만들어준다.
        }

        //z, z의 부모, z의 조부모가 일직선 상에 위치한다면, 색깔 조정 후 z의 조부모를 기준으로 회전시킨다.
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t, z->parent->parent);
      }
    }
    //2) z의 부모가 z 조부모의 오른쪽 자식인 경우를 확인
    else{
      node_t* uncle = z->parent->parent->left;
      
      //위의 경우와 동일하게 삼촌이 빨간색이면, 부모와 삼촌의 색깔을 모두 검은색으로 바꾸고 조부모의 색깔을 빨간색으로 바꾼다.
      if(uncle->color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;

        z = z->parent->parent;
      }else{
      //z, z의 부모, z의 조부모가 일직선 상에 위치하도록 회전시킨다.
        if(z == z->parent->left){
          z= z->parent;
          right_rotate(t,z);
        }

        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t, z->parent->parent);
      }
    } 
  }
// 특성 2. 루트 노드가 흑색이어야 한다는 조건을 만족해야 함.
  t->root->color = RBTREE_BLACK;
}




//노드가 검은색인 경우를 삭제할 때 임의의 노드에서 닐노드까지의 수가 바뀌는 것 때문에, 노드에 대한 조정 작업이 필요해
//x의 형제, 형제의 자식, 부모 노드의 색깔에 따른 경우를 분류해서, 노드에 대한 조정 작업 진행
void rb_erase_fixup(rbtree *t, node_t *x){
  printf("Fixing up node with key %d\n", x->key);
  node_t* b;                          //x의 형제 노드를 b로 주소 할당

  //x가 루트가 아니고 x가 doubly black 일 경우를 확인
  while( x!= t->root && x->color == RBTREE_BLACK){
  //doubly black이 왼쪽 자식일 경우를 확인
  //x가 부모의 왼쪽 자식, b가 형제로 지정
    if(x == x->parent ->left){
      b = x-> parent -> right;
      //Case 4. 경우 2~4로 위임 가능
      //부모의 왼쪽 자식을 빨간색으로 만드는 것을 목표로 함
      if(b->color == RBTREE_RED){
        b->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);                //부모를 기준으로 왼쪽으로 회전하고, 기존 x 형제의 자식을 부모의 오른쪽 자식으로 선언  
        b = x->parent->right;
      }

      //Case 3. 경우 1~4로 위임 가능
      //위의 if 문을 거치지 않았기 때문에, b인 형제의 색깔이 검은색인 경우를 확인

      //x의 형제인 b의 자녀가 모두 검은색일 때를 확인
      //x의 extra black을 x의 부모로 넘겨서, x는 검은색 / b는 빨간색으로 처리
      if(b->left->color == RBTREE_BLACK && b->right->color == RBTREE_BLACK){
        b->color = RBTREE_RED;
        x = x->parent;
      }else{
        //Case 2. 경우 4로 위임 가능
        //x의 형제인 b의 자녀 중 오른쪽 노드가 검은색 일때를 확인
        //x의 형제를 빨간색 / x의 형제인 b의 자녀 중 왼쪽 노드를 검은색으로 바꾸고, x의 형제를 기준으로 오른쪽 회전
        if(b->right->color == RBTREE_BLACK){
          b->left->color = RBTREE_BLACK;
          b->color = RBTREE_RED;
          right_rotate(t,b);
          b = x->parent->right;
        }

        //Case 1. 다른 경우로 위임되지 않고, 해당 경우에서 해결이 된다.
        b->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        b->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t->root;
      }

    }
    //doubly black인 x가 오른쪽 자식일 때를 확인
    else{
      b = x->parent->left;
      
      //Case4.
      if(b->color == RBTREE_RED){
        b->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        b = x->parent->left;
      }

      //Case3.
      if(b->right->color == RBTREE_BLACK && b->left->color == RBTREE_BLACK){
        b->color = RBTREE_RED;
        x = x->parent;
      }else{

      //Case2.
        if(b->left->color == RBTREE_BLACK){
          b->right->color = RBTREE_BLACK;
          b->color = RBTREE_RED;
          left_rotate(t, b);
          b = x->parent->left;
        }      

      //Case1.
        b->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        b->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root;
      }
    }
  }

  x->color = RBTREE_BLACK;  // x가 루트 노드이거나, red and black이면 해당 노드를 만나서 검은색 노드로 바뀌게 된다.
}





// #8 [중위 순회]
void inorder_rbtree_to_array(const rbtree *t, node_t *x, key_t *arr, int *idx, const size_t n) {
  if (x == t->nil) {
    return;
  }
  
  inorder_rbtree_to_array(t, x->left, arr, idx, n);
  if (*idx < n) {
    arr[(*idx)++] = x->key;                           // *idx는 0, 1, 2, 3...이다. 그리고 후위 연산자 ++이므로 0부터 인덱스가 시작된다.
  } else {
    return;
  }
  inorder_rbtree_to_array(t, x->right, arr, idx, n);
}

// [오름차순으로 배열에 값 저장]
// rbtree를 가르키는 t 값(주소값)은 변하면 안되므로 const
// arr는 inorder 함수를 거칠 때 마다 변할 수 있으므로 const 없이
// n은 arr의 사이즈로 변하면 안되므로 const를 붙여준다. size_t는 unsigned int로, 어차피 배열의 사이즈 n은 양수일테니 size_t로 선언
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  node_t *x = t->root;
  if (x == t->nil) {
    return 0;
  }
  int cnt = 0;
  int *idx = &cnt;
  inorder_rbtree_to_array(t, x, arr, idx, n);
  
  // TODO: implement to_array
  return 0;
}

// [중위 순회] : 확인용
// 중위 순회로 트리를 탐색하며 오름차순으로 키 값을 출력하고 각 노드에 해당하는 색깔을 같이 출력한다.
void inorder_tree_work(rbtree *t, node_t *x) {
  if (x != t->nil) {
    inorder_tree_work(t, x->left);
    printf("색깔 : %d, 키 값 : %d\n", x->color, x->key);
    inorder_tree_work(t, x->right);
  }
}


// // #8 트리를 배열로 변환 -> inorder traversing으로 구현!!! 
// // traversing 순서: node, node->left, node->right

// int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
//   // TODO: implement to_array


//   return 0;
// }

