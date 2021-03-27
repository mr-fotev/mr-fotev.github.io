/*
 [26-Feb-21 12:29]
 
 Напишете подпрограма, която подрежда зададени като нейни аргументи точки A, B, C и D така, че ABCD да бъде положително ориентиран четириъгълник –
 предполага се, че такъв съществува. Направете това с възможно най-малък брой размени на точки и по възможност кратко.
 Точките са зададени чрез координатите им, цели числа.
*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

typedef struct Point{
    int x;
    int y;
} P2;

void initP2(P2 *P, int x, int y){
    P->x = x; P->y = y;
}

void swap(P2 * p, P2 * q){
    P2 t = *p; *p = *q; *q = t;
}

// Banchev

int pos(P2 *p, P2 *q, P2 *r){
    return (q->x-p->x)*(r->y-p->y) > (r->x-p->x)*(q->y-p->y);
}

void makeq(P2 *a, P2 *b, P2 *c, P2 *d){
    int s1 = pos(a,b,c), s2 = pos(b,c,d);
    int p = s1 + s2 + pos(c,d,a) + pos(d,a,b);
    if (p < 2) swap(a,c); else if (p == 2) swap(s1?d:b,s2?a:c);
}

// Fotev

int c(P2 *A, P2 *B){
    return A->x*B->y - A->y*B->x;
}

int n(P2 *A, P2 *B, P2 *C){
    return c(A,B)+c(B,C)+c(C,A) > 0;
}

void p(P2 *A, P2 *B, P2 *C, P2 *D){
    if (n(A,B,C)){
        if (!n(A,C,D)){
            if (n(A,B,D)){
                if (!n(B,C,D)) swap(C,D);
            }
            else swap(A,D);
        }
    }
    else{
        if (n(A,C,D)){
            if (n(A,B,D)){
                if (!n(B,C,D)) swap(B,C);
            }
            else swap(A,B);
        }
        else swap(A,C);
    }
}

int aSeq[] = {100000, 200000, 500000, 1000000, 2000000, 5000000, 10000000, 20000000, 50000000, 100000000, 200000000};
int iSeq = sizeof(aSeq) / sizeof(int);

double aTimes[2][100];

int main()
{
    P2 *A = malloc(sizeof(P2));
    P2 *B = malloc(sizeof(P2));
    P2 *C = malloc(sizeof(P2));
    P2 *D = malloc(sizeof(P2));

    int iCurrSeq, i, j;

    // Test Banchev

    srand(2021);

    for (i = 0; i < iSeq; i++){
        iCurrSeq = aSeq[i];

        clock_t start = clock();

        for (j = 0; j < iCurrSeq; j++){
            initP2(A, rand(), rand());
            initP2(B, rand(), rand());
            initP2(C, rand(), rand());
            initP2(D, rand(), rand());

            makeq(A, B, C, D);
        }

        clock_t end = clock();

        aTimes[0][i] = (double) (end - start) / CLOCKS_PER_SEC;
    }

    // Test Fotev

    srand(2021);

    for (i = 0; i < iSeq; i++){
        iCurrSeq = aSeq[i];

        clock_t start = clock();

        for (j = 0; j < iCurrSeq; j++){
            initP2(A, rand(), rand());
            initP2(B, rand(), rand());
            initP2(C, rand(), rand());
            initP2(D, rand(), rand());

            p(A, B, C, D);
        }

        clock_t end = clock();

        aTimes[1][i] = (double) (end - start) / CLOCKS_PER_SEC;
    }

    printf("Attempts\tTime Banchev (sec)\tTime Fotev (sec)\n");

    for (i = 0; i < iSeq; i++){
        printf("%d\t%f\t%f\n", aSeq[i], aTimes[0][i], aTimes[1][i]);
    }

    return 0;
}
