/**
 * postfix.c
 *
 * School of Computer Science,
 * Chungbuk National University
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX_STACK_SIZE 10
#define MAX_EXPRESSION_SIZE 20

/* stack 내에서 우선순위, lparen = 0 가장 낮음 */
typedef enum {
    lparen = 0, /* ( 왼쪽 괄호 */
    rparen = 9, /* ) 오른쪽 괄호*/
    times = 7,  /* * 곱셈 */
    divide = 6, /* / 나눗셈 */
    plus = 5,   /* + 덧셈 */
    minus = 4,  /* - 뺄셈 */
    operand = 1 /* 피연산자 */
} precedence;

char infixExp[MAX_EXPRESSION_SIZE];   /* infix expression을 저장하는 배열 */
char postfixExp[MAX_EXPRESSION_SIZE]; /* postfix로 변경된 문자열을 저장하는 배열 */
char postfixStack[MAX_STACK_SIZE];    /* postfix로 변환을 위해 필요한 스택 */
int evalStack[MAX_STACK_SIZE];        /* 계산을 위해 필요한 스택 */

int postfixStackTop = -1; /* postfixStack용 top */
int evalStackTop = -1;    /* evalStack용 top */

int evalResult = 0; /* 계산 결과를 저장 */

void postfixPush(char x); // postfixStack에 문자를 push하는 함수
char postfixPop(); // postfixStack에서 문자를 pop하는 함수
void evalPush(int x); // evalStack에 정수를 push하는 함수
int evalPop(); // evalStack에서 정수를 pop하는 함수
void getInfix(); // 중위 표기식을 입력받는 함수
precedence getToken(char symbol); // 주어진 문자의 우선순위를 반환하는 함수
precedence getPriority(char x); // 주어진 문자의 연산자 우선순위를 반환하는 함수
void charCat(char *c); // postfixExp에 문자를 추가하는 함수
void toPostfix(); // 중위 표기식을 후위 표기식으로 변환하는 함수
void debug(); // 디버깅을 위한 함수
void reset(); // 초기화를 위한 함수
void evaluation(); // 후위 표기식을 계산하는 함수

int main() {
    char command; // 명령어를 저장할 변수

    do {
        printf("----------------------------------------------------------------\n");
        printf("               Infix to Postfix, then Evaluation               \n");
        printf("----------------------------------------------------------------\n");
        printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
        printf("----------------------------------------------------------------\n");

        printf("Command = ");
        scanf(" %c", &command); // 명령어 입력

        switch (command) {
            case 'i': 
            case 'I': // i나 I를 입력받으면 중위 표기식을 입력받음
                getInfix();
                break;
            case 'p':
            case 'P': // p나 P를 입력받으면 중위 표기식을 후위 표기식으로 변환
                toPostfix();
                break;
            case 'e':
            case 'E': // e나 E를 입력받으면 후위 표기식을 계산
                evaluation();
                break;
            case 'd':
            case 'D': // d나 D를 입력받으면 디버깅
                debug();
                break;
            case 'r':
            case 'R': // r나 R을 입력받으면 초기화
                reset();
                break;
            case 'q':
            case 'Q': // q나 Q를 입력받으면 종료
                break;
            default: // 그 외의 명령어를 입력받으면 학번 이름 출력
                printf("---------------------- 2023041018  김준후 ----------------------\n");
                break;
        }
        printf("erase screen . . .");
        Sleep(2000); // 2초 대기
        system("cls");
    } while (command != 'q' && command != 'Q'); // q나 Q를 입력받으면 반복문 종료
    return 1;
}

void postfixPush(char x) { // postfixStack에 문자를 push하는 함수
    postfixStack[++postfixStackTop] = x;
}

char postfixPop() { // postfixStack에서 문자를 pop하는 함수
    char x;
    if (postfixStackTop == -1) return '\0'; // 스택이 비어있으면 '\0' 반환
    else x = postfixStack[postfixStackTop--];  // 스택이 비어있지 않으면 pop
    return x;
}

void evalPush(int x) { evalStack[++evalStackTop] = x; } // evalStack에 정수를 push하는 함수

int evalPop() { // evalStack에서 정수를 pop하는 함수
    if (evalStackTop == -1) return -1; // 스택이 비어있으면 -1 반환
    else return evalStack[evalStackTop--]; // 스택이 비어있지 않으면 pop
}

void getInfix() { // 중위 표기식을 입력받는 함수
    printf("Type the expression >>> ");
    scanf("%s", infixExp); // 중위 표기식 입력
}

precedence getToken(char symbol) { // 주어진 문자의 우선순위를 반환하는 함수
    switch (symbol) {
    case '(': return lparen; // '('이면 lparen 반환
    case ')': return rparen; // ')'이면 rparen 반환
    case '+': return plus; // '+'이면 plus 반환
    case '-': return minus; // '-'이면 minus 반환
    case '/': return divide; // '/'이면 divide 반환
    case '*': return times; // '*'이면 times 반환
    default: return operand; // 그 외의 문자는 operand 반환
    }
}

precedence getPriority(char x) { return getToken(x); } // 주어진 문자의 연산자 우선순위를 반환하는 함수

void charCat(char *c) { // postfixExp에 문자를 추가하는 함수
    if (postfixExp == '\0') strncpy(postfixExp, c, 1); // postfixExp가 비어있으면 문자를 복사
    else strncat(postfixExp, c, 1); // postfixExp가 비어있지 않으면 문자를 추가
}

void toPostfix() { // 중위 표기식을 후위 표기식으로 변환하는 함수
    char *exp = infixExp; /* infixExp의 문자 하나씩을 읽기위한 포인터 */
    char x; /* 문자하나를 임시로 저장하기 위한 변수 */

    while (*exp != '\0') { // 문자가 끝날 때까지 반복
        if (getToken(*exp) == operand) charCat(exp); // 피연산자이면 바로 postfixExp에 추가
        else if (getToken(*exp) == lparen) postfixPush(*exp); // 왼쪽 괄호이면 push
        else if (getToken(*exp) == rparen) { // 오른쪽 괄호이면 '('가 나올 때까지 pop
            while (postfixStack[postfixStackTop] != '(') { // '('가 나올 때까지 pop
                charCat(&postfixStack[postfixStackTop]); // pop한 문자를 postfixExp에 추가
                postfixPop(); // pop
            }
            postfixPop(); // '(' 제거
        } 
        else { // 연산자이면
            while (postfixStackTop != -1 && getPriority(postfixStack[postfixStackTop]) >= getPriority(*exp)) {
            // 스택이 비어있지 않고 스택의 연산자 우선순위가 더 높거나 같으면
                charCat(&postfixStack[postfixStackTop]); // pop한 문자를 postfixExp에 추가
                postfixPop(); // pop
            }
            postfixPush(*exp); // 연산자 push
        }
        exp++; // 다음 문자로 이동
    }
    while (postfixStackTop != -1) { // 스택에 남은 연산자들을 모두 pop
        charCat(&postfixStack[postfixStackTop]); // pop한 문자를 postfixExp에 추가
        postfixPop(); // pop
    }
}

void debug() { // 디버깅을 위한 함수
    printf("\nDEBUG\n");
    printf("   infixExp =  %s\n", infixExp);
    printf("   postExp =  %s\n", postfixExp);
    printf("   eval result = %d\n", evalResult);
    printf("   postfixStack : ");
    for (int i = 0; i < MAX_STACK_SIZE; i++) printf("%c  ", postfixStack[i]);
    printf("\n");
}

void reset() { // 초기화를 위한 함수
    infixExp[0] = '\0';
    postfixExp[0] = '\0';

    for (int i = 0; i < MAX_STACK_SIZE; i++) postfixStack[i] = '\0';
    postfixStackTop = -1;
    evalStackTop = -1;
    evalResult = 0;
}

void evaluation() { // 후위 표기식을 계산하는 함수
    /* postfixExp, evalStack을 이용한 계산 */
    char *exp = postfixExp; // postfixExp의 문자 하나씩을 읽기위한 포인터
    int op1, op2; // 연산을 위한 변수
    while (*exp != '\0') { // 문자가 끝날 때까지 반복
        if (getToken(*exp) == operand) { // 피연산자이면
            evalPush(*exp - '0'); // 정수로 변환하여 push
        } 
        else { // 연산자이면
            op2 = evalPop(); // 두 번째 피연산자 pop
            op1 = evalPop(); // 첫 번째 피연산자 pop
            switch (getToken(*exp)) { // 연산자에 따라 계산
                case plus: // 덧셈
                    evalPush(op1 + op2);
                    break;
                case minus: // 뺄셈
                    evalPush(op1 - op2);
                    break;
                case times: // 곱셈
                    evalPush(op1 * op2);
                    break;
                case divide: // 나눗셈
                    evalPush(op1 / op2);
                    break;
            }
        }
        exp++; // 다음 문자로 이동
    }
    evalResult = evalPop(); // 계산 결과 저장
}
