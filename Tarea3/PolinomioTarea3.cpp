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


float horner(float X, float *coef, long size){
    float ACC =0.0;
    int i;
    for (i=0;i<size;i++){
        ACC = (ACC+coef[i])*X;
    }
    return ACC;
}

float horner_intrinsic(float X,float *coef, long size){
    float P;
    int i;
    __m256 *ymm0,X256,Y;

	float x2 = X * X;
	float x4 = x2 * x2;
	float x8 = x4 * x4;

    ymm0 = (__m256*)coef;

    X256 = _mm256_set1_ps(x8);

    Y = _mm256_set1_ps(0.0f);

    for(i=0;i<size/8-1;i++){
        __m256 coef_vec = _mm256_loadu_ps(&coef[i * 8]);
        Y = _mm256_add_ps(Y,ymm0[i]);
        Y = _mm256_mul_ps(Y,X256);
    }
	__m256 coef_vec = _mm256_loadu_ps(&coef[i * 8]);
    Y = _mm256_add_ps(Y, coef_vec);
    //Y = _mm256_add_ps(Y,ymm0[i]);

    alignas(32) float R[8];
	_mm256_store_ps(R, Y);
    
    //R = (float *)(&Y);

    P =  R[7] * X;
    P += R[6] * (X * X);
    P += R[5] * (X * X * X);
    P += R[4] * (X * X * X * X);
    P += R[3] * (X * X * X * X * X);
    P += R[2] * (X * X * X * X * X * X);
    P += R[1] * (X * X * X * X * X * X * X);
    P += R[0] * (X * X * X * X * X * X * X * X);;

    return P;
}

int main(){
    //alignas(32) float coef[4000];
    float X=0.5f;
    float R;
    float R1, R2;
    int i, num_traits = 100000;

    bench_t t1, t2;

    srand(time(NULL));

    float *coeficientes;
    int j;

    coeficientes = (float *)_mm_malloc(10000*sizeof(float), 32);

    for (j=0;j<1;j++){
        for(i=0; i<10000; i++){
            coeficientes[i]=(float)(rand()%1000)/1000;
            if(i<10){
                cout<< coeficientes[i] <<endl;
            }
        }
        R = horner(X,coeficientes,10000);
        cout<< R <<endl;
        R = horner_intrinsic(X,coeficientes,10000);
        cout << R <<endl;
    }

    auto t1_normal = chrono::steady_clock::now();
    //auto t1_normal = chrono::high_resolution_clock::now();
    //auto t1_normal = chrono::system_clock::now();
    for(int j = 0; j < num_traits; j++){
        R1 = horner(X,coeficientes,10000);
    }
    
    auto t2_normal = chrono::steady_clock::now();
    //auto t2_normal = chrono::high_resolution_clock::now();
    //auto t2_normal = chrono::system_clock::now();
    chrono::duration<float, milli> tiempo_normal = t2_normal - t1_normal;

    cout << "--- METODO HORNER ---" << endl;
    cout << "Resultado final: " << R1 << endl;
    cout << "Tiempo total (" << num_traits << " ejecuciones): " << tiempo_normal.count() << " ms" << endl;
    cout << "Tiempo promedio por iteracion: " << (tiempo_normal.count() * 1000) / num_traits << " us (microsegundos)" << endl;
    cout << "--------------------------------\n" << endl;

    auto t1_intrin = chrono::steady_clock::now();    
    //auto t1_intrin = chrono::high_resolution_clock::now();
    //auto t1_intrin = chrono::system_clock::now();
    for(int j = 0; j < num_traits; j++){
        R2 = horner_intrinsic(X,coeficientes,10000);
    }

    //auto t2_intrin = chrono::high_resolution_clock::now();
    //auto t2_intrin = chrono::system_clock::now();
    auto t2_intrin = chrono::steady_clock::now();

    chrono::duration<float, milli> tiempo_intrinsic = t2_intrin - t1_intrin;

    cout << "--- METODO HORNER CON INTRINSICS ---" << endl;
    cout << "Resultado final: " << R2 << endl;
    cout << "Tiempo total (" << num_traits << " ejecuciones): " << tiempo_intrinsic.count() << " ms" << endl;
    cout << "Tiempo promedio por iteracion: " << (tiempo_intrinsic.count() * 1000) / num_traits << " us (microsegundos)" << endl;
    cout << "--------------------------------" << endl;

    _mm_free(coeficientes);
    return 0;
}