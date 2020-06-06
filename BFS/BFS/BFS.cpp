//-------------- Created by TamHuynh June 6th-----------
/*
	BFS algorithm using text file input
	Run once
	There's still bug if you put BFS into a loop
*/

#include<stdio.h>
#include<conio.h>
#include<stdlib.h>

#define MAX 100
#define input "BFSInput.txt"

//--------------------------Data structure Queue----------------
typedef struct Node
{
	int data;
	Node* next;
}node;

typedef struct Queue_of_Nodes
{
	int capacity;
	int length;
	node* front;
	node* rear;
}Queue;

node* newNode()
{
	node* temp = new node();
	temp->data = 0;
	temp->next = NULL;
	return temp;
}

Queue* initiateQueue()
{
	Queue* q = new Queue();
	q->capacity = MAX;
	q->front = NULL;
	q->rear = NULL;
	q->length = -1;
	return q;
}

bool is_Empty(Queue* queue)
{
	return queue->length == -1;
}

bool is_Full(Queue* queue)
{
	return queue->length == queue->capacity - 1;
}

void Enqueue(Queue* queue, int x)
{
	if (is_Full(queue))
	{
		printf("Overflow condition! Cannot add %d into queue", x);
		return;
	}
	node* temp = newNode();
	if (is_Empty(queue))
	{
		temp->data = x;
		queue->front = queue->rear = temp;
		queue->length++;
	}
	else if (queue->length == 0)
	{
		temp->data = x;
		queue->rear = temp;
		queue->front->next = temp;
		queue->length++;
	}
	else
	{
		temp->data = x;
		queue->rear->next = temp;
		queue->rear = temp;
		queue->length++;
	}
}

int Dequeue(Queue* queue)
{
	int result = queue->front->data;
	if (is_Empty(queue))
	{
		printf("Underflow condition! Queue is empty.\n");
		return false;
	}
	node* temp = queue->front;
	if (queue->length == 0)
	{
		queue->length--;
		queue->front = queue->rear = NULL;
		delete temp;
	}
	else
	{
		queue->length--;
		queue->front = queue->front->next;
		delete temp;
	}
	return result;
}

void Display(Queue* queue)
{
	if (queue->length == -1)
	{
		printf("Queue is empty.\n");
		return;
	}
	node* p = queue->front;
	printf("Elements in queue: ");
	while (p != NULL)
	{
		printf("%d ", p->data);
		p = p->next;
	}
	//printf("\nFront value is: %d\nRear value is: %d\n", Front(queue), queue->rear->data);
	//printf("Size of queue: %d\n", Size(queue));
}
//------------------------------------------------------
int Graph[MAX][MAX];
int size;
int start;
int end;
Queue * Waiting;// store next nodes to visit here
int Parent[MAX];// store the parent of each node, staring node's parent is 0
int Visited[MAX];// store visited nodes, besides can be used as a path
int visitedNum = 0;

void file_Input()
{
	FILE* fi = fopen(input, "rt");
	if (fi == NULL)
	{
		printf("No such file directory!!\n");
		_sleep(1000);
		exit(0);
	}
	fscanf(fi, "%d", &size);
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			fscanf(fi, "%d", &Graph[i][j]);
	fclose(fi);
}

bool check_input_Error(int check)
{
	return check < 1 || check > size;
}

void node_Input(int &in)
{
	scanf("%d", &in);
	while (check_input_Error(in))
	{
		printf("Enter again: ");
		scanf("%d", &in);
	}
}

bool exist_in_Waiting(int value)
{
	node* temp = Waiting->front;
	while (temp != NULL)
	{
		if (temp->data == value)
			return true;
		temp = temp->next;
	}
	return false;
}

bool exist_in_Visited(int value)
{
	for (int i = 0; i < size; i++)
		if (Visited[i] == value)
			return true;
	return false;
}

void find_Adjacencies(int from)
{
	for (int j = 0; j < size; j++)
		if (Graph[from][j] == 1 && (exist_in_Waiting(j + 1) == false && exist_in_Visited(j + 1) == false))
		{
			Enqueue(Waiting, j + 1);
			Parent[j] = from + 1;
		}
}

void initiate()
{
	for (int i = 0; i < size; i++)
	{
		Parent[i] = -1;
		Visited[i] = 0;
	}
	Waiting = initiateQueue();

	Parent[start - 1] = 0;
	Visited[visitedNum++] = start;
	find_Adjacencies(start - 1);
}

int edges_Count()// Count number of edges
{
	int count = 0;
	for (int i = 0; i < size - 1; i++)
	{
		int j = i + 1;
		while (j < size)
		{
			if (Graph[i][j] == 1)
				count++;
			j++;
		}
	}
	return count;
}

void BFS()
{
	printf("Enter starting point: ");
	node_Input(start);// Input starting node
	initiate();
	int loop = size - 1;// 
	while (loop > 0)
	{
		int visiting = Dequeue(Waiting);
		find_Adjacencies(visiting - 1);
		Visited[visitedNum++] = visiting;
		loop--;
	}
}

void search_Path()
{
	printf("Enter destination: ");
	node_Input(end);
	printf("Path from %d to %d: ", start, end);
	while (end != start)
	{
		printf("%d <- ", end);
		end = Parent[end - 1];
	}
	printf("%d\n", start);
}

void traverse_Path()
{
	printf("Traversing order: ");
	for (int i = 0; i < size - 1; i++)
		printf("%d -> ", Visited[i]);
	printf("%d", Visited[size - 1]);
	printf("\n");
	printf("Parent: ");
	for (int i = 0; i < size; i++)
		printf("%d ", Parent[i]);
	printf("\n");
	printf("Number of edges: %d\n", edges_Count());
}

void main()
{
	file_Input();
	BFS();
	search_Path();
	traverse_Path();
	_getch();
}