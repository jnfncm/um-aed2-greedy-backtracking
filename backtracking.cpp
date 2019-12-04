#include <iostream>
#include <vector>
using namespace std;

int sumaMochilas(vector<int> & beneficios){
	int b = 0;
	for (int i=1; i < (int) beneficios.size(); i++){
		b += beneficios[i];
	}
	return b;
}

void sacarObjeto(int mochila, vector<int> & datosPesos, vector<int> & ocupaciones, vector<int> & beneficiosActuales, vector< vector<int> > & listasObjetos, vector< vector<int> > & listasBeneficios){
	
	ocupaciones[mochila] -= datosPesos[listasObjetos[mochila][listasObjetos[mochila][1]+1]];
	beneficiosActuales[mochila] = listasBeneficios[mochila][listasObjetos[mochila][1]];
	listasObjetos[mochila][1]--;
}

void insertarObjeto(int objeto, int mochila, vector<int> & datosPesos, vector<int> & ocupaciones, vector<int> & beneficiosActuales, vector< vector<int> > & datosAfinidades, vector< vector<int> > & listasObjetos, vector< vector<int> > & listasBeneficios){
	
	ocupaciones[mochila] += datosPesos[objeto];
	listasObjetos[mochila][1]++;
	listasObjetos[mochila][listasObjetos[mochila][1]+1] = objeto;

	for (int i=2; i <= listasObjetos[mochila][1]; i++){
		beneficiosActuales[mochila] += datosAfinidades[objeto][listasObjetos[mochila][i]];
	}

	listasBeneficios[mochila][listasObjetos[mochila][1]+1] = beneficiosActuales[mochila];
}

void Generar(int nivelActual, vector<int> & solucionParcial, vector<int> & ocupacionesActuales, vector<int> & datosPesos, vector< vector<int> > & datosAfinidades,
			vector<int> & beneficiosActuales, vector< vector<int> > & listasObjetos, vector< vector<int> > & listasBeneficios) {

	if (solucionParcial[nivelActual] > 0) {
		sacarObjeto(solucionParcial[nivelActual], datosPesos, ocupacionesActuales, beneficiosActuales, listasObjetos, listasBeneficios);
	}

	solucionParcial[nivelActual]++;

	if (solucionParcial[nivelActual] > 0) {
		insertarObjeto(nivelActual, solucionParcial[nivelActual], datosPesos, ocupacionesActuales, beneficiosActuales, datosAfinidades, listasObjetos, listasBeneficios);
	}

}

bool Criterio(int nivelActual, int nObjetos, int nMochilas, vector<int> & solucionParcial, vector<int> & ocupacionesActuales, vector<int> & datosCapacidades,
			  int valorOptimoGuardado, int beneficioActual, vector<int> & orden, vector< vector<int> > & datosAfinidades, vector< vector<int> > & listasObjetos) {

	if (nivelActual == nObjetos) {
		return false;
	}

	for (int i = 1; i <= nMochilas; i++) {
		if (ocupacionesActuales[i] > datosCapacidades[i]) { 
			return false;
		}
	}

	int b = 0;

	for (int mochila = 1; mochila <= nMochilas; mochila++){
		b = beneficioActual;
		for (int i=2; i <= listasObjetos[mochila][1]+1; i++) {
			for (int j = nivelActual+1; j <= nObjetos; j++){
				b += datosAfinidades[listasObjetos[mochila][i]][orden[j]];	
			}
		}

		for (int i = nivelActual+1; i <= nObjetos; i++){
			for (int j = nivelActual+1; j <= nObjetos; j++){
				b += datosAfinidades[orden[i]][orden[j]];
			}
		}

		if (b > valorOptimoGuardado){
				return true;
		}
	}

	return false;
}

bool Solucion(int nivelActual, int nObjetos, int nMochilas, vector<int> & solucionParcial, vector<int> & ocupacionesActuales, vector<int> & datosCapacidades) {

	if (nObjetos != nivelActual) {
		return false;
	}

	for (int i = 1; i <= nMochilas; i++) {
		if (ocupacionesActuales[i] > datosCapacidades[i]) {
			return false;
		}
	}

	return true;
}

bool MasHermanos(int nivelActual, vector<int> & solucionParcial, int nMochilas) {
	return solucionParcial[nivelActual] < nMochilas;
}

void Retroceder(int & nivelActual, vector<int> & solucionParcial, vector<int> & ocupacionesActuales, vector<int> & datosPesos, vector< vector<int> > & datosAfinidades,
				vector<int> & beneficiosActuales, vector< vector<int> > & listasObjetos, vector<int> & ordenPeso, vector< vector<int> > & listasBeneficios) {
	
	if (solucionParcial[ordenPeso[nivelActual]] > 0) {
		sacarObjeto(solucionParcial[ordenPeso[nivelActual]], datosPesos, ocupacionesActuales, beneficiosActuales, listasObjetos, listasBeneficios);
	}

	solucionParcial[ordenPeso[nivelActual]] = -1;
	nivelActual--;
}

int Backtracking(vector<int> & datosCapacidades, vector<int> & datosPesos, vector< vector<int> > & datosAfinidades) {

    int nObjetos = datosPesos.size() - 1;
    int nMochilas = datosCapacidades.size() - 1;

	vector<int> ordenPeso(nObjetos+1);
	vector<int> copiaPesos = datosPesos;
	int maximo = -1;
	int ind = 0;
	for (int j=1; j<=nObjetos; j++){
		maximo = -1;
		for (int i=1; i<=nObjetos; i++){
			if (copiaPesos[i] > maximo) {
				maximo = copiaPesos[i];
				ind = i;
			}
		}
		copiaPesos[ind] = -1;
		ordenPeso[j] = ind;
	}

	vector< vector<int> > afinidades = datosAfinidades;
	for (int i=1; i<=nObjetos; i++){
		for (int j=1; j<i; j++){
			afinidades[i][j] = datosAfinidades[i][j] + datosAfinidades[j][i];
			afinidades[j][i] = afinidades[i][j];
		}
	}
	datosAfinidades = afinidades;

	int nivelActual = 1;

	vector<int> solucionParcial(nObjetos + 1, -1);

	int valorOptimoGuardado = -1;

	vector<int> solucionOptimaActual(nObjetos + 1, 0);

    vector<int> ocupacionesActuales(nMochilas + 1, 0);

	int beneficioActual = 0;

	vector<int> beneficiosActuales(nMochilas + 1, 0);

	vector< vector<int> > listasObjetos(nMochilas + 1, vector<int>(nObjetos+2, 0));

	vector< vector<int> > listasBeneficios(nMochilas + 1, vector<int>(nObjetos+2, 0));

	while (nivelActual != 0) {
		Generar(ordenPeso[nivelActual], solucionParcial, ocupacionesActuales, datosPesos, datosAfinidades, beneficiosActuales, listasObjetos, listasBeneficios);
		beneficioActual = sumaMochilas(beneficiosActuales);

		if (Solucion(nivelActual, nObjetos, nMochilas, solucionParcial, ocupacionesActuales, datosCapacidades)) {
			
			if (beneficioActual > valorOptimoGuardado){
				valorOptimoGuardado = beneficioActual;
				solucionOptimaActual = solucionParcial;
			}    
		}
		if (Criterio(nivelActual, nObjetos, nMochilas, solucionParcial, ocupacionesActuales, datosCapacidades, valorOptimoGuardado, beneficioActual, ordenPeso, datosAfinidades, listasObjetos)) {
			nivelActual++;
		}
		else {
			while ((! MasHermanos(ordenPeso[nivelActual], solucionParcial, nMochilas)) && (nivelActual != 0)) {
				Retroceder(nivelActual, solucionParcial, ocupacionesActuales, datosPesos, datosAfinidades, beneficiosActuales, listasObjetos, ordenPeso, listasBeneficios);
			}
		}
	}

	return valorOptimoGuardado;
}

vector<int> leerCapacidades(int nm) {
    vector<int> capacidad(nm + 1);
    for (int i = 1; i < (nm + 1); i++){
        cin >> capacidad[i];
    }
    return capacidad;
}

vector<int> leerPesos(int no) {
	vector<int> pesos(no + 1);
	for (int i = 1; i < (no + 1); i++) {
		cin >> pesos[i];
	}
	return pesos;
}

vector< vector<int> > leerAfinidades(int no){
    vector< vector<int> > afinidades(no + 1, vector<int>(no + 1));
    for (int i = 1; i < (no + 1); i++) {
        for (int j = 1; j < (no + 1); j++) {
            if (i == j) {
                afinidades[i][j] = 0;
            } else {
                cin >> afinidades[i][j];
            }  
        }
    }
    return afinidades;
}

int main() {
    int T, nMochilas, nObjetos, solucion;
    vector<int> capacidades;
    vector<int> pesos;
    vector< vector<int> > afinidades;

    cin >> T;

    for (int i = 0; i < T; i++) {
        cin >> nMochilas;
        cin >> nObjetos;
        capacidades = leerCapacidades(nMochilas);
        pesos = leerPesos(nObjetos);
        afinidades = leerAfinidades(nObjetos);
        solucion = Backtracking(capacidades, pesos, afinidades);
        cout << solucion << endl;
    } 
}