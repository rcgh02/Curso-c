#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>

#define PI 3.14159

#define BITS_VAR 32
#define BITS_TOTAL (BITS_VAR * 2)
#define A_RANGO -5.12
#define B_RANGO 5.12

using namespace std;

// INDIVIDUO
typedef struct
{
    /* data */
    vector<int> cromosomas;
    // int cromosoma[BITS_TOTAL];
    double x;
    double y;
    double fitness;
} Individuo;

double evaluarFuncion(double x, double y)
{
    return 20.0 + (x * x - 10 * cos(2.0 * PI * x)) + (y * y - 10 * cos(2.0 * PI * y));
}

double binarioADecimal(const vector<int> &bits, int inicio, int fin)
{
    unsigned long long valorDecimal = 0;

    // binario a enetero utilizando desplazamioento de bits
    // a<<b = a*2^b
    // a>>b = floor(a/2^b)
    for (int i = inicio; i <= fin; ++i)
    {
        valorDecimal = (valorDecimal << 1) | bits[i];
    }

    // evitando desbordamiento
    unsigned long long maxVal = (1ULL << BITS_VAR) - 1;

    // FORMULA
    return A_RANGO + ((double)valorDecimal / (double)maxVal) * (B_RANGO - A_RANGO);
}

// generando poblacion inicial
vector<Individuo> generarPoblacion(int tamPoblacion, mt19937 &gen)
{
    vector<Individuo> poblacion(tamPoblacion);
    uniform_int_distribution<> dis(0, 1);

    for (int i = 0; i < tamPoblacion; ++i)
    {
        poblacion[i].cromosomas.resize(BITS_TOTAL);

        for (int j = 0; j < BITS_TOTAL; ++j)
        {
            poblacion[i].cromosomas[j] = dis(gen);
        }

        // Decodificar X (0 31)
        poblacion[i].x = binarioADecimal(poblacion[i].cromosomas, 0, BITS_VAR - 1);

        // Decodificar Y (32 63)
        poblacion[i].y = binarioADecimal(poblacion[i].cromosomas, BITS_VAR, BITS_TOTAL - 1);

        // EVALUAR FUNCION
        poblacion[i].fitness = evaluarFuncion(poblacion[i].x, poblacion[i].y);
    }
    return poblacion;
}

// Cruza de genes
// probabilidad .9
// Seleccionar dos padres aleatorios de la poblacion
// si un numero aleatorio es menor o igual a 0.9
//      elegimos un punto de cruza aleatorio de la cadena de 64 bits
//      el hijo 1 hereda los bits del padre 1 hasta el punto de cruza y el resto al padre 2
//      el hujo 2 hereda los bits del padre 2 hasta el punto de cruza y el resto al padre 1
// si no ocurre la cruza los hijo son copias exactas de sus padres

// funcion cruzar dos padres y generar dos hijo
// Generamos al azar un valor de 0 a 63 que será el punto de
// corte para poder realizar la cruza de sus bits, ejemplo si es 38
// al hijo 1 se le pasaran todos los bits del 38 a 63 es decir la cola
// de bits, al hijo 2 se le pasarán los cromosamos del padre 1
// la probabilidad de cruza es de 0.9
// y regresamos 2 hijos
pair<Individuo, Individuo> cruzar(
    const Individuo &padre1,
    const Individuo &padre2,
    double probCruza,
    mt19937 &gen)
{
    Individuo hijo1 = padre1;
    Individuo hijo2 = padre2;
    uniform_real_distribution<> disReal(0.0, 1.0);

    if (disReal(gen) <= probCruza)
    {
        // elegimos punto de corte de entre 1 y bits_total - 1 (0 63)
        uniform_int_distribution<> disPunto(1, BITS_TOTAL - 1);
        int puntoCorte = disPunto(gen);

        for (int i = puntoCorte; i < BITS_TOTAL; ++i)
        {
            hijo1.cromosomas[i] = padre2.cromosomas[i];
            hijo2.cromosomas[i] = padre1.cromosomas[i];
        }
    }
    return {hijo1, hijo2};
};

// Mutacion de genes
// la mutacion sirve para evitar que el algoritmo se quede atrapado en soluciones
// poco eficientes
// para eso introduciremos cambios aleatorios
// recorremos todos los bits de los hijos
// definimos una probabilidad muy baja de mutacion por bit por ejemplo 0.01 o 1 %
// si el valor aleatorio es menor o igual a esa probabilidad es decir 0.01,
// invertimos el bit, si es 0 pasa a 1 si es 1 pasa a 0

void mutar(Individuo &ind, double probMutacion, mt19937 &gen)
{
    uniform_real_distribution<> disReal(0.0, 1.0);

    for (int i = 0; i < BITS_TOTAL; ++i)
    {
        if (disReal(gen) <= probMutacion)
        {
            ind.cromosomas[i] = 1 - ind.cromosomas[i];
        }
    }
}

int main()
{
    int tamPoblacion = 1000;
    double probCruza = 0.9;
    double probMutacion = 0.01;
    int numGeneraciones = 10;
    // cruzar -> mutar -> decodificar -> evaluar
    random_device rd;
    mt19937 gen(rd());

    // 1.. Generamos a la poblacion inicial
    vector<Individuo> poblacion = generarPoblacion(tamPoblacion, gen);

    // ciclo generaqciones
    for (int genActual = 1; genActual <= numGeneraciones; ++genActual)
    {
        vector<Individuo> hijos;

        // generando hijos mediante cruza y mutacion
        while (hijos.size() < (unsigned int)tamPoblacion)
        {
            uniform_int_distribution<> disPadres(0, tamPoblacion - 1);
            Individuo padre1 = poblacion[disPadres(gen)];
            Individuo padre2 = poblacion[disPadres(gen)];

            // cruza paso 3
            // es una forma de sacar datos de un pair, y guardalos
            // en variables hijo1 e hijo2
            auto [hijo1, hijo2] = cruzar(padre1, padre2, probCruza, gen);

            // mutar paso 4
            mutar(hijo1, probMutacion, gen);
            mutar(hijo2, probMutacion, gen);

            // paso 5
            hijo1.x = binarioADecimal(hijo1.cromosomas, 0, BITS_VAR - 1);
            hijo1.y = binarioADecimal(hijo1.cromosomas, BITS_VAR, BITS_TOTAL - 1);
            hijo1.fitness = evaluarFuncion(hijo1.x, hijo1.y);

            hijo2.x = binarioADecimal(hijo2.cromosomas, 0, BITS_VAR - 1);
            hijo2.y = binarioADecimal(hijo2.cromosomas, BITS_VAR, BITS_TOTAL - 1);
            hijo2.fitness = evaluarFuncion(hijo2.x, hijo2.y);

            // meter en el cajon hecho linea 157 al final de la fila
            hijos.push_back(hijo1);
            hijos.push_back(hijo2);
        }
        // Unimos a los 500 padres + 500 hijos (ahora tenemos 100 individuos)
        poblacion.insert(poblacion.end(), hijos.begin(), hijos.end());
        // 1. Ordenamos de menor a mayor fitness (porque buscamos minimizar)
        sort(poblacion.begin(), poblacion.end(), [](const Individuo &a, const Individuo &b)
             { return a.fitness < b.fitness; });
        // 2. Redimensionamos a 1000 para descartar a los 1000 peores (los de abajo)
        poblacion.resize(tamPoblacion);

        cout << "Generacion " << genActual << " | Mejor Fitness: " << poblacion[0].fitness
             << " | x: " << poblacion[0].x << " | y: " << poblacion[0].y << "\n";
    }
    return 0;
}
