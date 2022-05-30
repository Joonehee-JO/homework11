#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTEX 10

typedef struct graph {
	int key;
	struct graph* link;
} Graph;

Graph* adj_list[MAX_VERTEX];
int visit_flag[MAX_VERTEX];
int stackTop = -1;

Graph* Queue[MAX_VERTEX];
int front = -1;
int rear = -1;

/* functions that you have to implement */
int initializeGraph(Graph** h);	//그래프를 초기화하는 함수
int insertVertex(Graph*, int);  //그래프에 정점을 생성하는 함수
int insertEdge(Graph*, int , int);   //두 정점 사이 간선을 생성하는 함수
void printGraph(Graph*);        //그래프를 출력하는 함수
int freeGraph(Graph*);          //그래프를 구성하기 위해 사용한 메모리를 반납하는 함수
void Push(Graph*);              //깊이우선탐색을 위한 스택에 푸쉬하는 함수
Graph* Pop();             //스택에 탑에 있는 값을 팝하는 함수
void DFS(Graph*, int);          //깊이우선탐색 함수
void enQueue(Graph*);           //넓이우선탐색을 위한 큐에 푸쉬하는 함수
Graph* deQueue();               //큐에 데이터를 팝하는 함수
void BFS(Graph*, int);          //넓이우선탐색 함수
/* you may add your own defined functions if necessary */

int main()
{
	char command;
	int key;
    int first, second;  //엣지 삽입 함수 실행 시 정점 숫자를 입력 받을 변수
    int num;
	Graph* head = NULL;

	printf("[----- [Cho Joon Hee]  [2017038076] -----]\n");

	do{
		printf("\n\n");
		printf("----------------------------------------------------------------\n");
		printf("                        Graph Searches                          \n");
		printf("----------------------------------------------------------------\n");
		printf(" Initialize Graph     = z                                       \n");
		printf(" Insert Vertex        = v      Insert Edge                  = e \n");
		printf(" Depth First Search   = d      Breath First Search          = b \n");
		printf(" Print Graph          = p      Quit                         = q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");	//커맨드 입력
		scanf(" %c", &command);

		switch(command) {
		case 'z': case 'Z':
			initializeGraph(&head);	//그래프 초기화
			break;
		case 'q': case 'Q':
			freeGraph(head);		//종료 전 메모리 반납
			break;
		case 'v': case 'V':
			printf("input vertex num = ");
			scanf("%d", &key);
			insertVertex(head, key);	//그래프에 해당 숫자의 정점 생성 
			break;
        case 'e': case 'E':
            printf("input two vertex num = ");  
            scanf("%d %d", &first, &second);
            insertEdge(head, first, second);    //두 개의 정점을 연결하는 간선 생성
            break;
        case 'd': case 'D':
            printf("input first vertex num what you want = ");  
            scanf("%d", &num);
            DFS(head, num);         //깊이우선탐색함수 실행
            break;
        case 'b': case 'B':
            printf("input first vertex num what you want = ");  
            scanf("%d", &num);
            BFS(head, num);         //넓이우선탐색함수 실행
            break;
		case 'p': case 'P':
			printGraph(head);		//현재 그래프 출력
			break;

		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

int initializeGraph(Graph** h){     //그래프를 초기화하는 함수
    if(*h != NULL){     //초기화 전 메모리를 할당받았었다면
        freeGraph(*h);
    }

    *h = (Graph*)malloc(sizeof(Graph) * MAX_VERTEX);    //정점배열 생성 후 각 필드값 할당
    for(int i=0; i<MAX_VERTEX; i++){
        (*h)[i].key = 0;
        (*h)[i].link = NULL;
    }

    for(int i=0; i<MAX_VERTEX; i++){    //DFS, BFS를 위한 자료구조
        visit_flag[i] = 0;              //방문했는지 체크하는 플래그
        adj_list[i] = NULL;             //인접노드리스트를 관리하는 포인터 배열
    }

    return 1;
}

int insertVertex(Graph* h, int key){    //정점을 생성하는 함수
    if(h == NULL){      //헤더노드가 할당받은 메모리가 없다면
        printf("the graph is not initialize\n");
        return 1;
    }

    if(key < 0 || key >= MAX_VERTEX){       //생성할 정점의 숫자가 조건에 맞지않는 수라면
        printf("you should input vertex num 0 ~ 9\n");
        return 1;
    }

    if(h[key].key != 0){        //이미 해당 숫자의 정점이 존재한다면
        printf("that vertex is exist in graph\n");
        return 1;
    }

    h[key].key = 1;     //해당 인덱스의 key값을 1로 할당하여 새로 정점 생성

    //printf("succes\n");
    return 1;
}

int insertEdge(Graph *h, int first, int second){    //두 정점 사이 간선을 생성하는 함수
    //int count = 0;

    if(h == NULL){      //헤더노드가 할당받은 메모리가 없다면
        printf("the graph is not initialize\n");
        return 1;
    }

    if((first < 0 || first >= MAX_VERTEX) || (second < 0 || second >= MAX_VERTEX)){     //입력한 정점의 숫자가 조건에 맞지않는 수라면
        printf("you should input vertext num 0~9\n");
        return 1;
    }

    if(h[first].key == 0 || h[second].key == 0){        //입력한 정점이 그래프에 존재하지 않는다면
        printf("that vertex is not exist in graph\n");
        return 1;
    }

    Graph *current_graph = h[first].link;       //현재 탐색하는 정점을 가리킬 포인터, 첫번째로 입력한 정점의 인접노드 탐색
    for(; current_graph; current_graph = current_graph->link){  //반복문을 통해 인접노드를 탐색하면서 이미 입력한 두 정점 사이에 간선이 존재할 경우
        if(current_graph->key == second){
            printf("that vertices are already connected\n");
            return 1;
        }
    }

    //조건들을 만족할 경우, 연결작업 실행 / 첫번째 입력 vertex의 인접노드로 두번째 입력노드를 연결하여 간선생성
    Graph* temp = (Graph*)malloc(sizeof(Graph));    //삽입할 vertex생성
    temp->key = second, temp->link = NULL;          //key값을 통해 두번째로 입력한 정점임을 표현
    Graph* first_vertex = h[first].link;            //간선을 생성할 정점의 인접노드를 가리킬 포인터
    Graph* prev = NULL;                             //이전 인접노드를 가리킬 포인터
    for(; first_vertex; prev = first_vertex, first_vertex = first_vertex->link);    //인접노드가 존재할 경우 더이상 존재하지 않을 때까지 반복문을 돌음
    if(!prev)h[first].link = temp;  //해당 vertex의 인접노드가 없었다면 바로 연결시킬 노드를 가리키도록함
    else{                           //인접노드를 전부 탐색 후 마지막에 연결할 정점을 가리키도록함
        prev -> link = temp;
    }

    //마찬가지로 두번째 입력 vertex의 인접노드로 첫번째 입력노드를 연결하여 간선생성
    Graph* temp2 = (Graph*)malloc(sizeof(Graph));    
    temp2->key = first, temp2->link = NULL;
    Graph* second_vertex = h[second].link;
    prev = NULL;
    for(; second_vertex; prev = second_vertex, second_vertex = second_vertex->link);
    if(!prev)h[second].link = temp2; 
    else{
        prev -> link = temp2;
    }

    return 1;
}

void printGraph(Graph *h){      //그래프를 출력하는 함수
    if(h == NULL){      //헤더노드가 메모리를 할당받지 못했을 경우
        printf("the graph is not initialize\n");
        return;
    }

    for(int i=0; i<MAX_VERTEX; i++){    //정점리스트를 돌기위한 반복문
        if(h[i].key == 1){              //해당정점이 그래프에 존재할 경우 해당 정점과 인접노드들 출력
            printf("%d -> ", i);        
            Graph* current_graph = h[i].link;
            for(; current_graph; current_graph = current_graph->link){
                printf("%d -> ", current_graph->key);
            }
            printf("\n");
        }
    }
}

int freeGraph(Graph* h)      //그래프를 구성하기위해 사용한 메모리를 반납하는 함수
{
	if(h == NULL){      //헤더노드가 메모리를 할당받지 못했을 경우
        printf("the graph is not initialize\n");
        return 1;
    }

    for(int i=0; i<MAX_VERTEX; i++){    //반복문을 통해 정점리스트를 돌음
        if(h[i].key == 1){              //해당 정점이 그래프에 삽입된 이력이 있다면
            Graph* current_graph = h[i].link;   //현재 탐색할 노드를 가리킬 포인터(인접노드들 탐색)
            Graph* prev = NULL;                 //이전 노드를 가리킬 포인터(해당 포인터가 가리키는 정점 메모리반납)
            while(current_graph){               //반복문을 통해 인접노드들을 돌면서 해당 정점과 연결된 노드들 메모리반납
                prev = current_graph;
                current_graph = current_graph->link;
                free(prev);
            }
        }
    }
	free(h);         //마지막으로 헤드노드 메모리 해제

	return 1;
}

void Push(Graph* ptr){      //깊이 우선 탐색을 위한 스택에 푸쉬하는 함수   
	stackTop++;	    //푸쉬전 스택탑에 1을 더해줌
	adj_list[stackTop] = ptr;   //인접노드를 가리키는 포인터 배열에 푸쉬
}

Graph* Pop(){       //스택에 탑에 있는 값을 팝하는 함수
    if (stackTop == -1) {       //스택이 비어있을 경우
		printf("stack is empty\n");
		return NULL;
	}

	return adj_list[stackTop--]; //스택탑에 있는 값을 리턴(인접노드 주소)
}

void DFS(Graph* h, int num){    //깊이 우선 탐색 함수
    if(h == NULL){      //헤더노드가 메모리를 할당받지 못했을 경우
        printf("the graph is not initialized\n");
        return;
    }

    if(num < 0 || num >= MAX_VERTEX){   //정점의 숫자가 조건에 맞지않는 수라면
        printf("you should input vertext num 0~9\n");
        return;
    }

    if(h[num].key == 0){            //입력한 정점이 그래프에 존재하지 않을 경우
        printf("that vertex is not exist in graph\n");
        return;
    }

    if (h[num].link == NULL){       //탐색을 시작한 정점의 인접노드가 존재하지 않을 경우
        printf(" %d ", num);        //해당 정점만 출력 후 종료
        return;
    }

    Graph* vertex = NULL;   //정점을 가리킬 포인터
    visit_flag[num] = 1;    //현재 정점 방문플래그 1로 설정
    printf("%d -> ", num);    //탐색 시작 정점 출력
    Push(h[num].link);      //해당 정점의 인접노드 주소 푸쉬

    while(stackTop != -1){
        vertex = Pop();     //해당 인접노드팝함
        while(vertex){      //인접노드가 존재한다면
            if(visit_flag[vertex->key] == 0){   //연결된 정점이 방문한 전적이 없을시
                Push(vertex);                   //스택에 푸쉬 후 
                visit_flag[vertex->key] = 1;    //해당 정점 방문플래그 1로 설정
                printf("%d -> ", vertex->key);    //그리고 해당 정점 출력 후
                vertex = h[vertex->key].link;   //vertex가 해당 정점의 인접노드를 가리키도록 설정
            }
            else{           //탐색하는 정점이 방문한 전적이 있다면
                vertex = vertex->link;  //다음 인접노드로 이동
            }
        }
    }
    
    for(int i=0; i<MAX_VERTEX; i++){    //탐색 후 다음 DFS를 위하여 초기상태로 되돌림
        visit_flag[i] = 0;
        adj_list[i] = NULL;
    }
}

Graph* deQueue()   //큐에 데이터를 팝하는 함수
{
	if (front == rear) {   //현재 큐가 비어있다면
		//printf("current Queue is empty\n" );
		return NULL;
	}
	else{         //큐가 비어있지 않다면 
		front = (front + 1) % MAX_VERTEX;   //모듈러 연산을 통해 front값 변경(원형큐형태)
		return Queue[front];         //팝하여 원소를 빼냄
	}
}

void enQueue(Graph* aNode)   //큐에 값을 푸쉬하는 함수
{
	if((rear+1) % MAX_VERTEX == front){   //모듈러연산을 통해 큐가 꽉차있는지 체크
		printf("current Queue is Full\n");   //꽉차있다면 함수종료
		return;
	}
	else {         //큐가 비어있다면
		rear = (rear+1) % MAX_VERTEX;   //모듈러연산을 통하여 rear값 변경, 큐가 원형큐형태를 띔
		Queue[rear] = aNode;            //큐에 푸쉬
	}
}

void BFS(Graph* h, int num){    //너비 우선 탐색 함수
    if(h == NULL){      //헤더노드가 메모리를 할당받지 못했을 경우
        printf("the graph is not initialized\n");
        return;
    }

    if(num < 0 || num >= MAX_VERTEX){   //정점의 숫자가 조건에 맞지않는 수라면
        printf("you should input vertext num 0~9\n");
        return;
    }

    if(h[num].key == 0){            //입력한 정점이 그래프에 존재하지 않을 경우
        printf("that vertex is not exist in graph\n");
        return;
    }

    if (h[num].link == NULL){       //탐색을 시작한 정점의 인접노드가 존재하지 않을 경우
        printf(" %d ", num);        //해당 정점만 출력 후 종료
        return;
    }

    Graph* vertex = NULL;

    visit_flag[num] = 1;            //시작 정점 방문플래그 1로 설정
    printf("%d -> ", num);          //해당 정점 출력 후
    enQueue(h[num].link);           //큐에 푸쉬

    while(front != rear){           //큐가 빌때까지 반복문 실행
        vertex = deQueue();         //vertex가 큐에서 팝한 노드를 가리키도록함
        for (; vertex; vertex = vertex->link){  //그 후 인접노드 탐색하면서(동일 레벨 노드 출력위함)
            if (visit_flag[vertex->key] == 0){  //방문한 전적이 없다면
                printf("%d -> ", vertex->key);  //해당 정점 출력
                visit_flag[vertex->key] = 1;    //해당 정점 방문플래그를 설정 후
                enQueue(h[vertex->key].link);   //큐에 삽입
            }
        }
    }

    for(int i=0; i<MAX_VERTEX; i++){    //탐색 후 다음 BFS를 위하여 초기상태로 되돌림
        visit_flag[i] = 0;
        adj_list[i] = NULL;
    }
    rear = -1, front = -1;
}