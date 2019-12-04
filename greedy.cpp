#include <iostream>
#define MINIMO_PESO 1
#define MAXIMO_PESO 2

using namespace std;

class Solucion {
public:
    int * monedas;
    int peso;

    void print(int N) {
        for (int i = 0; i < N; i++) {
            cout << monedas[i] << " ";
        }
        cout << endl;
    }
};

int Seleccionar(int * valores, int * pesos, int N, bool * elegidos, int version) {
    float vmax = -1;
    int i;
    int j = 0;
    if (version == MAXIMO_PESO) {
        for (i = 0; i < N; i++) {
            if ((float) pesos[i] / (float) valores[i] > vmax && ! elegidos[i]) {
                j = i;
                vmax = (float) pesos[i] / (float) valores[i];
            }
        }
    }
    else if (version == MINIMO_PESO) {
        for (i = 0; i < N; i++) {
            if ((float) valores[i] / (float) pesos[i] > vmax && ! elegidos[i]) {
                j = i;
                vmax = (float) valores[i] / (float) pesos[i];
            }
        }
    }
    return j;
}

Solucion Voraz(int * valores, int * pesos, int V, int N, int version) {
    int * S = new int [N] ();

    bool * elegidos = new bool [N] ();
    int x, act = 0;

    while (act != V) {
        x = Seleccionar(valores, pesos, N, elegidos, version);
        S[x] = (V - act) / valores[x];
        act += (S[x] * valores[x]);
        elegidos[x] = true;
    }

    delete [] elegidos;

    int pesoTotal = 0;
    for (int i = 0; i < N; i++) {
        pesoTotal += S[i] * pesos[i];
    }

    Solucion sol;
    sol.monedas = S;
    sol.peso = pesoTotal;

    return sol;
}

int * leerValores(int N) {
    int * valores = new int [N];
    for (int i = 0; i < N; i++) {
        cin >> valores[i];
    }
    return valores;
}

int * leerPesos(int N) {
    int * pesos = new int[300];
    for (int i = 0; i < N; i++) {
        cin >> pesos[i];
    }
    return pesos;
}

int main() {
    int T, N, V;
    cin >> T;

    for (int i = 0; i < T; i++) {
        cin >> N;
        cin >> V;
        int * datosValores = leerValores(N);
        int * datosPesos = leerPesos(N);
        Solucion SolucionMaximoPeso = Voraz(datosValores, datosPesos, V, N, MAXIMO_PESO);
        Solucion SolucionMinimoPeso = Voraz(datosValores, datosPesos, V, N, MINIMO_PESO);

        cout << SolucionMaximoPeso.peso - SolucionMinimoPeso.peso << endl;

        SolucionMaximoPeso.print(N);
        SolucionMinimoPeso.print(N);

        delete [] datosValores;
        delete [] datosPesos;
        delete [] SolucionMaximoPeso.monedas;
        delete [] SolucionMinimoPeso.monedas;
    }
}