#include <stdio.h>
#include <emmintrin.h>
#include <immintrin.h>
#include <stdint.h>


union Vector2int{
    int64_t Num2[2];
    __m128i Num;
};

int main(){
    union Vector2int V8A_1;
    union Vector2int V8B_2;
    union Vector2int V8AB_RBa;
    union Vector2int V8AB_RAl;
    unsigned long long resultado_bajo;
    unsigned long long resultado_alto;
    
    unsigned long long resultado0;
    unsigned long long resultado1;
    unsigned long long resultado2;
    unsigned long long resultado3 = 0;
    
    unsigned char carry1 = 0;
    unsigned char carry2 = 0;
    

    //V8A_1.Num2[0] = 0xFFFFFFFFFFFFFFFFULL;
    //V8A_1.Num2[1] = 0xFFFFFFFFFFFFFFFFULL;
    //
    // Ejemplo 2
    V8A_1.Num2[0] = 0xFFFFFFFFFFFFAAAAULL;
    V8A_1.Num2[1] = 0xFFFFFFFFFFFFBBBBULL;
    // Otro numeros de 64 bits
    // 1 : 0x1111222233334444ULL
    // 2 : 0x5555666677778888ULL
    printf("El nùmero A de 128 bits es: %016llX %016llX \n", V8A_1.Num2[0], V8A_1.Num2[1]);
    //V8A_1.Num2[0] = 0x314ULL; //788;
    //V8A_1.Num2[1] = 0x143ULL; //323;


    printf("--- ---\n");
    printf("Parte alta (Num2[1]) en Hex: %016llX (Decimal: %llu)\n", V8A_1.Num2[1], V8A_1.Num2[1]);
    printf("Parte baja (Num2[0]) en Hex: %016llX (Decimal: %llu)\n", V8A_1.Num2[0], V8A_1.Num2[0]);
    
    printf("\nEl número A de 128 bits completo es:\n");
    printf("%016llX %016llX\n", V8A_1.Num2[1], V8A_1.Num2[0]);
    printf("--------------------------------\n");

    V8B_2.Num2[0] = 0xFFFFFFFFFFFFFFFFULL;
    V8B_2.Num2[1] = 0xFFFFFFFFFFFFFFFFULL;

    printf("El nùmero B de 128 bits es: %016llX %016llX \n", V8B_2.Num2[0], V8B_2.Num2[1]);

    //V8B_2.Num2[0] = 0x309ULL;
    //V8B_2.Num2[1] = 0x311ULL; 
    // mas ejemplos
    // 1: 0xDEADC0DECAFEBABEULL
    // 2: 0xBEEFFACEFEEDC0DEULL
    // 1: 0xFFFFFFFFFFFFFFFFULL
    // 2: 0x0000000000000000ULL

    printf("--- ---\n");
    printf("Parte alta (Num2[1]) en Hex: %016llX (Decimal: %llu)\n", V8B_2.Num2[1], V8B_2.Num2[1]);
    printf("Parte baja (Num2[0]) en Hex: %016llX (Decimal: %llu)\n", V8B_2.Num2[0], V8B_2.Num2[0]);
    
    printf("\nEl número A de 128 bits completo es:\n");
    printf("%016llX %016llX\n", V8B_2.Num2[1], V8B_2.Num2[0]);
    printf("--------------------------------\n");

    // PARTE BAJA DE A POR LA PARTE BAJA DE B

    resultado0 = _mulx_u64(V8A_1.Num2[0], V8B_2.Num2[0], &resultado_alto);
    resultado1 = resultado_alto;

    // PARTE BAJA DE A POR LA PARTE ALTA DE B

    resultado_bajo = _mulx_u64(V8A_1.Num2[0], V8B_2.Num2[1], &resultado_alto);
    carry1 = _addcarry_u64(0,resultado1,resultado_bajo, &resultado1);
    _addcarry_u64(carry1, 0, resultado_alto, &resultado2);

    // PARTE ALTA DE A POR LA PARTE BAJA DE B

    resultado_bajo = _mulx_u64(V8A_1.Num2[1], V8B_2.Num2[0], &resultado_alto);
    carry1 = _addcarry_u64(0,resultado1,resultado_bajo, &resultado1);
    carry2 = _addcarry_u64(carry1,resultado2,resultado_alto, &resultado2);
    _addcarry_u64(carry2, 0, 0, &resultado3);

    // PARTE ALTA DE A POR LA PARTE ALTA DE B

    resultado_bajo = _mulx_u64(V8A_1.Num2[1], V8B_2.Num2[1], &resultado_alto);
    carry1 = _addcarry_u64(0,resultado2,resultado_bajo, &resultado2);
    _addcarry_u64(carry1, resultado3, resultado_alto, &resultado3);


    V8AB_RBa.Num = _mm_set_epi64x(resultado1, resultado0); 
    V8AB_RAl.Num = _mm_set_epi64x(resultado3, resultado2);


    printf("Multiplicacion:\n");
    printf("Resultado Alto: %016llX %016llX\n", 
            (unsigned long long)V8AB_RAl.Num2[1], (unsigned long long)V8AB_RAl.Num2[0]);
    printf("Resultado Bajo:  %016llX %016llX\n", 
            (unsigned long long)V8AB_RBa.Num2[1], (unsigned long long)V8AB_RBa.Num2[0]);

    printf("El resultado es :  %016llX %016llX %016llX %016llX \n", (unsigned long long)V8AB_RAl.Num2[1], (unsigned long long)V8AB_RAl.Num2[0],(unsigned long long)V8AB_RBa.Num2[1], (unsigned long long)V8AB_RBa.Num2[0]);


    return 0;

}
