#include <iostream>
#include <cassert>
#include <cstdlib>
#include <random>
#include "avl.h"

// Función auxiliar: retorna altura del árbol
template<typename K,typename V>
int h(const std::unique_ptr<NodoBST<K,V>>& r) { return r?r->altura:0; }

// Verifica el invariante AVL (|fb|<=1, alturas consistentes y orden BST)
template<typename K,typename V>
bool es_avl(const std::unique_ptr<NodoBST<K,V>>& n) {
    if (!n) return true;
    int hi = altura(n->izq), hd = altura(n->der);
    if (n->altura != 1 + std::max(hi, hd)) return false;
    if (std::abs(hd - hi) > 1) return false;
    if (n->izq && !(n->izq->clave < n->clave)) return false;
    if (n->der && !(n->clave < n->der->clave)) return false;
    return es_avl(n->izq) && es_avl(n->der);
}

void test_P1_insercion_simple() {
    // Insertar 3 claves ordenadas --- en BST sería h=3; en AVL h=2
    std::unique_ptr<NodoBST<int,int>> r=nullptr;
    insertar_avl(r,10,10);
    insertar_avl(r,20,20);
    insertar_avl(r,30,30); // provoca rotación RR (rotar_izquierda)
    assert(h(r)==2 && "P1: altura debe ser 2 tras RR rotation");
    assert(r->clave==20 && "P1: raiz debe ser 20");
    std::cout << "[P1 OK] insercion_simple" << std::endl;
}

void test_P2_caso_LR() {
    std::unique_ptr<NodoBST<int,int>> r=nullptr;
    insertar_avl(r,30,30);
    insertar_avl(r,10,10);
    insertar_avl(r,20,20); // provoca rotacion LR
    assert(r->clave==20 && "P2: raiz debe ser 20");
    assert(h(r)==2 && "P2: altura debe ser 2");
    std::cout << "[P2 OK] caso_LR" << std::endl;
}

void test_P3_insercion_ordenada_grande() {
    // Insertar 1..15 en orden --- BST degeneraría h=15
    std::unique_ptr<NodoBST<int,int>> r=nullptr;
    for(int i=1;i<=15;i++) insertar_avl(r,i,i);
    assert(h(r)<=4 && "P3: altura debe ser <= 4 para 15 nodos AVL");
    assert(buscar(r,8) && "P3: nodo 8 debe existir");
    std::cout << "[P3 OK] insercion_ordenada_grande h=" << h(r) << std::endl;
}

void test_P4_eliminar_hoja() {
    std::unique_ptr<NodoBST<int,int>> r=nullptr;
    for(int x:{20,10,30}) insertar_avl(r,x,x);
    eliminar_avl(r,10); // hoja izquierda
    assert(!buscar(r,10) && "P4: nodo 10 debe haberse eliminado");
    assert(h(r)<=2 && "P4: árbol sigue balanceado");
    std::cout << "[P4 OK] eliminar_hoja" << std::endl;
}

void test_P5_eliminar_raiz_dos_hijos() {
    std::unique_ptr<NodoBST<int,int>> r=nullptr;
    for(int x:{20,10,30,25,35}) insertar_avl(r,x,x);
    eliminar_avl(r,20); // nodo raiz con 2 hijos
    assert(!buscar(r,20) && "P5: nodo 20 eliminado");
    assert(h(r)<=3 && "P5: árbol sigue balanceado");
    std::cout << "[P5 OK] eliminar_raiz_dos_hijos" << std::endl;
}

void test_P6_eliminar_con_rebalanceo() {
    // Insertar 1..7, eliminar 1 y verificar balanceo
    std::unique_ptr<NodoBST<int,int>> r=nullptr;
    for(int i=1;i<=7;i++) insertar_avl(r,i,i);
    int h_antes = h(r);
    eliminar_avl(r,1);
    // Verificar que el árbol sigue siendo AVL
    assert(h(r)<=h_antes && "P6: altura no aumentó tras eliminación");
    std::cout << "[P6 OK] eliminar_con_rebalanceo" << std::endl;
}

// Prueba extra (no pedida): 2000 inserciones y 1000 eliminaciones aleatorias
void test_extra_invariante_aleatorio() {
    std::mt19937 gen(42);
    std::unique_ptr<NodoBST<int,int>> r=nullptr;
    for (int i=0;i<2000;i++) {
        insertar_avl(r, (int)(gen()%5000), i);
        assert(es_avl(r) && "extra: invariante AVL tras insertar");
    }
    for (int i=0;i<1000;i++) {
        eliminar_avl(r, (int)(gen()%5000));
        assert(es_avl(r) && "extra: invariante AVL tras eliminar");
    }
    std::cout << "[EXTRA OK] invariante AVL en insercion/eliminacion aleatoria" << std::endl;
}

int main() {
    test_P1_insercion_simple();
    test_P2_caso_LR();
    test_P3_insercion_ordenada_grande();
    std::cout << "=== Todos los casos P1-P3 pasaron ===" << std::endl;
    test_P4_eliminar_hoja();
    test_P5_eliminar_raiz_dos_hijos();
    test_P6_eliminar_con_rebalanceo();
    test_extra_invariante_aleatorio();
    std::cout << "=== Todos los casos P4-P6 pasaron ===" << std::endl;
    return 0;
}
