#include <time.h>
#include <memory>
#include "iostream"
#include <chrono> 
extern "C"
{
    #include <immintrin.h>
}

using namespace std;

typedef unsigned long long bench_t;

static bench_t before;
static bench_t after;

static inline bench_t cycles(void){
    unsigned int hi, lo;
    __asm__ __volatile__ ("rdtsc\n\t":"=a" (lo), "=d"(hi));
    return ((bench_t) lo | (((bench_t) hi) << 32));
}


double horner(double X, double *coef, long size){
    double ACC =0.0;
    int i;
    for (i=0;i<size;i++){
        ACC = (ACC+coef[i])*X;
    }
    return ACC;
}

double horner_intrinsic(double X,double *coef, long size){
    double *R,P;
    int i;
    __m256 *ymm0,X256,Y;

    ymm0 = (__m256*)coef;

    X256 = _mm256_set1_ps(X*X*X*X);

    Y = _mm256_set1_ps(0.0);

    for(i=0;i<size/4-1;i++){
        Y = _mm256_add_ps(Y,ymm0[i]);
        Y = _mm256_mul_ps(Y,X256);
    }

    Y = _mm256_add_ps(Y,ymm0[i]);

    R = (double *)(&Y);

    P = R[3]*X;
    P += R[2]*X*X;
    P += R[1]*X*X*X;
    P += R[0]*X*X*X*X;

    return P;
}

int main(){
    alignas(32) double coef[4000];
    double X=1.1;
    double R;
    double R1, R2;
    int i, num_traits = 10000000;

    bench_t t1, t2;

    srand(time(NULL));

    double *coeficientes;
    int j;

    coeficientes = (double *)_mm_malloc(100*sizeof(double), 32);

    for (j=0;j<1;j++){
        for(i=0; i<100; i++){
            coeficientes[i]=(double)(rand()%1000)/1000;
            if(i<10){
                cout<< coeficientes[i] <<endl;
            }
        }
        R = horner(X,coeficientes,1000);
        cout<< R <<endl;
        R = horner_intrinsic(X,coeficientes,1000);
        cout << R <<endl;
    }

    //auto t1_normal = chrono::high_resolution_clock::now();
    auto t1_normal = chrono::steady_clock::now();
    //auto t1_normal = chrono::system_clock::now();
    for(int j = 0; j < num_traits; j++){
        R1 = horner(X,coeficientes,100);
    }

    //auto t2_normal = chrono::high_resolution_clock::now();
    auto t2_normal = chrono::steady_clock::now();
    //auto t2_normal = chrono::system_clock::now();
    chrono::duration<double, milli> tiempo_normal = t2_normal - t1_normal;

    cout << "--- METODO HORNER NORMAL ---" << endl;
    cout << "Resultado final: " << R1 << endl;
    cout << "Tiempo total (" << num_traits << " ejecuciones): " << tiempo_normal.count() << " ms" << endl;
    cout << "Tiempo promedio por iteracion: " << (tiempo_normal.count() * 1000) / num_traits << " us (microsegundos)" << endl;
    cout << "--------------------------------\n" << endl;

    
    //auto t1_intrin = chrono::high_resolution_clock::now();
    auto t1_intrin = chrono::steady_clock::now();
    //auto t1_intrin = chrono::system_clock::now();
    for(int j = 0; j < num_traits; j++){
        R2 = horner_intrinsic(X,coeficientes,100);
    }

    //auto t2_intrin = chrono::high_resolution_clock::now();
    auto t2_intrin = chrono::steady_clock::now();
    //auto t2_intrin = chrono::system_clock::now();
    chrono::duration<double, milli> tiempo_intrinsic = t2_intrin - t1_intrin;

    cout << "--- METODO HORNER (INTRINSICS) ---" << endl;
    cout << "Resultado final: " << R2 << endl;
    cout << "Tiempo total (" << num_traits << " ejecuciones): " << tiempo_intrinsic.count() << " ms" << endl;
    cout << "Tiempo promedio por iteracion: " << (tiempo_intrinsic.count() * 1000) / num_traits << " us (microsegundos)" << endl;
    cout << "--------------------------------" << endl;

    _mm_free(coeficientes);

}