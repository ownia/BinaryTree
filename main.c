#include <stdio.h>
#include <stdlib.h>

typedef char TElemType;
typedef int Status;

typedef struct BiTNode {
  TElemType data;
  struct BiTNode *lchild, *rchild;
}BiTNode, *BiTree;

typedef BiTree SElemType;
typedef struct{
    SElemType *bottom;
    SElemType *top;
    int stacksize;
}SqStack;
//构造一个空栈S
Status InitStack(SqStack *S) {
    S -> bottom = (SElemType *) malloc (100*sizeof(SElemType));
    if(!S -> bottom) {
      printf("error...\n");
      exit(0);
    }
    S -> top = S -> bottom;
    S -> stacksize = 100;
    return 1;
}
//销毁栈S
Status DestroyStack(SqStack *S) {
    if (!S) {
      printf("point error...\n");
      exit(0);
    }
    free(S -> bottom);
    return 1;
}
//把栈S置为空栈
Status ClearStack(SqStack *S){
  if (!S)
    return 0;
  S -> top = S -> bottom;
  return 1;
}
//判断栈S是否为空栈
Status StackEmpty(SqStack S){
  if (S.top == S.bottom)
      return 1;
  else
    return 0;
}
//返回栈S的stacksize
int StackLength(SqStack S){
    return S.stacksize;
}
//返回S的栈顶元素和栈的情况
Status GetTop(SqStack S, SElemType *e) {
  if (S.top == S.bottom) {
    return 0;
  } else {
    *e = *(S.top - 1);
    return 1;
  }
}
//插入元素e为新的栈顶元素
Status Push(SqStack *S, SElemType e) {
  if (S -> top - S -> bottom >= S -> stacksize) {
    S -> bottom = (SElemType *) realloc (S -> bottom, (S -> stacksize + 10) * sizeof(SElemType));
  if(!S -> bottom) {
    printf("error...\n");
    exit(0);
  }
  S -> top = S -> bottom + S -> stacksize;
  S -> stacksize += 10;
  }
  *S -> top++ =e;
  return 1;
}
//若栈S不为空，则删除S的栈顶元素，用e返回其值
Status Pop(SqStack *S, SElemType *e) {
  if(S -> top == S -> bottom) {
    return 0;
  }
  *e = *(--S -> top);
  return 1;
}
//对二叉树中的数据元素访问
Status Visit(TElemType e) {
  if (e == '\0') {
    return 0;
  } else {
    printf("%2c", e);
  }
  return 1;
}
//建立二叉树
Status CreateBiTree(BiTree *T) {
  char ch;
  ch = getchar();
  if (ch == '#') {
    (*T) = NULL;
  } else {
    (*T) = (BiTree) malloc(sizeof(BiTNode));
    (*T) -> data = ch;
    CreateBiTree(&(*T) -> lchild);
    CreateBiTree(&(*T) -> rchild);
  }
  return 1;
}

void PreOrder(BiTree T) {
  if (T) {
    printf("%2c", T -> data);
    PreOrder(T -> lchild);
    PreOrder(T -> rchild);
  }
}

Status PreOrderNonRecursion(BiTree T, Status (*Visit)(TElemType e)) {
  SqStack S;
  SElemType p;
  InitStack(&S);
  Push(&S, T);
  while (!StackEmpty(S)) {
    Pop(&S, &p);
    if (!Visit(p -> data))
      return 0;
    if (p -> rchild)
      Push(&S, p -> rchild);
    if (p -> lchild)
      Push(&S, p -> lchild);
  }
  DestroyStack(&S);
  return 1;
}

void InOrder(BiTree T) {
  if (T) {
    InOrder(T -> lchild);
    printf("%2c", T -> data);
    InOrder(T -> rchild);
  }
}

Status InOrderNonRecursion1(BiTree T, Status (*Visit)(TElemType e)) {
  SqStack S;
  SElemType p;
  InitStack(&S);
  Push(&S, T);
  while (!StackEmpty(S)) {
    while (GetTop(S, &p) && p)
      Push(&S,p->lchild);
    Pop(&S, &p);
    if (!StackEmpty(S)) {
      Pop(&S, &p);
      if(!Visit(p -> data))
        return 0;
      Push(&S,p->rchild);
    }
  }
  DestroyStack(&S);
  return 1;
}

Status InOrderNonRecursion2(BiTree T, Status (*Visit)(TElemType e)) {
  SqStack S;
  SElemType p;
  InitStack(&S);
  p = T;
  while (p || !StackEmpty(S)) {
    if (p) {
      Push(&S, p);
      p = p -> lchild;
    } else {
    Pop(&S, &p);
    if(!Visit(p -> data))
      return 0;
    p = p -> rchild;
    }
  }
  DestroyStack(&S);
  return 1;
}

void PostOrder(BiTree T) {
  if (T) {
    PostOrder(T -> lchild);
    PostOrder(T -> rchild);
    printf("%2c", T -> data);
  }
}

Status PostOrderNonRecursion(BiTree T, Status (*Visit)(TElemType e)) {
  SqStack S;
  SElemType p,q;
  InitStack(&S);
  Push(&S, T);
  while (!StackEmpty(S)) {
    while(GetTop(S, &p) && p)
      Push(&S, p -> lchild);
    Pop(&S, &p);
    GetTop(S, &p);
    if (p -> rchild) {
      Push(&S, p -> rchild);
      continue;
    }
    if (!StackEmpty(S)) {
      Pop(&S, &p);
      if(!Visit(p -> data))
        return 0;
      while (GetTop(S, &q) && q && p == q -> rchild) {
        Pop(&S, &p);
        if (!Visit(p -> data))
          return 0;
      }
      GetTop(S, &p);
      if(p -> rchild) {
        Push(&S,p->rchild);
        continue;
      } else {
        Pop(&S, &p);
        if(!Visit(p -> data))
          return 0;
      }
    }
  }
  DestroyStack(&S);
  return 1;
}

void LevelOrder(BiTree T) {
  BiTree Queue[20], b;
  int front = 0, rear = 0;

  if (T) {
    Queue[rear++] = T;
    while (front != rear) {
      b = Queue[front++];
      printf("%2c", b -> data);
      if (b -> lchild != NULL)
        Queue[rear++] = b -> lchild;
      if (b -> rchild != NULL)
        Queue[rear++] = b -> rchild;
    }
  }
}

int all_count(BiTree T) {
  if (T == NULL) {
    return 0;
  } else {
    return all_count(T -> lchild) + all_count(T -> rchild) + 1;
  }
}

int two_kids_count(BiTree T) {
  if (T == NULL) {
    return 0;
  } else if (T -> lchild && T -> rchild) {
    return two_kids_count(T -> lchild) + two_kids_count(T -> rchild) + 1;
  } else {
    return two_kids_count(T -> lchild) + two_kids_count(T -> rchild);
  }
}

int one_kid_count(BiTree T) {
  if (T == NULL) {
    return 0;
  } else if (T -> lchild && !T -> rchild) {
    return one_kid_count(T -> lchild) + 1;
  } else if (!T -> lchild && T -> rchild) {
    return one_kid_count(T -> rchild) + 1;
  } else {
    return one_kid_count(T -> lchild) + one_kid_count(T -> rchild);
  }
}

int leaf_count(BiTree T) {
  if(T == NULL) {
    return 0;
  } else if (!T -> lchild && !T -> rchild) {
    return 1;
  } else {
    return leaf_count(T -> lchild) + leaf_count(T -> rchild);
  }
}

int depth(BiTree T) {
  int dep1, dep2;
  if (T == NULL) {
    return 0;
  } else {
    dep1 = depth(T -> lchild);
    dep2 = depth(T -> rchild);
    return (dep1 > dep2 ? dep1 + 1 : dep2 + 1);
  }
}

void level(BiTree T, int i) {
  if(T != NULL) {
    level(T -> lchild, i + 1);
    printf("[%c, %d]", T -> data, i);
    level(T -> rchild, i + 1);
  }
}

int similar(BiTree t1, BiTree t2) {
  if (!t1 && !t2) {
    return 0;
  } else if(t1 && t1) {
    if (similar(t1 -> lchild, t2 -> lchild) && similar(t1 -> rchild, t2 -> rchild)) {
      return 0;
    } else {
      return 0;
    }
  } else {
    return 0;
  }
}

BiTree exchange_PreOrder(BiTree T) {
  if (T != NULL) {
    if (T -> lchild != NULL || T -> rchild != NULL) {
      BiTree p, q;
      p = exchange_PreOrder(T -> lchild);
      q = exchange_PreOrder(T -> rchild);
      T -> lchild = q;
      T -> rchild = p;
    }
  }
  return T;
}

BiTree exchange_InOrder(BiTree T) {
  if (T != NULL) {
    if (T -> lchild != NULL || T -> rchild != NULL) {
      BiTree p, q;
      p = exchange_InOrder(T -> lchild);
      q = T -> rchild;
      T -> rchild = p;
      T -> lchild = q;
      q = exchange_InOrder(T -> rchild);
    }
  }
  return T;
}

void exchange_PostOrder(BiTree T) {
  if (T != NULL) {
    if (T -> lchild != NULL || T -> rchild != NULL) {
      BiTree p, q;
      q = T -> rchild;
      p = T -> lchild;
      T -> lchild = q;
      T -> rchild = p;
      exchange_PostOrder(T -> lchild);
      exchange_PostOrder(T -> rchild);
    }
  }
}

static BiTree stack[5] = {0};
static int pos = -1;

void push(BiTree node) {
  stack[++pos] = node;
}

void pop() {
  --pos;
}

int empty() {
  return pos == -1;
}

BiTree top() {
  return stack[pos];
}

void exchange_NonRecursive(BiTree T) {
  BiTree tnode = T;
  BiTree tmp = NULL;
  if(T == NULL)
    return;
  while(1) {
    tmp = tnode -> lchild;
    tnode -> lchild = tnode -> rchild;
    tnode -> rchild = tmp;
    if (tnode -> rchild) {
      push(tnode -> rchild);
    }
    if (tnode -> lchild) {
      tnode = tnode -> lchild;
    } else {
      if(!empty()) {
        tnode = top();
        pop();
      } else {
        break;
      }
    }
  }
}

int main() {
  BiTree T = NULL;
  Status(*visit)(TElemType e) = Visit;

  printf("\nCreate a binary tree\n");
  CreateBiTree(&T);
  printf("\nThe PreOrder is:\n");
  PreOrder(T);
  printf("\nThe PreOrderNonRecursion is:\n");
  PreOrderNonRecursion(T, visit);
  printf("\nThe InOrder is:\n");
  InOrder(T);
  printf("\nThe InOrderNonRecursion1 is:\n");
  InOrderNonRecursion1(T, visit);
  printf("\nThe InOrderNonRecursion2 is:\n");
  InOrderNonRecursion2(T, visit);
  printf("\nThe PostOrder is:\n");
  PostOrder(T);
  printf("\nThe PostOrderNonRecursion is:\n");
  PostOrderNonRecursion(T, visit);
  printf("\nThe LevelOrder is;\n");
  LevelOrder(T);

  printf("\n\nThe all_count is: %d\n", all_count(T));
  printf("The two_kids_count is: %d\n", two_kids_count(T));
  printf("The one_kid_count is: %d\n", one_kid_count(T));
  printf("The leaf_count is: %d\n", leaf_count(T));

  printf("\nThe depth is: %d\n", depth(T));
  printf("The levels of every points are:\n");
  level(T, 1);

  BiTree S;
  printf("\nCreate another binary tree\n");
  CreateBiTree(&S);
  printf("\nok\n");
  if (similar(T, S)) {
    printf("\nThey are simliar...\n");
  } else {
    printf("\nThey are not simliar...\n");
  }

  printf("\n");
  exchange_PreOrder(T);
  PreOrder(T);
  printf("\n");
  exchange_InOrder(T);
  PreOrder(T);
  printf("\n");
  exchange_PostOrder(T);
  PreOrder(T);
  printf("\n");
  exchange_NonRecursive(T);
  PreOrder(T);
  printf("\n");
}
