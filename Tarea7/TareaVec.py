import ctypes
import numpy as np
import random

biblioteca = ctypes.CDLL('./TareaVec.so')

class NumeroComplejo(ctypes.Structure):
    _fields_= [
        ("real", ctypes.c_double),
        ("imag", ctypes.c_double)
    ]

biblioteca.multiplicacion_avx2_doble.argtypes = [
    ctypes.POINTER(NumeroComplejo),
    ctypes.POINTER(NumeroComplejo),
    ctypes.POINTER(NumeroComplejo),
    ctypes.c_int,
]

biblioteca.multiplicacion_avx2_doble.restype = None

biblioteca.AVX_dot_product.argtypes = [
    ctypes.POINTER(ctypes.c_float),
    ctypes.POINTER(ctypes.c_float),
    ctypes.c_int,
]
biblioteca.AVX_dot_product.restype = ctypes.c_float

tamano = 4

ArrayComplejo = NumeroComplejo * tamano
datos1 = ArrayComplejo()
datos2 = ArrayComplejo()
datos3 = ArrayComplejo()


for i in range(tamano):
    datos1[i].real = random.uniform(1.0,10.0)
    datos1[i].imag = random.uniform(1.0,10.0)
    #print("Datos "datos1[i].real,datos1[i].imag)
    datos2[i].real = random.uniform(1.0,10.0)
    datos2[i].imag = random.uniform(1.0,10.0)

biblioteca.multiplicacion_avx2_doble(datos1,datos2,datos3, tamano)

print(f"*** Multiplicacion de {tamano} números complejos ***")
for i in range(tamano):
    print(f"Elemento [{i}]: {datos3[i].real:.2f} + {datos3[i].imag:.2f}i")

print(f"*** Producto punto de dos vectores con {tamano} entradas cada uno***\n")

vec_a = np.random.uniform(1, 10, tamano).astype(np.float32)
vec_b = np.random.uniform(1, 10, tamano).astype(np.float32)


print("Vector A", np.round(vec_a,5).tolist())

print("Vector B", np.round(vec_b,5).tolist())


ptr_a = vec_a.ctypes.data_as(ctypes.POINTER(ctypes.c_float))
ptr_b = vec_b.ctypes.data_as(ctypes.POINTER(ctypes.c_float))

resultado_dot = biblioteca.AVX_dot_product(ptr_a, ptr_b, tamano)
print(f"\nResultado Producto Punto: {resultado_dot:.4f}")
