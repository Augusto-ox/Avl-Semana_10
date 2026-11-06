import random
import math
import sys
from dataclasses import dataclass
from time import perf_counter


class Nodo:
    def __init__(self, clave, valor=None):
        self.clave = clave
        self.valor = valor
        self.izq = self.der = None
        self.altura = 1


# ---------------- Funciones AVL ----------------
def height(n):
    return n.altura if n else 0


def fb(n):
    """Factor de balance = altura(der) - altura(izq)."""
    return height(n.der) - height(n.izq) if n else 0


def actualizar(n):
    n.altura = 1 + max(height(n.izq), height(n.der))


def rotar_der(z):
    y = z.izq
    z.izq = y.der
    y.der = z
    actualizar(z)
    actualizar(y)
    return y


def rotar_izq(z):
    y = z.der
    z.der = y.izq
    y.izq = z
    actualizar(z)
    actualizar(y)
    return y


def balancear(n):
    actualizar(n)
    f = fb(n)
    if f < -1:                       # cargado a la izquierda
        if fb(n.izq) > 0:            # caso LR
            n.izq = rotar_izq(n.izq)
        return rotar_der(n)          # caso LL
    if f > 1:                        # cargado a la derecha
        if fb(n.der) < 0:            # caso RL
            n.der = rotar_der(n.der)
        return rotar_izq(n)          # caso RR
    return n


def insertar_avl(raiz, clave, valor=None):
    if raiz is None:
        return Nodo(clave, valor)
    if clave < raiz.clave:
        raiz.izq = insertar_avl(raiz.izq, clave, valor)
    elif clave > raiz.clave:
        raiz.der = insertar_avl(raiz.der, clave, valor)
    else:
        raiz.valor = valor           # clave repetida: actualiza el valor
        return raiz
    return balancear(raiz)


def buscar(raiz, clave):
    while raiz is not None:
        if clave == raiz.clave:
            return raiz
        raiz = raiz.izq if clave < raiz.clave else raiz.der
    return None


# ---------------- BST simple ----------------
def insertar_bst(raiz, clave):  # BST simple sin balanceo
    if raiz is None:
        return Nodo(clave)
    if clave < raiz.clave:
        raiz.izq = insertar_bst(raiz.izq, clave)
    elif clave > raiz.clave:
        raiz.der = insertar_bst(raiz.der, clave)
    return raiz


def altura_arbol(n):
    if n is None:
        return 0
    return 1 + max(altura_arbol(n.izq), altura_arbol(n.der))


def experimento(n, orden='random'):
    claves = list(range(1, n + 1))
    if orden == 'random':
        random.shuffle(claves)
    # BST simple
    raiz_bst = None
    sys.setrecursionlimit(n + 100)
    for c in claves:
        raiz_bst = insertar_bst(raiz_bst, c)
    h_bst = altura_arbol(raiz_bst)
    # AVL
    raiz_avl = None
    for c in claves:
        raiz_avl = insertar_avl(raiz_avl, c)
    h_avl = raiz_avl.altura if raiz_avl else 0
    limite_teorico = int(1.44 * math.log2(n + 1))
    return h_bst, h_avl, limite_teorico


def tabla_2b():
    random.seed(2026)  # reproducible
    print(f"{'n':>8} | {'BST (ord)':>10} | {'AVL (ord)':>10} | {'Teorico':>10} | {'BST (rnd)':>10} | {'AVL (rnd)':>10}")
    print('-' * 68)
    for n in [100, 500, 1000, 5000, 10000]:
        h_bst_o, h_avl_o, teo = experimento(n, 'ordered')
        h_bst_r, h_avl_r, _ = experimento(n, 'random')
        print(f"{n:>8} | {h_bst_o:>10} | {h_avl_o:>10} | {teo:>10} | {h_bst_r:>10} | {h_avl_r:>10}")


# ---------------- Actividad 3: SIGA-UNAP-AVL ----------------
@dataclass
class EstudianteUNAP:
    codigo: str
    nombre: str
    escuela: str
    promedio: float


class SIGA_AVL:
    def __init__(self):
        self.raiz = None
        self.total = 0

    def matricular(self, est: EstudianteUNAP):
        t0 = perf_counter()
        existe = buscar(self.raiz, est.codigo) is not None
        self.raiz = insertar_avl(self.raiz, est.codigo, est)
        if not existe:
            self.total += 1
        t1 = perf_counter()
        accion = 'Actualizado' if existe else 'Matriculado'
        print(f'  {accion} {est.nombre} en {(t1 - t0) * 1e6:.2f} µs')

    def buscar_estudiante(self, codigo: str):
        t0 = perf_counter()
        nodo = buscar(self.raiz, codigo)
        t1 = perf_counter()
        if nodo:
            print(f'  Encontrado: {nodo.valor} ({(t1 - t0) * 1e6:.2f} µs)')
        else:
            print(f'  Código {codigo} no encontrado ({(t1 - t0) * 1e6:.2f} µs)')

    def listar_ordenado(self):
        # recorrido inorden (iterativo) => lista ordenada por código
        pila, actual, i = [], self.raiz, 0
        if actual is None:
            print('  (sin estudiantes)')
            return
        print(f"  {'#':>3} {'Código':<12}{'Nombre':<16}{'Escuela':<10}{'Prom.':>6}")
        while pila or actual:
            while actual:
                pila.append(actual)
                actual = actual.izq
            actual = pila.pop()
            e = actual.valor
            i += 1
            print(f'  {i:>3} {e.codigo:<12}{e.nombre:<16}{e.escuela:<10}{e.promedio:>6.1f}')
            actual = actual.der

    def estadisticas(self):
        h = self.raiz.altura if self.raiz else 0
        teo = int(1.44 * math.log2(self.total + 1)) if self.total > 0 else 0
        print(f'  Total estudiantes: {self.total}')
        print(f'  Altura AVL actual: {h}')
        print(f'  Altura teórica max: {teo}')
        print(f'  Eficiencia: {"ÓPTIMO" if h <= teo else "SUBÓPTIMO"}')


MUESTRA = [
    EstudianteUNAP('2026100001', 'Ana Quispe', 'Sistemas', 15.5),
    EstudianteUNAP('2026100002', 'Luis Mamani', 'Sistemas', 14.0),
    EstudianteUNAP('2026100003', 'Rosa Condori', 'IA&CD', 17.2),
    EstudianteUNAP('2026100004', 'Ivan Huanca', 'Sistemas', 13.8),
    EstudianteUNAP('2026100005', 'Maria Apaza', 'IA&CD', 16.9),
    EstudianteUNAP('2026100006', 'Carlos Tito', 'Sistemas', 12.1),
    EstudianteUNAP('2026100007', 'Lucia Ramos', 'IA&CD', 18.0),
    EstudianteUNAP('2026100008', 'Pedro Vargas', 'Sistemas', 15.0),
    EstudianteUNAP('2026100009', 'Sofia Luna', 'IA&CD', 16.4),
    EstudianteUNAP('2026100010', 'Juan Puma', 'Sistemas', 14.7),
]


def menu():
    siga = SIGA_AVL()
    print('Cargando 10 estudiantes...')
    for e in MUESTRA:
        siga.matricular(e)

    while True:
        print('\n=== SIGA-UNAP-AVL ===')
        print('1. Matricular nuevo estudiante')
        print('2. Buscar por código')
        print('3. Listar todos (inorden)')
        print('4. Estadísticas del árbol')
        print('5. Salir')
        try:
            op = input('Opción: ').strip()
        except EOFError:
            break
        if op == '1':
            cod = input('  Código: ').strip()
            nom = input('  Nombre: ').strip()
            esc = input('  Escuela: ').strip()
            try:
                prom = float(input('  Promedio: '))
            except ValueError:
                print('  Promedio inválido, operación cancelada.')
                continue
            siga.matricular(EstudianteUNAP(cod, nom, esc, prom))
        elif op == '2':
            siga.buscar_estudiante(input('  Código: ').strip())
        elif op == '3':
            siga.listar_ordenado()
        elif op == '4':
            siga.estadisticas()
        elif op == '5':
            break
        else:
            print('  Opción no válida.')


if __name__ == '__main__':
    if len(sys.argv) > 1 and sys.argv[1] == '--tabla':
        tabla_2b()          # solo la tabla de la Actividad 2B
    else:
        tabla_2b()
        menu()
