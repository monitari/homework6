/**
 * circularQ.c
 *
 * School of Computer Science,
 * Chungbuk National University
**/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define MAX_QUEUE_SIZE 4 // ť ����� 4�� ����

typedef char element; // element Ÿ���� char�� ����
typedef struct { 
    element queue[MAX_QUEUE_SIZE]; // ���� ť�� ������ �迭
    int front, rear; // ���� ť�� front�� rear
} QueueType; // QueueType�� ����

QueueType *createQueue(); // ���� ť�� �����ϴ� �Լ�
int freeQueue(QueueType *cQ); // ť�� �����ϴ� �Լ�
int isEmpty(QueueType *cQ); // ���� ť�� ����ִ��� Ȯ���ϴ� �Լ�
int isFull(QueueType *cQ); // ���� ť�� ���� ���ִ��� Ȯ���ϴ� �Լ�
void enQueue(QueueType *cQ, element item); // ���� ť�� ���Ҹ� �߰��ϴ� �Լ�
void deQueue(QueueType *cQ, element *item); // ���� ť���� ���Ҹ� �����ϴ� �Լ�
void printQ(QueueType *cQ); // ���� ť�� ����ϴ� �Լ�
void debugQ(QueueType *cQ); // ���� ť�� ������ϴ� �Լ�
element getElement(); // ���Ҹ� �Է¹޴� �Լ�

int main(void) {
    QueueType *cQ = createQueue(); // ���� ť ����
    element data; // ���Ҹ� ������ ����
    char command; // ��ɾ ������ ����

    do {
        printf("------------------------------------------------------\n");
        printf("                     Circular Q                   \n");
        printf("------------------------------------------------------\n");
        printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
        printf("------------------------------------------------------\n");

        printf("Command = ");
        scanf(" %c", &command); // ��ɾ� �Է�

        switch (command) {
            case 'i':
            case 'I': // i�� I�� �Է¹����� ���Ҹ� ���� ť�� �߰�
                data = getElement();
                enQueue(cQ, data);
                break;
            case 'd':
            case 'D': // d�� D�� �Է¹����� ���Ҹ� ���� ť���� ����
                deQueue(cQ, &data);
                break;
            case 'p':
            case 'P': // p�� P�� �Է¹����� ���� ť�� ���
                printQ(cQ);
                break;
            case 'b':
            case 'B': // b�� B�� �Է¹����� ���� ť�� �����
                debugQ(cQ);
                break;
            case 'q':
            case 'Q': // q�� Q�� �Է¹����� ���� ť�� ����
                freeQueue(cQ);
                break;
            default: // �� ���� ��ɾ �Է¹����� �й� �̸� ���
                printf("----------------- 2023041018  ������ -----------------\n");
                break;
        }
        printf("erase screen . . .");
        Sleep(2000); // 2�� ���
        system("cls");
    } while (command != 'q' && command != 'Q'); // q�� Q�� �Է¹����� �ݺ��� ����
    return 1;
}

QueueType *createQueue() { // ���� ť�� �����ϴ� �Լ�
    QueueType *cQ;
    cQ = (QueueType *)calloc(1, sizeof(QueueType)); // ���� ť�� ���� �Ҵ� (calloc�� ����Ͽ� 0���� �ʱ�ȭ)
    return cQ;
}

int freeQueue(QueueType *cQ) { // ���� ť�� �����ϴ� �Լ�
    if (cQ == NULL) return 1;
    free(cQ);
    return 1;
}

int isEmpty(QueueType *cQ) { // ���� ť�� ����ִ��� Ȯ���ϴ� �Լ�
    return cQ->front == cQ->rear; // front�� rear�� ������ ť�� �������
}

int isFull(QueueType *cQ) { // ���� ť�� ���� ���ִ��� Ȯ���ϴ� �Լ�
    return cQ->front == (cQ->rear + 1) % MAX_QUEUE_SIZE; // front�� rear�� �� ĭ ���̳��� ť�� ���� ������
}

void enQueue(QueueType *cQ, element item) { // ���� ť�� ���Ҹ� �߰��ϴ� �Լ�
    if (isFull(cQ)) {
        printf("Queue is full!\n");
        return;
    }
    cQ->rear = (cQ->rear + 1) % MAX_QUEUE_SIZE; // rear�� �� ĭ �̵�
    cQ->queue[cQ->rear] = item; // rear�� ���� �߰�
}

void deQueue(QueueType *cQ, element *item) { // ���� ť���� ���Ҹ� �����ϴ� �Լ�
    if (isEmpty(cQ)) {
        printf("Queue is empty!\n");
        return;
    }
    cQ->front = (cQ->front + 1) % MAX_QUEUE_SIZE; // front�� �� ĭ �̵�
    *item = cQ->queue[cQ->front]; // front�� ���Ҹ� item�� ����
}

void printQ(QueueType *cQ) { // ���� ť�� ����ϴ� �Լ�
    int i, first, last;

    first = (cQ->front + 1) % MAX_QUEUE_SIZE;
    last = (cQ->rear + 1) % MAX_QUEUE_SIZE;

    printf("Circular Queue : [");
    i = first; // first���� ����
    while (i != last) { // last���� ���
        printf("%3c", cQ->queue[i]);
        i = (i + 1) % MAX_QUEUE_SIZE; // ���� ���ҷ� �̵�
    }
    printf(" ]\n");
}

void debugQ(QueueType *cQ) { // ���� ť�� ������ϴ� �Լ�
    printf("\nDEBUG\n");
    for (int i = 0; i < MAX_QUEUE_SIZE; i++) { // ť�� ��� ���� ���
        if (i == cQ->front) { // front�� ���
            printf("  [%d] = front\n", i);
            continue;
        }
        printf("  [%d] = %c", i, cQ->queue[i]);
        if (i == cQ->rear) printf(" (rear)\n"); // rear�� ���
        else printf("\n");
    }
    printf("front = %d, rear = %d\n", cQ->front, cQ->rear);
}

element getElement() { // ���Ҹ� �Է¹޴� �Լ�
    element item;
    printf("Input element = ");
    scanf(" %c", &item);
    return item;
}