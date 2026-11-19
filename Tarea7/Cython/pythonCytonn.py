import numpy as np
import TareaCython  # Importamos modulo

tamano = 10

x_r = np.random.uniform(1.0, 10.0, tamano)
x_i = np.random.uniform(1.0, 10.0, tamano)
y_r = np.random.uniform(1.0, 10.0, tamano)
y_i = np.random.uniform(1.0, 10.0, tamano)

res_r, res_i = TareaCython.multiplicar_complejos_cython(x_r, x_i, y_r, y_i)
lista_res = [f"{res_r[i]:.2f} + {res_i[i]:.2f}i" for i in range(tamano)]
print("Complejos Cython: [" + ", ".join(lista_res) + "]")

vec_a = np.random.uniform(1.0, 10.0, tamano).astype(np.float32)
vec_b = np.random.uniform(1.0, 10.0, tamano).astype(np.float32)

dot_res = TareaCython.producto_punto_cython(vec_a, vec_b)
print(f"\nProducto Punto Cython: {dot_res:.4f}")
