#include <conio.h>
#include <math.h>
#include <stdio.h>
#include <windows.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <dos.h>

const int maxThreadsCount = 10000;
const int minimumThreadsCount = 1;
const int maximumRadiusEvent = 5;
const float supInf = (float)123456789.0; // possible make it to first element in matrix
const float infInf = (float)-1234567899.0; // possible make it to first element in matrix
pthread_t *threads; // depends on maxThreadsCount
pthread_mutex_t mutex;
pthread_mutex_t func;
pthread_mutex_t help;
int pointer = 0;
int threadCurrentCounter = 0;
int usingThread;
int eventRadius;
int sub;

struct TMatrix{
    float **matrWas;
    float **matr;
    int nColmn;
    int mRows;
    int timesOfEvent;
    int lI;
    int lJ;
};

void plus(){
    pthread_mutex_lock(&mutex);
    ++threadCurrentCounter;
    pthread_mutex_unlock(&mutex);
}

void minus(){
    pthread_mutex_lock(&mutex);
    --threadCurrentCounter;
    pthread_mutex_unlock(&mutex);
}

float getMin(float** m, int i, int j) {
    float min = supInf;
    for (int localI = i - eventRadius; localI <= i + eventRadius; ++localI){
        for (int localJ = j - eventRadius; localJ <= j + eventRadius; ++localJ){
            if (m[localI][localJ] < min){
                min = m[localI][localJ];
            }
        }
    }
    return min;
}

float getMax(float** m, int i, int j) {
    float max = infInf;
    for (int localI = i - eventRadius; localI <= i + eventRadius; ++localI){
        if (i < 0) {
            continue;
        }
        for (int localJ = j - eventRadius; localJ <= j + eventRadius; ++localJ){
            if (j < 0) {
                continue;
            }
            if (m[localI][localJ] > max){
                max = m[localI][localJ];
            }
        }
    }
    return max;
}

void* getMinFunc(void* arg) {
    plus();
    struct TMatrix *funcMatr = arg;
    funcMatr -> matr[funcMatr -> lI][funcMatr-> lJ] = getMin(funcMatr -> matrWas, funcMatr -> lI, funcMatr -> lJ);
    pthread_mutex_unlock(&help);
    minus();
}

void* getMaxFunc(void* arg) {
    plus();
    struct TMatrix *funcMatr = arg;
    printf("max on %d %d : %lf\n", funcMatr -> lI, funcMatr -> lJ,getMax(funcMatr -> matrWas, funcMatr -> lI, funcMatr -> lJ));
    funcMatr -> matr[funcMatr -> lI][funcMatr-> lJ] = getMax(funcMatr -> matrWas, funcMatr -> lI, funcMatr -> lJ);
    pthread_mutex_unlock(&help);
    minus();
}

void* errosFunc(void* arg){
    plus();
    struct TMatrix *fMatr = arg;
    for (int q = 0; q < fMatr -> timesOfEvent; ++q){
        pthread_mutex_lock(&func);
        for (int i = 0; i < fMatr -> nColmn; ++i){
            free(fMatr -> matrWas[i]);
        }
        pthread_mutex_unlock(&func);
        fMatr -> matrWas = NULL;
        fMatr -> matrWas = (float**) malloc ((fMatr -> nColmn) * sizeof(float));
        pthread_mutex_lock(&func);
        for (int i = 0; i < fMatr -> nColmn; ++i){
            fMatr -> matrWas[i] = (float*) malloc ((fMatr -> mRows) * sizeof(float));
            for (int j = 0; j < fMatr -> mRows; ++j){
                fMatr -> matrWas[i][j] = fMatr -> matr[i][j];
            }
        }
        pthread_mutex_unlock(&func);
        if (usingThread <= threadCurrentCounter || usingThread <= 2){
            for (int i = eventRadius; i < fMatr -> nColmn - eventRadius; ++i) {
                for (int j = eventRadius; j < fMatr -> mRows - eventRadius; ++j) {
                    fMatr -> matr[i][j] = getMin(fMatr -> matrWas, i, j);
                }
            }
        }else {
            for (int i = eventRadius; i < fMatr -> nColmn - eventRadius; ++i) {
                for (int j = eventRadius; j < fMatr -> mRows - eventRadius; ++j) {
                    pthread_mutex_lock(&func);
                    printf("i = %d, j = %d\n", i, j);
                    pthread_mutex_lock(&help);
                    fMatr -> lI = i;
                    fMatr -> lJ = j;
                    pointer = (pointer + 1) % sub;
                    pthread_join(threads[pointer], NULL);
                    pthread_create(&threads[pointer], NULL, getMinFunc, &*fMatr);
                    pthread_mutex_unlock(&func);
                }
            }
        }
        pthread_mutex_lock(&func);
        for (int i = 0; i < sub; ++i) {
            pthread_join(threads[i], NULL);
        }
        pthread_mutex_unlock(&func);
    }
    minus();
}

void* escalFunc(void* arg){
    plus();
    struct TMatrix *fMatr = arg;
    for (int q = 0; q < fMatr -> timesOfEvent; ++q){
        pthread_mutex_lock(&func);
        for (int i = 0; i < fMatr -> nColmn; ++i){
            free(fMatr -> matrWas[i]);
        }
        pthread_mutex_unlock(&func);
        fMatr -> matrWas = NULL;
        fMatr -> matrWas = (float**) malloc ((fMatr -> nColmn) * sizeof(float));
        pthread_mutex_lock(&func);
        for (int i = 0; i < fMatr -> nColmn; ++i){
            fMatr -> matrWas[i] = (float*) malloc ((fMatr -> mRows) * sizeof(float));
            for (int j = 0; j < fMatr -> mRows; ++j){
                fMatr -> matrWas[i][j] = fMatr -> matr[i][j];
            }
        }
        pthread_mutex_unlock(&func);
        if (usingThread <= threadCurrentCounter || usingThread <= 2){
            for (int i = eventRadius; i < fMatr -> nColmn - eventRadius; ++i) {
                for (int j = eventRadius; j < fMatr -> mRows - eventRadius; ++j) {
                    fMatr -> matr[i][j] = getMax(fMatr -> matrWas, i, j);
                }
            }
        }else {
            for (int i = eventRadius; i < fMatr -> nColmn - eventRadius; ++i) {
                for (int j = eventRadius; j < fMatr -> mRows - eventRadius; ++j) {
                    pthread_mutex_lock(&func);
                    pthread_mutex_lock(&help);
                    fMatr -> lI = i;
                    fMatr -> lJ = j;
                    pointer = (pointer + 1) % sub;
                    pthread_join(threads[pointer], NULL);
                    pthread_create(&threads[pointer], NULL, getMaxFunc, &*fMatr);
                    pthread_mutex_unlock(&func);
                }
            }
        }
        pthread_mutex_lock(&func);
        for (int i = 0; i < sub; ++i) {
            pthread_join(threads[i], NULL);
        }
        pthread_mutex_unlock(&func);
    }
    minus();
}

int main(int argc, char const* argv[]) {
    int row;
    int column;
    int times;
    struct TMatrix errosionMatr;
    struct TMatrix escalateMatr;
    printf("Enter number of threads : \n");
    scanf("%d", &usingThread);
    if (usingThread > maxThreadsCount){
        usingThread = maxThreadsCount;
    }
    threads = (pthread_t*) malloc (usingThread * (sizeof (pthread_t)));
    for (int i = 0; i < usingThread; ++i) {
        threads[i] = 0;
    }
    sub = usingThread - 2;
    if (usingThread < minimumThreadsCount){
        printf("Too few threads\n");
        return 0;
    }
    printf("Enter radius of operations : \n");
    scanf("%d", &eventRadius);
    if (eventRadius < 0){
        printf("Would be 0!\n");
        eventRadius = 0;
    }
    if (eventRadius > maximumRadiusEvent){
        printf("Would be smaller!\n");
        eventRadius = maximumRadiusEvent;
    }
    printf("Enter size of matrix (N * M): \n");
    scanf("%d", &row);
    scanf("%d", &column);
    row += (2 * eventRadius);
    column += (2 * eventRadius);
    printf("Enter matrix: \n");
    errosionMatr.matrWas = (float**) malloc(sizeof(float) * column);
    errosionMatr.matr = (float**) malloc(sizeof(float) * column);
    escalateMatr.matrWas = (float**) malloc(sizeof(float) * column);
    escalateMatr.matr = (float**) malloc(sizeof(float) * column);
    for (int i = 0; i < column; ++i) {
        errosionMatr.matrWas[i] = (float*) malloc(sizeof(float) * row);
        errosionMatr.matr[i] = (float*) malloc(sizeof(float) * row);
        escalateMatr.matrWas[i] = (float*) malloc(sizeof(float) * row);
        escalateMatr.matr[i] = (float*) malloc(sizeof(float) * row);
        for (int j = 0; j < row; ++j) {
            if (i < eventRadius || i > column - eventRadius - 1 || j < eventRadius || j > row - eventRadius - 1) {
                errosionMatr.matrWas[i][j] = errosionMatr.matr[i][j] = supInf;
                escalateMatr.matr[i][j] = escalateMatr.matrWas[i][j] = infInf;
            } else {
                float in;
                scanf("%f", &in);
                errosionMatr.matrWas[i][j] = errosionMatr.matr[i][j] = in;
                escalateMatr.matrWas[i][j] = escalateMatr.matr[i][j] = in;
            }
        }
    }
    escalateMatr.nColmn = errosionMatr.nColmn = column;
    escalateMatr.mRows = errosionMatr.mRows = row;
    printf("Enter times of errosion and escalating: \n");
    scanf("%d", &times);
    errosionMatr.timesOfEvent = escalateMatr.timesOfEvent = times;
    pthread_create(&threads[sub], NULL, &errosFunc, &errosionMatr);
    if (threadCurrentCounter >= usingThread){
        pthread_join(threads[sub], NULL);
    }
    pthread_create(&threads[sub + 1], NULL, &escalFunc, &escalateMatr);
    pthread_join(threads[sub], NULL);
    pthread_join(threads[sub + 1], NULL);
    printf("Erosion matrix result:\n");
    for (int i = eventRadius; i < errosionMatr.nColmn - eventRadius; ++i){
        for (int j = eventRadius; j < errosionMatr.mRows - eventRadius; ++j){
            printf("%.2f\t", errosionMatr.matr[i][j]);
        }
        printf("\n");
    }
    printf("Escalation matrix result:\n");
    for (int i = eventRadius; i < escalateMatr.nColmn - eventRadius; ++i){
        for (int j = eventRadius; j < escalateMatr.mRows - eventRadius; ++j){
            printf("%.2f\t", escalateMatr.matr[i][j]);
        }
        printf("\n");
    }
    return 0;
}
