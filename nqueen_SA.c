/* =====================================================================
焼きなまし法(シミュレーテッド・アニーリング: Simulated Annealing)でNクイーン問題を解く

実行方法
$ gcc -Wall -o nqueen_SA nqueen_SA.c -lm
$ ./nqueen_SA

Written by L3onSW 2023/10/05
===================================================================== */

#include <stdio.h> // stdin,stdout,stderr
#include <stdlib.h> // EXIT_FAILURE,rand(),srand(),RAND_MAX,malloc(),calloc(),free
#include <stdbool.h> // bool,true,false
#include <time.h> // time()
#include <string.h> // memcpy()
#include <math.h> // exp()

// クイーンの数(2桁(~99)までは綺麗に表示できる)
#define N 8
// 冷却度
#define ALPHA 0.98
// 初期温度
#define T0 30
// 終了温度
#define T1 0.05
// ステップ数の上限
#define STEPNUM 100

// クイーンの位置をランダムに配置させる
void randomly_replace(int* Queen);
// クイーンの位置の初期化
void initialize_queen(int* Queen);
// クイーンが衝突する位置にいるかどうか
void check_conflict(int* Queen, bool* cflags);
// 衝突する位置にいるクイーンの数
int sum_conflict(bool* cflags);
// チェス盤の表示
void plot_chess(int* Queen, bool* cflags, int E, int t);
// 値の交換
void swap(int* a, int* b);
// ランダムに2つのクイーンを選び、入れかえる
void tweak(int* Queen);


// メイン関数
int main(void){
    printf("%d-Queens Problem using Simulated Annealing\n", N); // クイーンの数を表示
    double t = T0; // 温度
    int Queen[N]; // クイーンの位置
    bool cflags[N]; // クイーンが衝突する位置にいるかどうか(true:衝突する，false:衝突しない)
    initialize_queen(Queen); // クイーンの位置の初期状態
    check_conflict(Queen, cflags); // クイーンが衝突する位置にいるかどうかの初期状態
    int E = sum_conflict(cflags); // 衝突する位置にいるクイーンの数の初期状態
    int Queen_best[N]; // 最良のクイーンの位置
    memcpy(Queen_best, Queen, sizeof(Queen)); // Queen_bestにQueenをコピー
    bool conflict_best[N]; // 最良のクイーンが衝突する位置にいるかどうか
    memcpy(conflict_best, cflags, sizeof(cflags)); // conflict_bestにcflagsをコピー
    int E_best = E; // 最良の衝突する位置にいるクイーンの数
    plot_chess(Queen, cflags, E_best,t); // 初期状態でのチェス盤の表示
    srand((unsigned)time(0));
    
    // 以下、焼きなまし法(シミュレーテッド・アニーリング: Simulated Annealing)
    while(t > T1){
        for(int i = 0; i < STEPNUM; i++){
            int Queen_tmp[N]; // 位置を交換してみた場合のクイーン位置
            memcpy(Queen_tmp, Queen, sizeof(Queen)); // Queen_tmpにQueenをコピー
            bool conflict_tmp[N]; // 位置を交換してみた場合のクイーンが衝突する位置にいるかどうか
            memcpy(conflict_tmp, cflags, sizeof(cflags)); // conflict_tmpにcflagsをコピー
            tweak(Queen_tmp); // クイーンの位置を入れ替えてみる
            check_conflict(Queen_tmp, conflict_tmp); // 入れ替えた場合に衝突する位置にいるかどうか
            int E_tmp = sum_conflict(conflict_tmp); // 入れ替えた場合の衝突する位置にいるクイーンの数
            if(E_tmp <= E){
                memcpy(Queen, Queen_tmp, sizeof(Queen_tmp)); // QueenをQueen_tmpで更新
                memcpy(cflags, conflict_tmp, sizeof(conflict_tmp)); // cflagsをconflict_tmpで更新
                E = E_tmp; // EをE_tmpで更新
            }else{
                int r = rand() / RAND_MAX; // 0以上1未満の乱数
                int dE = E_tmp - E;
                int xi = exp(-dE / t);
                if(xi > r){
                    memcpy(Queen, Queen_tmp, sizeof(Queen_tmp)); // QueenをQueen_tmpで更新
                    memcpy(cflags, conflict_tmp, sizeof(conflict_tmp)); // cflagsをconflict_tmpで更新
                    E = E_tmp;// EをE_tmpで更新
                }
            }
            if(E < E_best){
                memcpy(Queen_best, Queen, sizeof(Queen)); // Queen_bestをQueenで更新
                memcpy(conflict_best, cflags, sizeof(cflags)); // conflict_bestをcflagsで更新
                E_best = E; // E_bestをEで更新
            }    
        }
        plot_chess(Queen, cflags, E, t); // 各温度でのチェス盤の表示
        t = ALPHA * t; // 指数冷却により温度を更新
        if(E == 0){
            break; // 衝突する位置にいるクイーンの数が0なら終了
        }
    }
    // 焼きなまし法終了後のチェス盤の表示(ここは重複するので無くてよい)
    // plot_chess(Queen_best, conflict_best, E, t);
    return 0;
}


// クイーンの位置をランダムに配置させる
void randomly_replace(int* Queen){
    for(int i = 0; i < N; i++){
        int j = rand() % N;
        int tmp = Queen[i];
        Queen[i] = Queen[j];
        Queen[j] = tmp;
    }
}

// クイーンの位置の初期化
void initialize_queen(int* Queen){
    for(int i = 0; i < N; i++){
        Queen[i] = i;
    }
    randomly_replace(Queen);
}


// クイーンが衝突する位置にいるかどうか
void check_conflict(int* Queen, bool* cflags){
    int i, j;
    for(i = 0; i < N; i++){
        cflags[i] = false; // クイーンが衝突する位置に居ないとして初期化
    }
    for(i = 0; i < N - 1; i++){
        for(j = i + 1; j < N; j++){
            int queen = Queen[i]; // 第i列目でのクイーンの位置
            if(queen == Queen[j]){
                cflags[j] = true; // 右横方向にクイーン居るなら衝突するのでtrue
            }else if(queen-(j-i) == Queen[j]){
                cflags[j]=true; // 右上方向にクイーン居るなら衝突するのでtrue
            }else if(queen+(j-i) == Queen[j]){
                cflags[j] = true; // 右下方向にクイーン居るなら衝突するのでtrue
            }
        }
    }
}

// 衝突する位置にいるクイーンの数
int sum_conflict(bool* cflags){
    int sum = 0;
    for(int i = 0;i < N; i++){
        if(cflags[i] == true){
            sum++;
        }
    }
    return sum;
}


// チェス盤の表示
// コンソールの文字色・背景色を変えられることを利用
// 2桁までは綺麗に表示できる
void plot_chess(int* Queen, bool* cflags, int E, int t){
    printf("E=%d, t=%d\n", E, t);
    for(int x = N-1; x >= 0; x--){
        // 黒色背景・白色文字で数字(チェス盤のサイズ)を表示
        printf("\x1b[40m\x1b[37m%2d\x1b[39m\x1b[49m", x+1);
        for(int y = 0; y < N; y++){
            if(Queen[y]==x && cflags[y]==true){
                // 衝突する位置のクイーンは，赤色背景・黒色文字で表示
                printf("\x1b[41m\x1b[30m Q \x1b[39m\x1b[49m");
            }else if(Queen[y]==x && cflags[y]==false){
                // 衝突しない位置のクイーンは，青色背景・黒色文字で表示
                printf("\x1b[44m\x1b[30m Q \x1b[39m\x1b[49m");
            }else{
                // クイーンがないチェス盤のマス目を白色背景で表示
                printf("\x1b[47m   \x1b[49m");
            }
        }
        printf("\n");
    }
    printf("  ");
    for(int y = 0; y < N; y++){
        // 黒色背景・白色文字で数字(チェス盤のサイズ)を表示
        printf("\x1b[40m\x1b[37m%2d \x1b[39m\x1b[49m", y+1);
    }
    printf("\n");
}


// 値の交換
void swap(int* a, int* b){
    int tmp = *a;
    *a = *b;
    *b = tmp;
}


// ランダムに2つのクイーンを選び、入れかえる
// tweak: 微調整
void tweak(int* Queen){
    int index1 = rand() % N; // 入れ替えるクイーン1つ目の添え字
    int index2 = rand() % N; // 入れ替えるクイーン2つ目の添え字
    while(index1 == index2){
        index2 = rand() % N; // 1つ目と2つ目を違う添え字にする
    }
    swap(&Queen[index1], &Queen[index2]); // クイーンの位置を入れ替える
}
