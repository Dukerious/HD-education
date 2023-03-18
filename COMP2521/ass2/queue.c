#include "queue.h"
static Node CreateNode();
static Node do_QueueEnqueue(Queue Q, Node cur, 
Page p, bool (*fp) (Page, Page));
static void do_freeQueue(Node cur);
static Node find_nodeByname(Node cur, char *p);
static Node find_nodeByvertex(Node cur, int vertex);
static Node find_nodeBySearch(Node cur, int search);
static void do_StorePRInFile(FILE *f, Node cur);
static void do_MarkingIndex(Node cur, int index);
static Queue sorting_queue(Queue Q, bool (*fp) (Page, Page));
static void do_sorting_queue(Queue new, Node cur, bool (*fp) (Page, Page));
static void printPageName(Queue Q, int num);
static void do_print_PageName(Node cur, int num);
static void do_print_queue(Node cur);
static void do_merge_queue(Node cur, Queue Q, Queue merge);
static double do_RankFileDis(Node cur, int len, Queue P, int *PR);
static double FootruleDis(Queue P, Node cur, int size, int order);
static bool do_CheckWeight(Node head, int size, Queue P, 
int *PR, int cid, double *dis, double *sum);

// the struct of queue.
struct queue {
    int length;
    Node head;
    Node tail;
    char name[MAX];
    Queue next;
};

// node is a element in a queue.
struct node {
    struct node *next;
    Page p;
};

// set up empty queue.
Queue newQueue(char *Qname) {
   Queue Q = malloc(sizeof(struct queue));
   Q->length = 0;
   Q->head = NULL;
   Q->tail = NULL;
   if(Qname != NULL)
      strcpy(Q -> name, Qname);
   else strcpy(Q -> name, "");
   Q->next = NULL;
   return Q;
}


// remove unwanted queue
void freeQueue(Queue Q) {
   if (Q == NULL) {
      return;
   }
   freeQueue(Q -> next);
   do_freeQueue(Q -> head);
   free(Q);
}
// helper function to freeQueue.
static void do_freeQueue(Node cur) {
    if(cur == NULL) {
        return;
    }
   do_freeQueue(cur -> next);
   free(cur);
}


// make a new queue, and connect to the previous one.
Queue queueAdd(Queue prev, char *Qname) {
   Queue new = newQueue(Qname);
   if (prev != NULL) {
      prev -> next = new;
   }
   return new;
}


// set up empty node.
static Node CreateNode(Page p) {
   Node new = malloc(sizeof(struct node));
   new -> p = p;
   new -> next = NULL;
   assert(new != NULL);
   return new;
}

// create a page by page's data.
Page page_made(char *name, double PR, 
int outdegree, int searching, int vertex) {
   Page p;
   strcpy(p.name, name);
   p.PR = PR;
   p.outdegree = outdegree;
   p.vertex = vertex;
   p.searching = searching;
   return p;
}

// check whether queue is empty
bool QueueIsEmpty(Queue Q) {
   return (Q->length == 0);
}
// count num of page in a queue.
int num_page(Queue Q) {
   return Q -> length;
}


// insert page into queue in a specific order.
void QueueEnqueue(Queue Q, Page p, bool (*fp) (Page, Page)) {
   Q -> head = do_QueueEnqueue(Q, Q -> head, p, fp);   
   Q->length++;
}

static Node do_QueueEnqueue(Queue Q, Node cur, 
Page p, bool (*fp) (Page, Page)) {
   // base case.
   if(cur == NULL) {
      Node new = CreateNode(p);
      new -> next = cur;
      Q -> tail = new;
      return new;
   }
   // insert a page in a specific order.
   if(fp(cur -> p, p)) {
      Node new = CreateNode(p);
      new -> next = cur;
      return new;
   }
   cur -> next = do_QueueEnqueue(Q, cur -> next, p, fp);
   return cur;
}


// remove a page from front of queue
Page QueueDequeue(Queue Q) {
   assert(Q->length > 0);
   Node delete = Q->head;
   Q->head = Q->head->next;
   if (Q->head == NULL) {
      Q->tail = NULL;
   }
   Q->length--;
   Page p = delete -> p;
   free(delete);
   return p;
}


// return a sorted queue, which sort in a specific order.
static Queue sorting_queue(Queue Q, bool (*fp) (Page, Page)) {
   Queue new = newQueue(NULL);
   do_sorting_queue(new, Q -> head, fp);
   return new;
}
// helper function for sorting_queue.
static void do_sorting_queue(Queue new, Node cur, bool (*fp) (Page, Page)) {
   if(cur == NULL) {
      return;
   }
   QueueEnqueue(new, cur -> p, fp);
   do_sorting_queue(new, cur -> next, fp);
}


// some sorting condition functions:
bool SortingBySearch(Page a, Page b) {
   return (a.searching < b.searching);
}
bool SortingByVertex(Page a, Page b) {
   return (a.vertex > b.vertex);
}
bool SortingByPR(Page a, Page b) {
   return (a.PR < b.PR);
}
bool SortingByalpha(Page a, Page b) {
   return (strcmp(a.name, b.name) > 0); 
}
bool SortingBystable(Page a, Page b) {
   return(1 == 2);
}


// return a node, which found by name.
static Node find_nodeByname(Node cur, char *p) {
   if(cur == NULL) {
      return NULL;
   }
   Page page = cur -> p;
   if(strcmp(page.name, p) == 0) {
      return cur;
   }
   return find_nodeByname(cur -> next, p);
}

// return a node, which found by vertex.
static Node find_nodeByvertex(Node cur, int vertex) {
   if(cur == NULL) {
      return NULL;
   }
   Page p = cur -> p;
   if(p.vertex == vertex) {
      return cur;
   }
   return find_nodeByvertex(cur -> next, vertex);
}

// return a node, which found by searching.
static Node find_nodeBySearch(Node cur, int search) {
   if(cur == NULL) {
      return NULL;
   }
   Page p = cur -> p;
   if(p.searching == search) {
      return cur;
   }
   return find_nodeBySearch(cur -> next, search);
}

// find the page's vertex in graph.
int find_vertex(Queue Q, char *p) {
   assert(Q -> head != NULL);
   Node found = find_nodeByname(Q -> head, p);
   return found -> p.vertex;
}

// find the page's data in a queue by page name.
Page find_pageInQueue(Queue Q, char *p) {
   assert(Q != NULL);
   Node found = find_nodeByname(Q -> head, p);
   if(found == NULL) {
      Page NoFound = page_made("", 0, 0, 0, 0);
      return NoFound;
   }
   return found -> p;
}

// find the diff between the new and old PageRank.
// and store the new PR.
double diff(Queue Q, double new_PR, char *p) {
   assert(Q -> head != NULL);
   Node found = find_nodeByname(Q -> head, p);
   double dif = fabs(found -> p.PR - new_PR);
   found -> p.PR = new_PR;
   return dif;
}

// plus one for page's outdegree.
void outdegree_updating(Queue Q, int v) {
   Node found = find_nodeByvertex(Q -> head, v);
   (found -> p).outdegree ++;
}

// change the vertex for a page, find the page by searching.
void vertex_updating(Queue Q, int search, int vertex) {
   Node found = find_nodeBySearch(Q -> head, search);
   assert(found != NULL);
   (found -> p).vertex = vertex;
}

// find a page's outdegree by vertex.
int found_outdegree(Queue Q, int vertex) {
   Node found = find_nodeByvertex(Q -> head, vertex);
   return found -> p.outdegree;
}

// find a page's PageRank in queue.
double PR_finding(Queue Q, int vertex) {
   Node found = find_nodeByvertex(Q -> head, vertex);
   assert(found != NULL);
   return (found -> p).PR;
}

// store PageRank to a file in desending order.
// if PageRank is same, then ordered alphabetically.
void StorePRInFile(Queue Q) {
   Queue alpha = sorting_queue(Q, SortingByalpha);
   Queue sorted = sorting_queue(alpha, SortingByPR);
   FILE *f = fopen("pagerankList.txt", "wb");
   do_StorePRInFile(f, sorted -> head);
   freeQueue(alpha);
   freeQueue(sorted);
}
// helper function for StorePRInFile.
static void do_StorePRInFile(FILE *f, Node cur) {
   if (cur == NULL) {
      return;
   }
   Page p = cur -> p;
   fprintf(f, "%s, %d, %.7lf\n", p.name, p.outdegree, p.PR);
   do_StorePRInFile(f, cur -> next);
}

// part2, sorting our queue by alpha, PR and search,
// step by step.
void SortingSearchQueue(Queue Q) {
   if(Q -> head == NULL) return;
   Queue alpha = sorting_queue(Q, SortingByalpha);
   Queue PR = sorting_queue(alpha, SortingByPR);
   Queue matching = sorting_queue(PR, SortingBySearch);
   // print top 30.
   printPageName(matching, 30);
   freeQueue(alpha);
   freeQueue(PR);
   freeQueue(matching);
 }

// find a queue by search item.
Queue find_queue(Queue Q, char *item) {
    if(Q == NULL) {
        return NULL;
    }
    if(strcmp(Q -> name, item) == 0) {
        return Q;
    }
    return find_queue(Q -> next, item);
}

// Combine all the queues in the queue list into one queue.
// and update the number of matching search terms.
void mergeListQueue(Queue Q, Queue merge) {
   if(Q == NULL) {
      return;
   }
   merge_queue(Q, merge);
   mergeListQueue(Q -> next, merge);
}
// merge two queues, avoid duplicate page.
void merge_queue(Queue Q, Queue merge) {
    do_merge_queue(Q -> head, Q, merge);
}
// helper function for merge_queue.
static void do_merge_queue(Node cur, Queue Q, Queue merge) {
   if(cur == NULL) {
      return;
   }
   Page p = find_pageInQueue(Q, cur -> p.name);
   Page check = find_pageInQueue(merge, cur -> p.name);

   // avoid duplicate items.
   // if page doesn't exist in merge queue, then add.
   if(strcmp(check.name, "") == 0) {
      QueueEnqueue(merge, p, SortingBystable);
   }
   // add searchRank for a page.
   AddSearchRank(merge, cur -> p.name);
   do_merge_queue(cur -> next, Q, merge);
}

// adding number of matching search terms for a page.
void AddSearchRank(Queue Q, char *name) {
   Node found = find_nodeByname(Q -> head, name);
   assert(found != NULL);
   found -> p.searching++;
}

// print a certain number of pages in a queue.
static void printPageName(Queue Q, int num) {
   do_print_PageName(Q -> head, num);
}
static void do_print_PageName(Node cur, int num) {
   int count = 0;
   while (cur != NULL && count < num) {
      printf("%s\n", cur -> p.name);
      cur = cur -> next;
      count++;
   }
}

////////////////////////////
// part 3:
// calculate scaled-footrule distance for all pages. 
double RankFileDis(Queue Q, Queue P, int *PR) {
   // base case
   if(Q == NULL) {
      return 0;
   }
   int size = Q -> length;
   // distance for cur queue.
   double W = do_RankFileDis(Q -> head, size, P, PR);
   // add each distance for each queue.
   return W + RankFileDis(Q -> next, P, PR);
}
static double do_RankFileDis(Node cur, int size, Queue P, 
int *PR) {
   // base case
   double W = 0;
   if(cur == NULL) {
      return W;
   }
   // Find the vertex corresponding to the page.
   int index = find_vertex(P, cur -> p.name); 
   // find the position for a vertex.
   int p = PR[index];
   // find the distance for a page.
   W = FootruleDis(P, cur, size, p);

   // add each weight for each page.
   return W + do_RankFileDis(cur -> next, size, P, PR);
}

// check the weight for some pages until cid index.
bool CheckWeight(Queue Q, Queue P, int *PR, 
double *dis, int cid) {
   double sum = 0;
   Queue cur = Q;
   while (cur != NULL) {
      bool check = 
do_CheckWeight(cur -> head, cur -> length, P, PR, cid, dis, &sum);
      // if calculate until now is greater than min_dis,
      // stop calculate and return false signal.
      // in this way, we can avoid to calculate useless weight.
      if(!check) {
         return false;
      }
      cur = cur -> next;
   }
   return true;
}
static bool do_CheckWeight(Node head, int size, Queue P, 
int *PR, int cid, double *dis, double *sum) {
   Node cur = head;
   while (cur != NULL) {
      // Find the vertex corresponding to the page.
      int index = find_vertex(P, cur -> p.name); 

      // we just calculate the position's index less the cid.
      if(index <= cid) {
         // find the position for a vertex.
         int p = PR[index];
         *sum += FootruleDis(P, cur, size, p);
         // if calculate until now is greater than min_dis,
         // stop calculate and return false signal.
         // in this way, we can avoid to calculate useless weight.
         if(*sum > *dis) {
            return false;
         }
      }
      cur = cur -> next;
   }
   return true;
}

// calculate the weight for a page.
static double FootruleDis(Queue P, Node cur, int size, int order) {
   int c = cur -> p.vertex;
   double Wp = (double) order / (P -> length);
   double Wc = (double) c / size;
   double W = fabs(Wp - Wc);
   return W;
}

// part3, print a queue follow the order in array.
void PrintByArrayQueue(int *a, int len, Queue Q) {
   // unpdate the new position for the pages in queue.
   for(int i = 0; i < len; i++) {
      vertex_updating(Q, i, a[i] - 1);
   }
   // update the queue, by the position rank.
   Queue result = sorting_queue(Q, SortingByVertex);
   printPageName(result, result -> length);
   freeQueue(result);
}

// initialize all pages index in a queue.
// and copy index store in searching.
void Markingindex(Queue Q) {
   do_MarkingIndex(Q -> head, 0);
   
}
static void do_MarkingIndex(Node cur, int index) {
   if(cur == NULL) {
      return;
   }
   cur -> p.vertex = index;
   cur -> p.searching = index;
   do_MarkingIndex(cur -> next, index + 1);
}


///////////////////////////////////////////////
// some debugging functions:

// print a queue's name
void print_queue_name(Queue Q) {
   printf("%s\n", Q -> name);
}

// print all page's name by page's vertex.
void print_Byvertex(Queue Q, int vertex) {
   Node found = find_nodeByvertex(Q -> head, vertex);
   printf("%d, %s", found -> p.vertex, found -> p.name);
}

// print all data store in all queue.
void print_queue(Queue Q) {
   if (Q == NULL) {
      return;
   }
   if(Q -> name != NULL)
      printf("%s:\n", Q -> name);
   do_print_queue(Q -> head);
   printf("\n \n");
   print_queue(Q -> next);
}
static void do_print_queue(Node cur) {
   if (cur == NULL) {
      return;
   }
   Page p = cur -> p;
   printf("%d: %s\n", p.vertex, p.name);
   do_print_queue(cur -> next);
}