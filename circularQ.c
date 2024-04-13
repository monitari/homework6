/**
 * circularQ.c
 *
 * School of Computer Science,
 * Chungbuk National University
**/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define MAX_QUEUE_SIZE 4 // 큐 사이즈를 4로 정의

typedef char element; // element 타입을 char로 정의
typedef struct { 
    element queue[MAX_QUEUE_SIZE]; // 원형 큐를 저장할 배열
    int front, rear; // 원형 큐의 front와 rear
} QueueType; // QueueType을 정의

QueueType *createQueue(); // 원형 큐를 생성하는 함수
int freeQueue(QueueType *cQ); // 큐를 해제하는 함수
int isEmpty(QueueType *cQ); // 원형 큐가 비어있는지 확인하는 함수
int isFull(QueueType *cQ); // 원형 큐가 가득 차있는지 확인하는 함수
void enQueue(QueueType *cQ, element item); // 원형 큐에 원소를 추가하는 함수
void deQueue(QueueType *cQ, element *item); // 원형 큐에서 원소를 제거하는 함수
void printQ(QueueType *cQ); // 원형 큐를 출력하는 함수
void debugQ(QueueType *cQ); // 원형 큐를 디버깅하는 함수
element getElement(); // 원소를 입력받는 함수

int main(void) {
    QueueType *cQ = createQueue(); // 원형 큐 생성
    element data; // 원소를 저장할 변수
    char command; // 명령어를 저장할 변수

    do {
        printf("------------------------------------------------------\n");
        printf("                     Circular Q                   \n");
        printf("------------------------------------------------------\n");
        printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
        printf("------------------------------------------------------\n");

        printf("Command = ");
        scanf(" %c", &command); // 명령어 입력

        switch (command) {
            case 'i':
            case 'I': // i나 I를 입력받으면 원소를 원형 큐에 추가
                data = getElement();
                enQueue(cQ, data);
                break;
            case 'd':
            case 'D': // d나 D를 입력받으면 원소를 원형 큐에서 제거
                deQueue(cQ, &data);
                break;
            case 'p':
            case 'P': // p나 P를 입력받으면 원형 큐를 출력
                printQ(cQ);
                break;
            case 'b':
            case 'B': // b나 B를 입력받으면 원형 큐를 디버깅
                debugQ(cQ);
                break;
            case 'q':
            case 'Q': // q나 Q를 입력받으면 원형 큐를 해제
                freeQueue(cQ);
                break;
            default: // 그 외의 명령어를 입력받으면 학번 이름 출력
                printf("----------------- 2023041018  김준후 -----------------\n");
                break;
        }
        printf("erase screen . . .");
        Sleep(2000); // 2초 대기
        system("cls");
    } while (command != 'q' && command != 'Q'); // q나 Q를 입력받으면 반복문 종료
    return 1;
}

QueueType *createQueue() { // 원형 큐를 생성하는 함수
    QueueType *cQ;
    cQ = (QueueType *)calloc(1, sizeof(QueueType)); // 원형 큐를 동적 할당 (calloc을 사용하여 0으로 초기화)
    return cQ;
}

int freeQueue(QueueType *cQ) { // 원형 큐를 해제하는 함수
    if (cQ == NULL) return 1;
    free(cQ);
    return 1;
}

int isEmpty(QueueType *cQ) { // 원형 큐가 비어있는지 확인하는 함수
    return cQ->front == cQ->rear; // front와 rear가 같으면 큐가 비어있음
}

int isFull(QueueType *cQ) { // 원형 큐가 가득 차있는지 확인하는 함수
    return cQ->front == (cQ->rear + 1) % MAX_QUEUE_SIZE; // front와 rear가 한 칸 차이나면 큐가 가득 차있음
}

void enQueue(QueueType *cQ, element item) { // 원형 큐에 원소를 추가하는 함수
    if (isFull(cQ)) {
        printf("Queue is full!\n");
        return;
    }
    cQ->rear = (cQ->rear + 1) % MAX_QUEUE_SIZE; // rear를 한 칸 이동
    cQ->queue[cQ->rear] = item; // rear에 원소 추가
}

void deQueue(QueueType *cQ, element *item) { // 원형 큐에서 원소를 제거하는 함수
    if (isEmpty(cQ)) {
        printf("Queue is empty!\n");
        return;
    }
    cQ->front = (cQ->front + 1) % MAX_QUEUE_SIZE; // front를 한 칸 이동
    *item = cQ->queue[cQ->front]; // front의 원소를 item에 저장
}

void printQ(QueueType *cQ) { // 원형 큐를 출력하는 함수
    int i, first, last;

    first = (cQ->front + 1) % MAX_QUEUE_SIZE;
    last = (cQ->rear + 1) % MAX_QUEUE_SIZE;

    printf("Circular Queue : [");
    i = first; // first부터 시작
    while (i != last) { // last까지 출력
        printf("%3c", cQ->queue[i]);
        i = (i + 1) % MAX_QUEUE_SIZE; // 다음 원소로 이동
    }
    printf(" ]\n");
}

void debugQ(QueueType *cQ) { // 원형 큐를 디버깅하는 함수
    printf("\nDEBUG\n");
    for (int i = 0; i < MAX_QUEUE_SIZE; i++) { // 큐의 모든 원소 출력
        if (i == cQ->front) { // front일 경우
            printf("  [%d] = front\n", i);
            continue;
        }
        printf("  [%d] = %c", i, cQ->queue[i]);
        if (i == cQ->rear) printf(" (rear)\n"); // rear일 경우
        else printf("\n");
    }
    printf("front = %d, rear = %d\n", cQ->front, cQ->rear);
}

element getElement() { // 원소를 입력받는 함수
    element item;
    printf("Input element = ");
    scanf(" %c", &item);
    return item;
}