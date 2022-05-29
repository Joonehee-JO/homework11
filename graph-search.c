#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTEX 10

typedef struct graph {
	int key;
	struct graph* link;
} Graph;

/* functions that you have to implement */
int initializeGraph(Graph** h);	//그래프를 초기화하는 함수
/* you may add your own defined functions if necessary */

int main()
{
	char command;
	int key;
    int first, second;
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
			//freeBST(head);		//종료 전 메모리 반납
			break;
		case 'v': case 'V':
			printf("input vertex num = ");
			scanf("%d", &key);
			insertVertex(head, key);	//트리에 입력한 값을 갖는 노드 삽입 
			break;
        case 'e': case 'E':
            printf("input two vertex num = ");
            scanf("%d %d", &first, &second);
            insertEdge(head, first, second);
            break;
		case 'p': case 'P':
			printGraph(head);		//현재 스택에 들어있는 노드의 값 출력
			break;

		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

int initializeGraph(Graph** h){
    if(*h != NULL){
        //freeGraph(*h);
    }

    *h = (Graph*)malloc(sizeof(Graph) * MAX_VERTEX);
    for(int i=0; i<MAX_VERTEX; i++){
        (*h)[i].key = 0;
        (*h)[i].link = NULL;
    }

    return 1;
}

int insertVertex(Graph* h, int key){
    if(h == NULL){
        printf("the graph is not initialize\n");
        return 1;
    }

    if(key < 0 || key >= MAX_VERTEX){
        printf("you should input vertex num 0 ~ 9\n");
        return 1;
    }

    if(h[key].key != 0){
        printf("that vertex is exist in graph\n");
        return 1;
    }

    h[key].key = 1;

    printf("succes\n");
    return 1;
}

int insertEdge(Graph *h, int first, int second){
    //int count = 0;

    if(h == NULL){
        printf("the graph is not initialize\n");
        return 1;
    }

    if((first < 0 || first >= MAX_VERTEX) || (second < 0 || second >= MAX_VERTEX)){
        printf("you should input vertext num 0~9\n");
        return 1;
    }

    if(h[first].key == 0 || h[second].key == 0){
        printf("that vertex is not exist in graph\n");
        return 1;
    }

    Graph *current_graph = h[first].link;
    for(; current_graph; current_graph = current_graph->link){
        if(current_graph->key == second){
            printf("that vertices are already connected\n");
            return 1;
        }
    }

    //연결작업 첫번째 입력 vertex 삽입
    Graph* temp = (Graph*)malloc(sizeof(Graph));    //삽입할 vertex생성
    temp->key = second, temp->link = NULL;
    Graph* first_vertex = h[first].link;
    Graph* prev = NULL;
    for(; first_vertex; prev = first_vertex, first_vertex = first_vertex->link);
    if(!prev)h[first].link = temp; //해당 vertex의 인접노드가 없었다면 바로 연결시킬 노드를 가리키도록함
    else{
        prev -> link = temp;
    }

    //연결작업 두번째 입력 vertex 삽입
    Graph* temp2 = (Graph*)malloc(sizeof(Graph));    //삽입할 vertex생성
    temp2->key = first, temp2->link = NULL;
    Graph* second_vertex = h[second].link;
    prev = NULL;
    for(; second_vertex; prev = second_vertex, second_vertex = second_vertex->link);
    if(!prev)h[second].link = temp2; //해당 vertex의 인접노드가 없었다면 바로 연결시킬 노드를 가리키도록함
    else{
        prev -> link = temp2;
    }

    printf("succes2\n");
    return 1;
}

void printGraph(Graph *h){
    if(h == NULL){
        printf("the graph is not initialize\n");
        return 1;
    }

    for(int i=0; i<MAX_VERTEX; i++){
        if(h[i].key == 1){
            printf("%d -> ", i);
            Graph* current_graph = h[i].link;
            for(; current_graph; current_graph = current_graph->link){
                printf("%d -> ", current_graph->key);
            }
            printf("\n");
        }
    }
}