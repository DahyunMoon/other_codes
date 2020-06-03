#include <stdio.h>
#include <float.h>

struct list {
  int vertex_index;
  double weight;
  struct list* next;
};

const int num_vertices = 7;
struct list Adj_array[num_vertices];
struct list all_lists[50]; // assume that we will have lists no more than 49
int num_all_lists_sf = 0; // number of all lists so far

void CreateArrayOfAdjList(double A[num_vertices][num_vertices]);
void PrintAdjList(int vertex_index);

int main(void) {

  // #define DBL_MAX 1.7976931348623158e+308 /* max value for a double variable defined in float.h*/ 
  double A[num_vertices][num_vertices] = {
    0,  25.0, DBL_MAX,  DBL_MAX,  DBL_MAX,  DBL_MAX,  DBL_MAX,
    DBL_MAX,  0,  10.0, 14.0, DBL_MAX,  DBL_MAX,  DBL_MAX,
    1,  DBL_MAX,  0,  DBL_MAX,  DBL_MAX,  16.0, DBL_MAX,
    DBL_MAX,  6.0,  18.0, 0,  DBL_MAX,  DBL_MAX,  DBL_MAX,
    DBL_MAX,  DBL_MAX,  DBL_MAX,  DBL_MAX,  0,  DBL_MAX,  DBL_MAX,
    DBL_MAX,  DBL_MAX,  DBL_MAX,  32.0, 42.0, 0,  14.0,
    DBL_MAX,  DBL_MAX,  DBL_MAX,  DBL_MAX,  DBL_MAX,  11.0, 0,
  };

  // create array of adjacency list
  CreateArrayOfAdjList(A);

  // print all adjacency lists
  for(int i=0; i<num_vertices; i++)
    PrintAdjList(i);

  return 0;
}

void CreateArrayOfAdjList(double A[num_vertices][num_vertices])
{
  for (int i=0; i<num_vertices; i++)
  {
    Adj_array[i].next = NULL;
    struct list* prev = &Adj_array[i];
    for (int j=0; j<num_vertices; j++)
    {
      if (i==j) continue;
      if (A[i][j] != DBL_MAX)
      {
        // write your code here
        struct list* curr = &all_lists[num_all_lists_sf];
        
        curr->vertex_index = j;
        curr->weight = A[i][j];
        curr->next = NULL;
        prev->next = curr;
        prev = &all_lists[num_all_lists_sf];
        num_all_lists_sf++;
      }
    }
  }
}


void PrintAdjList(int vertex_index)
{
  printf("%d → ", vertex_index);

  struct list* curr = &Adj_array[vertex_index];
  while (curr->next != NULL)
  {
    // write your code here
    curr = curr->next;
    printf("%d, %f → ", curr->vertex_index, curr->weight);
  }
  printf("nil\n");
  return;
}
