#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include <vector>
#include <fstream>

using namespace std;

// Estructura para los nodos del árbol de Huffman
struct MinNode
{
    char caracter;
    unsigned frecuencia;
    MinNode *izquierda, *derecha;

    MinNode(char caracter, unsigned frecuencia)
    {
        izquierda = derecha = nullptr;
        this->caracter = caracter;
        this->frecuencia = frecuencia;
    }

    ~MinNode()
    {
        delete izquierda;
        delete derecha;
    }
};

// Objeto de comparación para la cola de prioridad (Min-Heap)
struct Comparar
{
    bool operator()(MinNode *l, MinNode *r)
    {
        return (l->frecuencia > r->frecuencia);
    }
};

// Función para generar los códigos de Huffman a partir del árbol
void generarCodigos(MinNode *raiz, string str, unordered_map<char, string> &codigosHuffman)
{
    if (!raiz)
        return;

    if (raiz->caracter != '\0')
    {
        codigosHuffman[raiz->caracter] = str;
    }

    generarCodigos(raiz->izquierda, str + "0", codigosHuffman);
    generarCodigos(raiz->derecha, str + "1", codigosHuffman);
}

void serializarArbol(MinNode *raiz, ofstream &archivo)
{
    if (!raiz)
        return;

    if (!raiz->izquierda && !raiz->derecha)
    {
        archivo << '1' << raiz->caracter;
    }
    else
    {
        archivo << '0';
        serializarArbol(raiz->izquierda, archivo);
        serializarArbol(raiz->derecha, archivo);
    }
}

MinNode *deserializarArbol(ifstream &archivo)
{
    char token;
    if (!archivo.get(token))
    {
        return nullptr;
    }

    if (token == '1')
    {
        char caracter;
        archivo.get(caracter);
        return new MinNode(caracter, 0);
    }
    else
    {
        MinNode *nodoInterno = new MinNode('\0', 0);
        nodoInterno->izquierda = deserializarArbol(archivo);
        nodoInterno->derecha = deserializarArbol(archivo);
        return nodoInterno;
    }
}

void guardarTexto(string texto, unordered_map<char, string> &codigosHuffman, MinNode *raiz)
{
    string strCodificado = "";
    for (char c : texto)
    {
        strCodificado += codigosHuffman[c];
    }

    ofstream archivo("Archivo_codificado.bin", ios::binary);
    if (archivo.is_open())
    {
        // 1. Guardar estructura del árbol
        serializarArbol(raiz, archivo);
        archivo << endl;

        // 2. Guardar cuántos bits útiles tiene la cadena binaria (crucial para no leer basura al final)
        int totalBits = strCodificado.length();
        archivo.write(reinterpret_cast<const char *>(&totalBits), sizeof(totalBits));

        // 3. EMPAQUETADO BINARIO: Convertir string de '0' y '1' a bits reales
        char byteActual = 0;
        int contadorBits = 0;

        for (char bit : strCodificado)
        {
            // desplazamiento bits izq.
            byteActual <<= 1;
            if (bit == '1')
            {
                byteActual |= 1;
            }
            contadorBits++;

            // Cuando juntamos 8 bits (1 byte completo), lo mandamos al archivo
            if (contadorBits == 8)
            {
                archivo.put(byteActual);
                byteActual = 0;
                contadorBits = 0;
            }
        }
        // Si sobraron bits al final que no completaron el byte, los rellenamos y enviamos
        if (contadorBits > 0)
        {
            byteActual <<= (8 - contadorBits);
            archivo.put(byteActual);
        }

        archivo.close();
        cout << "\nGuardando archivo extensión .bin ." << endl;
    }
}

void codificarHuffman(string texto)
{
    // 1. Calcular la frecuencia de cada carácter
    unordered_map<char, int> frecuenciaMapa;
    for (char c : texto)
    {
        frecuenciaMapa[c]++;
    }

    // 2. Crear una cola de prioridad para almacenar los nodos del árbol
    priority_queue<MinNode *, vector<MinNode *>, Comparar> minHeap;

    for (auto par : frecuenciaMapa)
    {
        minHeap.push(new MinNode(par.first, par.second));
    }

    // 3. Construir el árbol de Huffman combinando los nodos con menor frecuencia
    while (minHeap.size() != 1)
    {
        MinNode *izq = minHeap.top();
        minHeap.pop();

        MinNode *der = minHeap.top();
        minHeap.pop();

        // El carácter '\0' indica un nodo interno (no es una hoja)
        MinNode *sumNode = new MinNode('\0', izq->frecuencia + der->frecuencia);
        sumNode->izquierda = izq;
        sumNode->derecha = der;

        minHeap.push(sumNode);
    }

    MinNode *raiz = minHeap.top();

    // 4. Generar la tabla de códigos
    unordered_map<char, string> codigosHuffman;
    generarCodigos(raiz, "", codigosHuffman);

    cout << "=== TABLA DE CODIGOS DE HUFFMAN ===" << endl;
    for (auto par : codigosHuffman)
    {
        cout << par.first << " : " << par.second << endl;
    }

    guardarTexto(texto, codigosHuffman, raiz);
    // Liberar memoria del árbol
    delete raiz;
}

void decodificarHuffman()
{
    ifstream archivo("Archivo_codificado.bin", ios::binary);
    if (!archivo.is_open())
    {
        cout << "No se pudo abrir el archivo" << endl;
        return;
    }

    MinNode *raizReconstruida = deserializarArbol(archivo);

    char saltoDeLinea;
    archivo.get(saltoDeLinea);
    int totalBits = 0;
    archivo.read(reinterpret_cast<char *>(&totalBits), sizeof(totalBits));

    // 3. DESEMPAQUETAR BYTES: Reconstruir la secuencia de bits leída del arc
    string textoCodificado = "";
    char byteLeido;
    int bitsLeidos = 0;

    while (bitsLeidos < totalBits && archivo.get(byteLeido))
    {
        for (int i = 7; i >= 0 && bitsLeidos < totalBits; i--)
        {
            // Extraer bit por bit de izquierda a derecha usando máscaras
            int bit = (byteLeido >> i) & 1;
            textoCodificado += (bit == 1) ? "1" : "0";
            bitsLeidos++;
        }
    }
    archivo.close();

    // 4. UTILIZAR EL ÁRBOL RECONSTRUIDO PARA DECODIFICAR
    string textoOriginal = "";
    MinNode *actual = raizReconstruida;

    for (char bit : textoCodificado)
    {
        if (bit == '0')
        {
            actual = actual->izquierda;
        }
        else
        {
            actual = actual->derecha;
        }

        if (!actual->izquierda && !actual->derecha)
        {
            textoOriginal += actual->caracter; // Guardamos la letra
            actual = raizReconstruida;         // ¡Regresamos a la raíz para la siguiente letra!
        }
    }

    // 5. Guardar el texto original ya limpio en el archivo decodificado
    ofstream archivoEscritura("Archivo_decodificado.txt");
    if (archivoEscritura.is_open())
    {
        archivoEscritura << textoOriginal;
        archivoEscritura.close();
        cout << "Archivo decodificado guardado exitosamente." << endl;
    }
    else
    {
        cout << "No se pudo crear el archivo decodificado." << endl;
    }

    // Liberar la memoria del árbol que reconstruimos
    delete raizReconstruida;
}

int main()
{
    int opcion;
    string texto;
    cout << "========================================" << endl;
    cout << "     Compresión HUFFMAN EN C++        " << endl;
    cout << "========================================" << endl;
    cout << "1. Comprimir un texto nuevo" << endl;
    cout << "2. Descomprimir 'Archivo_codificado.bin'" << endl;
    cout << "3. Salir" << endl;
    cout << "Selecciona una opcion (1-3): ";
    cin >> opcion;

    cin.ignore();

    if (opcion == 1)
    {
        cout << " \n Escribe el texto a comprimir" << endl;
        getline(cin, texto);

        if (texto.empty())
        {
            cout << "\nNo ingresaste ningun texto" << endl;
        }
        else
        {
            codificarHuffman(texto);
        }
    }
    else if (opcion == 2)
    {
        cout << "\nBuscando archivo..." << endl;
        decodificarHuffman();
    }
    else if (opcion == 3)
    {
        cout << "\nSaliendo." << endl;
    }
    else
    {
        cout << "\nOpcion no valida." << endl;
    }
    return 0;
}
