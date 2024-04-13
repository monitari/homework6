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

/* stack ������ �켱����, lparen = 0 ���� ���� */
typedef enum {
    lparen = 0, /* ( ���� ��ȣ */
    rparen = 9, /* ) ������ ��ȣ*/
    times = 7,  /* * ���� */
    divide = 6, /* / ������ */
    plus = 5,   /* + ���� */
    minus = 4,  /* - ���� */
    operand = 1 /* �ǿ����� */
} precedence;

char infixExp[MAX_EXPRESSION_SIZE];   /* infix expression�� �����ϴ� �迭 */
char postfixExp[MAX_EXPRESSION_SIZE]; /* postfix�� ����� ���ڿ��� �����ϴ� �迭 */
char postfixStack[MAX_STACK_SIZE];    /* postfix�� ��ȯ�� ���� �ʿ��� ���� */
int evalStack[MAX_STACK_SIZE];        /* ����� ���� �ʿ��� ���� */

int postfixStackTop = -1; /* postfixStack�� top */
int evalStackTop = -1;    /* evalStack�� top */

int evalResult = 0; /* ��� ����� ���� */

void postfixPush(char x); // postfixStack�� ���ڸ� push�ϴ� �Լ�
char postfixPop(); // postfixStack���� ���ڸ� pop�ϴ� �Լ�
void evalPush(int x); // evalStack�� ������ push�ϴ� �Լ�
int evalPop(); // evalStack���� ������ pop�ϴ� �Լ�
void getInfix(); // ���� ǥ����� �Է¹޴� �Լ�
precedence getToken(char symbol); // �־��� ������ �켱������ ��ȯ�ϴ� �Լ�
precedence getPriority(char x); // �־��� ������ ������ �켱������ ��ȯ�ϴ� �Լ�
void charCat(char *c); // postfixExp�� ���ڸ� �߰��ϴ� �Լ�
void toPostfix(); // ���� ǥ����� ���� ǥ������� ��ȯ�ϴ� �Լ�
void debug(); // ������� ���� �Լ�
void reset(); // �ʱ�ȭ�� ���� �Լ�
void evaluation(); // ���� ǥ����� ����ϴ� �Լ�

int main() {
    char command; // ��ɾ ������ ����

    do {
        printf("----------------------------------------------------------------\n");
        printf("               Infix to Postfix, then Evaluation               \n");
        printf("----------------------------------------------------------------\n");
        printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
        printf("----------------------------------------------------------------\n");

        printf("Command = ");
        scanf(" %c", &command); // ��ɾ� �Է�

        switch (command) {
            case 'i': 
            case 'I': // i�� I�� �Է¹����� ���� ǥ����� �Է¹���
                getInfix();
                break;
            case 'p':
            case 'P': // p�� P�� �Է¹����� ���� ǥ����� ���� ǥ������� ��ȯ
                toPostfix();
                break;
            case 'e':
            case 'E': // e�� E�� �Է¹����� ���� ǥ����� ���
                evaluation();
                break;
            case 'd':
            case 'D': // d�� D�� �Է¹����� �����
                debug();
                break;
            case 'r':
            case 'R': // r�� R�� �Է¹����� �ʱ�ȭ
                reset();
                break;
            case 'q':
            case 'Q': // q�� Q�� �Է¹����� ����
                break;
            default: // �� ���� ��ɾ �Է¹����� �й� �̸� ���
                printf("---------------------- 2023041018  ������ ----------------------\n");
                break;
        }
        printf("erase screen . . .");
        Sleep(2000); // 2�� ���
        system("cls");
    } while (command != 'q' && command != 'Q'); // q�� Q�� �Է¹����� �ݺ��� ����
    return 1;
}

void postfixPush(char x) { // postfixStack�� ���ڸ� push�ϴ� �Լ�
    postfixStack[++postfixStackTop] = x;
}

char postfixPop() { // postfixStack���� ���ڸ� pop�ϴ� �Լ�
    char x;
    if (postfixStackTop == -1) return '\0'; // ������ ��������� '\0' ��ȯ
    else x = postfixStack[postfixStackTop--];  // ������ ������� ������ pop
    return x;
}

void evalPush(int x) { evalStack[++evalStackTop] = x; } // evalStack�� ������ push�ϴ� �Լ�

int evalPop() { // evalStack���� ������ pop�ϴ� �Լ�
    if (evalStackTop == -1) return -1; // ������ ��������� -1 ��ȯ
    else return evalStack[evalStackTop--]; // ������ ������� ������ pop
}

void getInfix() { // ���� ǥ����� �Է¹޴� �Լ�
    printf("Type the expression >>> ");
    scanf("%s", infixExp); // ���� ǥ��� �Է�
}

precedence getToken(char symbol) { // �־��� ������ �켱������ ��ȯ�ϴ� �Լ�
    switch (symbol) {
    case '(': return lparen; // '('�̸� lparen ��ȯ
    case ')': return rparen; // ')'�̸� rparen ��ȯ
    case '+': return plus; // '+'�̸� plus ��ȯ
    case '-': return minus; // '-'�̸� minus ��ȯ
    case '/': return divide; // '/'�̸� divide ��ȯ
    case '*': return times; // '*'�̸� times ��ȯ
    default: return operand; // �� ���� ���ڴ� operand ��ȯ
    }
}

precedence getPriority(char x) { return getToken(x); } // �־��� ������ ������ �켱������ ��ȯ�ϴ� �Լ�

void charCat(char *c) { // postfixExp�� ���ڸ� �߰��ϴ� �Լ�
    if (postfixExp == '\0') strncpy(postfixExp, c, 1); // postfixExp�� ��������� ���ڸ� ����
    else strncat(postfixExp, c, 1); // postfixExp�� ������� ������ ���ڸ� �߰�
}

void toPostfix() { // ���� ǥ����� ���� ǥ������� ��ȯ�ϴ� �Լ�
    char *exp = infixExp; /* infixExp�� ���� �ϳ����� �б����� ������ */
    char x; /* �����ϳ��� �ӽ÷� �����ϱ� ���� ���� */

    while (*exp != '\0') { // ���ڰ� ���� ������ �ݺ�
        if (getToken(*exp) == operand) charCat(exp); // �ǿ������̸� �ٷ� postfixExp�� �߰�
        else if (getToken(*exp) == lparen) postfixPush(*exp); // ���� ��ȣ�̸� push
        else if (getToken(*exp) == rparen) { // ������ ��ȣ�̸� '('�� ���� ������ pop
            while (postfixStack[postfixStackTop] != '(') { // '('�� ���� ������ pop
                charCat(&postfixStack[postfixStackTop]); // pop�� ���ڸ� postfixExp�� �߰�
                postfixPop(); // pop
            }
            postfixPop(); // '(' ����
        } 
        else { // �������̸�
            while (postfixStackTop != -1 && getPriority(postfixStack[postfixStackTop]) >= getPriority(*exp)) {
            // ������ ������� �ʰ� ������ ������ �켱������ �� ���ų� ������
                charCat(&postfixStack[postfixStackTop]); // pop�� ���ڸ� postfixExp�� �߰�
                postfixPop(); // pop
            }
            postfixPush(*exp); // ������ push
        }
        exp++; // ���� ���ڷ� �̵�
    }
    while (postfixStackTop != -1) { // ���ÿ� ���� �����ڵ��� ��� pop
        charCat(&postfixStack[postfixStackTop]); // pop�� ���ڸ� postfixExp�� �߰�
        postfixPop(); // pop
    }
}

void debug() { // ������� ���� �Լ�
    printf("\nDEBUG\n");
    printf("   infixExp =  %s\n", infixExp);
    printf("   postExp =  %s\n", postfixExp);
    printf("   eval result = %d\n", evalResult);
    printf("   postfixStack : ");
    for (int i = 0; i < MAX_STACK_SIZE; i++) printf("%c  ", postfixStack[i]);
    printf("\n");
}

void reset() { // �ʱ�ȭ�� ���� �Լ�
    infixExp[0] = '\0';
    postfixExp[0] = '\0';

    for (int i = 0; i < MAX_STACK_SIZE; i++) postfixStack[i] = '\0';
    postfixStackTop = -1;
    evalStackTop = -1;
    evalResult = 0;
}

void evaluation() { // ���� ǥ����� ����ϴ� �Լ�
    /* postfixExp, evalStack�� �̿��� ��� */
    char *exp = postfixExp; // postfixExp�� ���� �ϳ����� �б����� ������
    int op1, op2; // ������ ���� ����
    while (*exp != '\0') { // ���ڰ� ���� ������ �ݺ�
        if (getToken(*exp) == operand) { // �ǿ������̸�
            evalPush(*exp - '0'); // ������ ��ȯ�Ͽ� push
        } 
        else { // �������̸�
            op2 = evalPop(); // �� ��° �ǿ����� pop
            op1 = evalPop(); // ù ��° �ǿ����� pop
            switch (getToken(*exp)) { // �����ڿ� ���� ���
                case plus: // ����
                    evalPush(op1 + op2);
                    break;
                case minus: // ����
                    evalPush(op1 - op2);
                    break;
                case times: // ����
                    evalPush(op1 * op2);
                    break;
                case divide: // ������
                    evalPush(op1 / op2);
                    break;
            }
        }
        exp++; // ���� ���ڷ� �̵�
    }
    evalResult = evalPop(); // ��� ��� ����
}
