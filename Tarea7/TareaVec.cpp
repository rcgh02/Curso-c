#include <emmintrin.h>
#include <immintrin.h>
#include <ostream>
#include <pmmintrin.h>
#include <stdio.h>
#include <chrono>
#include <iostream>
#include <vector>
#include <random>
#include <xmmintrin.h>


#define TAMANO_ELEMENTOS 100

using namespace std;

typedef struct{
    double real;
    double imag;

} numero_complejo;

typedef struct {
    double vector_a;
    double vector_b;
} prodcuto_punto ;


extern "C" {
float AVX_dot_product(float *a, float *b, int tamano) {
    float total = 0;

    if (tamano < 8) {
        for (int i = 0; i < tamano; i++) {
            total += a[i] * b[i];
        }
        return total;
    }
    __m256 num1, num2, num3;
    __m128 top, bot;
    num3 = _mm256_setzero_ps();
    int i = 0;
    int limite = tamano - (tamano % 8); 

    for (int i = 0; i < limite; i+=8) {
        num1 = _mm256_loadu_ps(a + i);
        num2 = _mm256_loadu_ps(b + i);
        num3 = _mm256_fmadd_ps(num1, num2, num3);
    }
    
    num3 = _mm256_hadd_ps(num3, num3);

    top = _mm256_extractf128_ps(num3, 1);
    bot = _mm256_extractf128_ps(num3, 0);


    top = _mm_add_ps(top, bot);
    top = _mm_hadd_ps(top, top);

    _mm_store_ss(&total, top); 

    for (; i < tamano; i++) {
        total += a[i] * b[i];
    }
    return total;

}

void multiplicacion_avx2_doble(const numero_complejo* x, const numero_complejo* y, numero_complejo* z, int tamano) {
    
    for (int i = 0 ; i < tamano; i+=2) {    
        __m256d v_x = _mm256_loadu_pd((const double*)(x + i));
        __m256d v_y = _mm256_loadu_pd((const double*)(y + i));

        __m256d v_y_real = _mm256_permute_pd(v_y, 0x00); 
        __m256d v_y_imag = _mm256_permute_pd(v_y, 0x0F);
        __m256d v_x_perm = _mm256_permute_pd(v_x, 0x05);

        __m256d prod_real = _mm256_mul_pd(v_x, v_y_real);
        __m256d prod_imag = _mm256_mul_pd(v_x_perm, v_y_imag);

        __m256d res = _mm256_addsub_pd(prod_real, prod_imag);

        _mm256_storeu_pd((double*)(z + i), res);
    }
}

}

int main(){

    vector<numero_complejo> datos1(TAMANO_ELEMENTOS);
    vector<numero_complejo> datos2(TAMANO_ELEMENTOS);
    vector<numero_complejo> datos3_avx(TAMANO_ELEMENTOS);
    vector<numero_complejo> datos4_c(TAMANO_ELEMENTOS);

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> distribution(1,10);

    for (int i = 0; i < TAMANO_ELEMENTOS ; i++) {
        datos1[i].real = distribution(gen);
        datos1[i].imag = distribution(gen);
        
        datos2[i].real = distribution(gen);
        datos2[i].imag = distribution(gen);
    }

    vector<float> vector_a(TAMANO_ELEMENTOS);
    vector<float> vector_b(TAMANO_ELEMENTOS);
    uniform_real_distribution<float> distr_float(1.0f,10.0f);

    for (int i = 0; i < TAMANO_ELEMENTOS; i++) {
        vector_a[i] = distr_float(gen);
        vector_b[i] = distr_float(gen);

    }

    printf("Datos cargados\n");


    multiplicacion_avx2_doble(datos1.data(),datos2.data(),datos3_avx.data(),TAMANO_ELEMENTOS);
    
    float producto_punto_avx = AVX_dot_product(vector_a.data(), vector_b.data(),TAMANO_ELEMENTOS);
    // === REEMPLAZA EL BLOQUE DE IMPRESIÓN DEL MAIN POR ESTE BUCLE ===
    cout << "==========================================================" << endl;
    cout << "   COMPROBACIÓN DETALLADA DE MULTIPLICACIÓN COMPLEJA AVX2 " << endl;
    cout << "==========================================================" << endl << endl;

    // Recorremos los 10 elementos calculados uno por uno
    for (int i = 0; i < TAMANO_ELEMENTOS; i++) {
        printf("--- Elemento [%d] ---\n", i);
        printf("  Num A: (%2.4f) + (%2.4f)i\n", datos1[i].real, datos1[i].imag);
        printf("  Num B: (%2.4f) + (%2.4f)i\n", datos2[i].real, datos2[i].imag);
        printf("  -> Resultado:  %2.4f + %2.4fi\n\n", datos3_avx[i].real, datos3_avx[i].imag);
    }

    cout << "==========================================================" << endl;
    cout << "         COMPROBACIÓN DETALLADA DE PRODUCTO PUNTO         " << endl;
    cout << "==========================================================" << endl << endl;

    // Para el producto punto, imprimimos los 10 elementos que se sumaron
    cout << "  Vector A: [";
    for(int i = 0; i < TAMANO_ELEMENTOS; i++) printf("%2.2f%s", vector_a[i], (i == TAMANO_ELEMENTOS-1) ? "" : ", ");
    cout << "]" << endl;

    cout << "  Vector B: [";
    for(int i = 0; i < TAMANO_ELEMENTOS; i++) printf("%2.2f%s", vector_b[i], (i == TAMANO_ELEMENTOS-1) ? "" : ", ");
    cout << "]" << endl;

    printf("  -> RESULTADO AVX PRODUCTO PUNTO: %f\n\n", producto_punto_avx);

    return 0;


}