PRÁCTICA S10 - Árboles AVL | SIS210 Algoritmos y Estructuras de Datos | UNAP 2026-II
Alumno: ____________________   Código: ____________

CONTENIDO
  avl.h          AVL en C++17 (rotaciones, balancear, insertar_avl, eliminar_avl)
  avl_main.cpp   Casos de prueba P1-P6 (+ prueba extra de invariante AVL)
  avl_python.py  Tabla BST vs AVL (2B) y menú SIGA-UNAP-AVL (Actividad 3)

COMPILAR Y EJECUTAR
  C++ :   g++ -std=c++17 -Wall -Wextra -o avl_main avl_main.cpp && ./avl_main
  Python: python3 avl_python.py          (tabla 2B + menú interactivo)
          python3 avl_python.py --tabla  (solo la tabla 2B)

RESPUESTAS - ACTIVIDAD 1
P1-A) Insertar 10, 20, 30: ocurre 1 rotación, de tipo RR (simple a la izquierda,
      rotar_izquierda sobre el 10). La raíz pasa a ser 20 y la altura es 2.
      (El comentario del enunciado dice "LL", pero 10-20-30 crece hacia la
      derecha, por eso el caso correcto es RR.)
P1-B) Insertar 30, 10, 20: ocurren 2 rotaciones, de tipo LR (doble):
      rotar_izquierda(10) y luego rotar_derecha(30). La raíz pasa a ser 20.
P1-C) Con 15 nodos el AVL puede medir como mínimo 4 y como máximo 5 niveles:
      el mínimo de nodos para altura h cumple N(h)=N(h-1)+N(h-2)+1 con
      N = 1, 2, 4, 7, 12, 20..., y 12 <= 15 < 20, luego h_max = 5.
      P3 acepta h <= 4 porque insertar 1..15 en orden produce un árbol perfecto
      (2^4 - 1 = 15), que es la altura mínima posible; es un caso favorable.
      Para un AVL cualquiera de 15 nodos, la cota general sería h <= 5.

RESPUESTAS - ACTIVIDAD 2B (resultados reales de la ejecución, semilla 2026)
         n | BST(ord) | AVL(ord) | Teórico | BST(rnd) | AVL(rnd)
       100 |      100 |        7 |       9 |       12 |        8
       500 |      500 |        9 |      12 |       18 |       11
      1000 |     1000 |       10 |      14 |       20 |       12
      5000 |     5000 |       13 |      17 |       27 |       15
     10000 |    10000 |       14 |      19 |       29 |       16
P2B-A) La altura del BST con inserción ordenada es igual a n: el árbol se
      degenera en una lista enlazada (h = n, búsqueda O(n)).
P2B-B) No la supera. 1.44*log2(10001) = 19.13 -> límite 19; el AVL mide 14
      (ordenado) y 16 (aleatorio), es decir 5 y 3 niveles por debajo.
P2B-C) La premisa no es exacta: las alturas no son idénticas (14 vs 16 con
      n=10000), pero ambas son O(log n) y quedan muy cerca. El AVL mantiene
      |fb| <= 1 en todos los nodos con rotaciones, sin importar el orden de
      llegada. La inserción ordenada da un árbol casi perfecto (h = ceil(log2(n+1)));
      la aleatoria es un poco más alta porque el AVL tolera desbalances de 1.

ACTIVIDAD 3 - SIGA-UNAP-AVL
  Menú: 1 matricular, 2 buscar por código (muestra µs), 3 listar inorden
  (ordenado por código), 4 estadísticas (altura actual vs teórica), 5 salir.
  Si se matricula un código repetido, se actualizan los datos (no duplica).
