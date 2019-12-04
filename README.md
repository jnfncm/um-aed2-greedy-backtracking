# Avance rápido (voraz) y vuelta atrás (backtracking)

Algoritmos y Estructuras de Datos 2  
(Curso 2018 - 2019)

Grado en Ingeniería Informática  
Universidad de Murcia

Por: Sergio Ortuño Aguilar y Juan Francisco Carrión Molina

Profesor: Norberto Marín Pérez

## Memoria de las prácticas

### 1. Avance Rápido

#### 1.1. Pseudocódigo y diseno

En este apartado explicamos el diseno del algoritmo, analizamos sus partes, variables y funciones, y presentamos su pseudocódigo.

En primer lugar, utilizamos un tipo Solucion que nos permite almacenar una solución al problema propuesto mediante una tabla de enteros que representa el número de monedas de cada tipo usadas y el peso total ahorrado en el cambio.

```
tipo Solucion = registro
    monedas: array[1..N] de entero;
    peso: entero;
fin;
```

La única función externa al algoritmo principal y no implícita en este es Seleccionar(). Esta función devuelve el elemento que más nos convenga del conjunto de candidatos aún no seleccionados ni rechazados.

Dado que en el enunciado se especifica la necesidad de encontrar una solución con peso máximo y otra con peso mínimo, el criterio `argumentoMoneda` utilizado para seleccionar una moneda `i` será `pesos[i]/valores[i]` o `valores[i]/pesos[i]`, respectivamente.

```
funcion Seleccionar(N: entero; valores, pesos: array[1..N] de entero;
    elegidos: array[1..N] de booleano): entero;

variables locales
    vmax: flotante;
    i, j: entero;

hacer
    vmax := -1;
    j := 0;

    para i := 1 hasta N hacer
        si (argumentoMoneda > vmax) y (no elegidos[i]) entonces
            j := i;
            vmax = argumentoMoneda;
        finsi;
    finpara;

    Seleccionar := j;
fin;
```

En el algoritmo principal, `Voraz()`, encontramos el esquema general de los algoritmos voraces.

```
funcion Voraz(V, N: entero; valores, pesos: array[1..N] de entero;): Solucion;

variables locales
    S: array[1..N] de entero:
    elegidos: array[1..N] de booleano;
    x, act, pesoTotal: entero;
    sol: Solucion;

hacer
    inicializar(S, 0);
    inicializar(elegidos, falso);
    x := 0;
    act := 0;

    mientras act != V hacer
        x := Seleccionar(N, valores, pesos, elegidos);
        S[x] := (V - act) / valores[x];
        act := act + S[x] * valores[x];
        elegidos[x] := verdadero;
    finmientras;

    pesoTotal = 0;
    para i := 1 hasta N hacer
        pesoTotal := pesoTotal + S[i] * pesos[i];
    finpara;

    sol.monedas := S;
    sol.peso := pesoTotal;

    Voraz := sol;
fin;
```

#### 1.2. Programación e implementación

Programación del algoritmo (debe funcionar). El programa debe ir documentado, con explicación de qué es cada variable, qué realiza cada función y su correspondencia con las funciones básicas del esquema algorítmico correspondiente.

_Archivo `greedy.cpp`_

#### 1.3. Tiempo de ejecución

_En `Memoria.pdf`, ya que Markdown de GitHub no admite LaTeX._

### 2. Backtracking

#### 2.1. Pseudocódigo y diseño

Hemos decidido que la representación de la solución debe ser una tupla (x1, x2, ..., xno) tal que xi ∈ {0} ∪ {1, ..., nm}, es decir, que indique, para cada objeto, si no se ha metido en ninguna mochila o en qué mochila se ha metido, en su caso. Habiendo tomado esta decisión, utilizaremos para el algoritmo un árbol k-ario, en este caso (nm + 1)-ario, donde cada nivel representará un objeto y cada nodo no hoja tendrá (nm + 1) hijos.

La siguiente es una representación gráfica del árbol de soluciones del primer caso de prueba presentado en Mooshak. Como vemos, utilizamos algunas variables para adaptar el esquema de backtracking al deseado de optimización, en este caso, de maximización del beneficio. Las posiciones en rojo indican que se sobrepasa el peso de alguna mochila, en verde que el peso es correcto, en lila que se inserta un único objeto en alguna mochila (por lo que no hay beneficio óptimo y es podable) y en naranja que se actualiza el valor óptimo en ese momento.

_Gráfico no disponible_

Para facilitar la construcción del pseudocódigo y de la implementación, utilizamos nombres bastante descriptivos en funciones, procedimientos y variables. Además, asumimos que el número de objetos `no` y el número de mochilas `nm` son constantes globales.

En primer lugar, presentamos las variables generales utilizadas en el algoritmo:

- `no` Número de objetos
- `nm` Número de mochilas
- `datosCapacidades` Capacidades de cada mochila
- `datosPesos` Pesos de cada objeto
- `datosAfinidades` Afinidades de cada pareja de objetos
- `ordenPeso` Permite que se pruebe a insertar objetos en las mochilas en orden de mayor a menor peso
- `solucionParcial` Solución parcial al problema en un momento concreto del recorrido del árbol
- `solucionOptimaActual` Mejor solución que se ha encontrado en un momento concreto
- `beneficioActual` Beneficio que aporta la solución parcial al problema en un momento concreto
- `ocupacionesActuales` Ocupaciones de las mochilas en un momento concreto
- `beneficiosActuales` Beneficio que aporta cad mochila en un momento concreto
- `listasObjetos` Para cada mochila, almacena una tabla que indica, en primera posición, el número de objetos de cada mochila y, en el resto de posiciones, cuáles son esos objetos
- `listasBeneficios` Para cada mochila, almacena una tabla que indica el beneficio que aporta cada objeto dentro de la misma (la primera posición se omite para que coincida cada posición con la variable `listasObjetos`)

En segundo lugar, presentamos algunas funciones y procedimientos auxiliares. Como podemos ver, hemos optado por mantener almacenados los estados de cada mochila en lugar de calcular en cada movimiento pesos y beneficios, pudiendo, así, reducir sustancialmente el tiempo de ejecución. La función `sumaMochilas()` devuelve un entero que representa el beneficio actual en un momento del recorrido del árbol.

```
funcion sumaMochilas(beneficiosActuales: array[1..nm] de entero;): entero;

variables locales
    b, i: entero;

hacer
    b := 0;
    para i := 1 hasta nm hacer
        b := b + beneficiosActuales[i];
    finpara;
    devolver b;
fin;
```

El procedimiento `sacarObjeto()` actualiza el estado del recorrido para representar que se saca un objeto que previamente había sido introducido en una mochila. Para ello, retira el peso aportado por el objeto, actualiza el beneficio actual de la mochila afectada y actualiza su lista de objetos. Lo utilizan `Generar()` y `Retroceder()`.

```
procedimiento sacarObjeto(mochila: entero; datosPesos: array[1..no] de entero;
    ocupacionesActuales, beneficiosActuales: array[1..nm] de entero;
    listasObjetos, listasBeneficios: array[1..nm, 1..no + 1] de entero;);

hacer
    ocupacionesActuales[mochila]
        :-= datosPesos[listasObjetos[mochila, listasObjetos[mochila, 1] + 1]];
    beneficiosActuales[mochila]
        := listasBeneficios[mochila, listasObjetos[mochila, 1]];
    listasObjetos[mochila, 1] :-= 1;
fin;
```

El procedimiento `insertarObjeto()` actualiza el estado del recorrido para representar que se mete un objeto en una mochila. Para ello, anade el peso aportado por el objeto. Lo utiliza `Generar()`.

```
procedimiento insertarObjeto(mochila, objeto: entero;
    datosPesos: array[1..no] de entero;
    ocupacionesActuales, beneficiosActuales: array[1..nm] de entero;
    listasObjetos, listasBeneficios: array[1..nm, 1..no + 1] de entero;
    datosAfinidades: array[1..no, 1..no] de entero;);

variables locales
    i: entero;

hacer
    ocupacionesActuales[mochila] :+= datosPesos[objeto];
    listasObjetos[mochila, 1] :+= 1;
    listasObjetos[mochila, listasObjetos[mochila, 1] + 1] := objeto;

    para i := 2 hasta listasObjetos[mochila, 1] hacer
        beneficioActuales[mochila]
            :+= datosAfinidades[objeto][listasObjetos[mochila, i]];
    finpara;

    listasBeneficios[mochila, listasObjetos[mochila, 1] + 1]
        := beneficiosActuales[mochila];
fin;
```

A continuación, presentamos los métodos propios del esquema de backtracking. Se ha escogido un esquema de optimización según el criterio especificado por el enunciado del problema.

El procedimiento `Generar()` produce el siguiente el siguiente hermano de un nodo o, en caso de estar en un nuevo nivel, el primero. Como vemos, si se trata de un nuevo hermano de otro nodo, se saca un objeto, se avanza el objeto en la solución parcial y se inserta ese nuevo objeto. En caso de ser un nuevo nivel, no se saca ningún objeto.

```
procedimiento Generar(nivelActual: entero;
    solucionParcial, datosPesos: array[1..no] de entero;
    ocupacionesActuales, beneficiosActuales: array[1..nm] de entero;
    datosAfinidades: array[1..no, 1..no] de entero;
    listasObjetos, listasBeneficios: array[1..nm, 1..no + 1] de entero;);

hacer
    si solucionParcial[nivelActual] > 0 entonces
        sacarObjeto(solucionParcial[nivelActual], datosPesos, ocupacionesActuales,
            beneficiosActuales, listasObjetos, listasBeneficios);
    finsi;

    solucionParcial[nivelActual] := solucionParcial[nivelActual] + 1;

    si solucionParcial[nivelActual] > 0 entonces
        insertarObjeto(nivelActual, soluconParcial[nivelActual], datosPesos,
            ocupacionesActuales, beneficiosActuales, datosAfinidades, listasObjetos,
            listasBeneficios);
    finsi;
fin;
```

La función `Solucion()` comprueba si `(solucionParcial[1], ..., solucionParcial[nivelActual])` es una tupla solución válida para el problema. Para ello, comprueba que se está en un nodo hoja y que no se superan las capacidades de las mochilas.

```
funcion Solucion(nivelActual: entero; solucionParcial: array[1..no] de entero;
    ocupacionesActuales, datosCapacidades: array[1..nm] de entero;): booleano;
variables locales
    i: entero;

hacer
    si no <> nivelActual entonces
        devolver falso;
    finsi;

    para i := 1 hasta nm hacer
        si ocupacionesActuales[i] > datosCapacidades[i] entonces
            devolver falso;
        finsi;
    finpara;

    devolver verdadero;
fin;
```

La función `Criterio()` comprueba si se puede alcanzar una solución válida a partir de la tupla `(solucionParcial[1], ..., solucionParcial[nivelActual])`. En otro rechaza todos los descendientes. Se comprueba que no se está en un nodo hoja (_puedo seguir_) y que las capacidades de las mochilas no se superan (_voy bien_). Además, la poda se complementa con varios bucles que pretenden eliminar ramas de soluciones que no puedan obtener un beneficio mejor que el que ya se ha obtenido en algún momento anterior.

```
funcion Criterio(nivelActual, valorOptimoGuardado, beneficioActual: entero;
    solucionParcial, orden: array[1..no] de entero;
    ocupacionesActuales, datosCapacidades: array[1..nm] de entero;
    datosAfinidades: array[1..no, 1..no] de entero;
    listasObjetos: array[1..nm, 1..no + 1] de entero;): booleano;

variables locales
    seguir: booleano;
    i, j, k, b, mochila: entero;

hacer
    si nivelActual = no entonces
        devolver falso;
    finsi;

    para i := 1 hasta nm hacer
        si ocupacionesActuales[i] > datosCapacidades[i] entonces
            devolver falso;
        finsi;
    finpara;

    b := 0;
    para mochila := 1 hasta nm hacer
        b := beneficioActual;
        para i := 2 hasta listasObjetos[mochila, 1] hacer
            para j := nivelActual + 1 hasta no hacer
                b := b + datosAfinidades[listasObjetos[mochila, i]][orden[j]];
            finpara;
        finpara;

        para i := nivelActual + 1 hasta no hacer
            para j := nivelActual + 1 hasta no hacer
                b := b + datosAfinidades[orden[i], orden[j]];
            finpara;
        finpara;

        si b > valorOptimoGuardado entonces
            devolver verdadero;
        finsi;
    finpara;

    devolver falso;
fin;
```

La función `MasHermanos()` comprueba si el nodo actual tiene más hermanos que aún no han sido generados, es decir, si quedan mochilas en las que probar a meter el objeto actual.

```
funcion MasHermanos(nivelActual: entero;
    solucionParcial: array[1..no] de entero;): booleano;
hacer
    devolver solucionParcial[nivelActual] > nm;
fin;
```

El procedimiento `Retroceder()` se encarga de subir un nivel en el árbol de soluciones. Si se había introducido el objeto en alguna mochila, se deshace y se saca. En cualquier caso, se inicializa la posición correspondiente de la solución parcial actual y se disminuye el nivel.

```
procedimiento Retroceder(nivelActual: entero;
    solucionParcial, datosPesos, ordenPeso: array[1..no] de entero;
    ocupacionesActuales, beneficiosActuales: array[1..nm] de entero;
    datosAfinidades: array[1..no, 1..no] de entero;
    listasObjetos, listasBeneficios: array[1..nm, 1..no + 1] de entero;);

hacer
    si solucionParcial[ordenPeso[nivelActual]] > 0 entonces
        sacarObjeto(solucionParcial[ordenPeso[nivelActual]], datosPesos, ocupacionesActuales, beneficiosActuales, listasObjetos, listasBeneficios);
    finsi;

    solucionParcial[ordenPeso[nivelACtual]] := -1;
    nivelActual := nivelActual - 1;
fin;
```

Por último, la función `Backtracking()` constituye el esquema principal del algoritmo. En primer lugar, realiza una organización de los objetos según sus pesos, de mayor a menor. A continuación, modifica la representación de las afinidades entre objetos. Estas dos acciones contribuyen a la aceleración del tiempo de ejecución.

Seguidamente, se realiza una inicialización de todas las variables que se utilizarán durante la ejecución del algoritmo. Una vez hecho esto, llegamos al bucle principal. Este se ejecuta mientras no se haya llegado al nivel 0, que representa la salida del árbol. Dentro del bucle se sigue el esquema siguiente. Se llama a `Generar`. Si lo generado es solucion, se comprueba si mejora lo que teníamos almacenado y, en su caso, se actualiza esto. A continuación, si `Criterio` permite continuar, se avanza el nivel. En caso contrario, se retrocede mientras sea necesario (no queden más hermanos por visitar en el nivel actual) y posible (no hemos salido del árbol). El algoritmo no devuelve la tupla solución, sino el valor óptimo conseguido.

```
funcion Backtracking(datosCapacidades: array[1..nm] de entero;
    datosPesos: array[1..no] de entero;
    datosAfinidades: array[1..no, 1..no] de entero;): entero;

variables locales
    ordenPeso, copiaPesos, solucionParcial,
        solucionOptimaActual: array[1..no] de entero;
    maximo, ind, i, j, nivelActual, valorOptimoGuardado, beneficioActual: entero;
    afinidadesCalculo: array[1..no, 1..no] de entero;
    ocupacionesActuales, beneficiosActuales: array[1..nm] de entero;
    listasObjetos, listasBeneficios: array[1..nm, 1..no + 1] de entero;

hacer
    copiaPesos := datosPesos;

    para j := 1 hasta no hacer
        maximo := -1;
        para j := 1 hasta no hacer
            si copiaPesos[i] > maximo entonces
                maximo := copiaPesos[i];
                ind := i;
            finsi;
        finpara;
        copiaPesos[ind] := -1;
        ordenPeso[j] := ind;
    finpara;

    afinidadesCalculo := datosAfinidades;
    para i := 1 hasta no hacer
        para j := 1 hasta i hacer
            afinidadesCalculo[i, j] := datosAfinidades[i, j] + datosAfinidades[j, i];
            afinidadesCalculo[j, i] := afinidadesCalculo[i, j];
        finpara;
    finpara;
    datosAfinidades := afinidadesCalculo;

    nivelActual := 1;
    inicializar(solucionParcial, -1);
    valorOptimoGuardado := -1;
    inicializar(solucionOptimaActual, 0);
    inicializar(ocupacionesActuales, 0;
    beneficioActual := 0;
    inicializar(beneficiosActuales, 0);
    inicializar(listasObjetos, 0);
    inicializar(listasBeneficios, 0);

    mientras nivelActual <> 0 hacer
        Generar(ordenPeso[nivelActual], solucionParcial, ocupacionesActuales,
            datosPesos, datosAfinidades, beneficiosActuales, listasObjetos,
            listasBeneficios);
        beneficioActual := sumaMochilas(beneficiosActuales);

        si Solucion(nivelActual, solucionParcial,
            ocupacionesActuales, datosCapacidades) entonces
            si beneficioActual > valorOptimoGuardado entonces
                valorOptimoGuardado := beneficioACtual;
                solucionOptimaActual := solucionParcial;
            finsi;
        finsi;

        si Criterio(nivelActual, solucionParcial, ocupacionesActuales,
            datosCapacidades, valorOptimoGuardado, beneficioActual, ordenPeso,
            datosAfinidades, listasObjetos) entonces
            nivelActual := nivelActual + 1;
        si_no
            mientras (no MasHermanos(ordenPeso[nivelActual], solucionParcial,
                nMochilas)) y nivelACtual <> 0 hacer
                Retroceder(nivelActual, solucionParcial, ocupacionesActuales,
                datosPesos, datosAfinidades, beneficiosActuales, listasObjetos,
                ordenPeso, listasBeneficios);
            finmientras;
        finsi;
    finmientras;

    devolver valorOptimoGuardado;
fin;
```

#### 2.2. Programación e implementación

En la implementación del algoritmo se incluyen las funciones requeridas para la correcta presentación en Mooshak. Las funciones y variables generales coinciden en nombre y funcionalidad con las descritas en el apartado anterior.

Destacar que se han utilizado contenedores `vector` de C++ en lugar de arrays primitivos por su mayor facilidad de gestión y robustez. Además, hemos convenido en descartar las primeras posiciones (posiciones 0) de dichos contenedores para utilizar solo a partir del 1. Así, todos los contenedores tienen una posición más y el algoritmo es un poco más comprensible.

_Archivo `backtracking.cpp`_

#### 2.3. Tiempo de ejecución

_En `Memoria.pdf`, ya que Markdown de GitHub no admite LaTeX._

### 3. Conclusiones

La realización de esta práctica ha supuesto todo un reto para los dos componentes del equipo que la ha realizado. Algunas de las mayores dificultades han sido hallar un buen criterio de selección para el algoritmo de avance rápido y reducir el tiempo de ejecución del algoritmo de backtracking mediante podas. Así pues, superar estos retos nos ha permitido comprender en profundidad el funcionamiento de los algoritmos de avance rápido y backtracking.

En cuanto al tiempo empleado para completar la práctica, utilizamos todas las sesiones de laboratorio y bastantes más horas de trabajo adicionalmente. En total hemos dedicado 30 horas de trabajo aproximadamente.
