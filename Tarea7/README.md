# Tarea 7 Creación de biblioteca y utilizarla en un script de python

Se realizó la multiplicacion de números complejos y producto punto de dos vectores con intrinsics.

La multiplicación de números complejos

Sea a,b $\in C$ dos números complejos de la forma $a = x + yi$, para realizar la multiplicación de
estos números se hace lo siguiente



$$
\mathbf{a} = x + yi
$$


$$
\mathbf{b} = k + zi
$$


$$
\mathbf{i*i} = -1
$$


$$
\mathbf{c} = x*k + x*zi + k*yi - y*z
$$

De esta forma tenemos 

$$
\mathbf{c} = x*k - y*z + i(x*z + k*y)
$$


Para este ejercicio se generarón números aleatorios entre 1.0 y 10.0, y se formaron estos números complejos, para efectuar 4 multiplicaciones se generarón 8 numeros complejos al azar.

Por ejemplo

![screenshot](imagenes/imag1.png)


## Intrisics utilizados

* `_mm256_loadu_pd`: Carga 256 bits en 4 paquetes de 64 bits. NO necesita ser alineado en memoria.
 
* `_mm256_permute_pd`: Reordena elementos de puntos flotante de doble precision (64 bits) dentro de segmentos de 128 bits

* `_mm256_mul_pd`: Multiplica elementos de 64 bits empaquetados en a y b. Se almacena resultado.

* `_mm256_addsub_pd`: Suma y resta alternadamente elementos de punto flotante de doble precision (64 bits) en paquetes a y b. Almacena resultado.

* `_mm256_storeu_pd`: Almacena 256 bits en 4 paquetes de elementos de puntos flotante de presicion simple (64 bits). NO nocesita estar alineado en memoria.






# Producto punto 

Sea a,b $\in R^{n}$, donde a, b son vectores de longitud n. El producto punto se define de la siguiente forma

Sea 

$$
\mathbf{a} = [a_1, a_2, a_3, \dots, a_n]
$$



$$
\mathbf{b} = [b_1, b_2, b_3, \dots, b_n]
$$

$$
\mathbf{a} \cdot \mathbf{b} = [a_1, a_2, a_3, \dots, a_n] \cdot [b_1, b_2, b_3, \dots, b_n] = \sum_{i=1}^{n} a_i b_i
$$

De la misma forma se generan vectores a y b aleatorios



* `_mm256_loadu_ps`: Carga 256 bits en 8 bloques de punto flotante de precisión simple (32 bits).
* `_mm256_fmadd_ps`: Multiplica los elementos de punto flotante de 32 bits empaquetados en `a` y `b`, y suma el elemento intermedio empaquetado en `c`.
* `_mm256_hadd_ps`: Realiza la suma horizontal de paquetes de 32 bits adyacentes y los almacena en el destino.
* `_mm256_extractf128_ps`: Extrae 128 bits (4 paquetes de punto flotante de 32 bits) y los almacena en una variable de 128 bits.
* `_mm256_add_ps`: Suma elementos de punto flotante de precisión simple (32 bits) empaquetados en `a` y `b`, y almacena los resultados.
* `_mm_store_ss`: Almacena el primer elemento de punto flotante de 32 bits en memoria (`mem_addr` no necesita estar alineado en ningún límite en particular).
* `_mm256_setzero_ps`: Inicializa todos los bits del registro de 256 bits con ceros.

## Intriscs utilizados

* `_mm256_loadu_ps`: cargar 256 bit en 8 bloques de 32 bits.

* `_mm256_fmadd_ps`: Multiplicar los elementos del punto flotante de 32 bits enpaquetados en a y b, y el elemento intermedio 
c es sumado.

* `_mm256_hadd_ps`: Hacer la suma horizontal de paquetes de 32 bits adyacentes y los almacena en el destino.
 
* `_mm256_extractf128_ps`: Extraer 128 bits en 4 paquetes de punto flotante de 32 bits, y almacenados en una variable de 128 bits

* `_mm256_add_ps`: Sumar elementos de punto flotante de precision simple (32 bits) empaquetados en a y b, y almacena los resultados.

* `_mm_store_ss`: Almacena el elemento de punto flotante inferior de 32 bits en memoria. mem_addr no necesita estar alineado en ningún límite en particular.

* `_mm256_setzero_ps`: INicializa registros de 256 bits con ceros.

## Requerimientos 

C++17

## Compilación 

```bash
$ g++ huffman.c -o huffman.o
```

## Ejecución

```bash
$ ./huffman.o
```

![alt text](imagenes/Pantallahuffman.png)

Al elegir 1

![alt text](imagenes/Seleccionando.png)


![alt text](imagenes/tablaHuffman.png)

Al elegir 2 decodificamos

![alt text](imagenes/decodoficando.png)

Al codificar en un archivo binario se puede ver que hubo una compresion der archivo.

![alt text](imagenes/compresion.png)





# Bibliografía

https://github.com/oneapi-src/oneAPI-samples/blob/master/DirectProgramming/C%2B%2B/CompilerInfrastructure/Intrinsics/src/intrin_dot_sample.cpp

https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html