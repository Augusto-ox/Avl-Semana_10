#pragma once
#include <memory>
#include <algorithm>
#include <string>
#include <utility>

template<typename K, typename V = K>
struct NodoBST {
    K clave; V valor;
    std::unique_ptr<NodoBST> izq, der;
    int altura;
    explicit NodoBST(K k, V v=V{})
        : clave(std::move(k)), valor(std::move(v)),
          izq(nullptr), der(nullptr), altura(1) {}
};

template<typename K,typename V>
int altura(const std::unique_ptr<NodoBST<K,V>>& n)
{ return n ? n->altura : 0; }

template<typename K,typename V>
int factor_balance(const std::unique_ptr<NodoBST<K,V>>& n)
{ return n ? altura(n->der)-altura(n->izq) : 0; }

template<typename K,typename V>
void actualizar_altura(std::unique_ptr<NodoBST<K,V>>& n)
{ if(n) n->altura=1+std::max(altura(n->izq),altura(n->der)); }

/* TODO 1: rotar_derecha(z)
         z                y
        / \              / \
       y   C    -->     A   z
      / \                  / \
     A   B                B   C
*/
template<typename K,typename V>
void rotar_derecha(std::unique_ptr<NodoBST<K,V>>& z) {
    std::unique_ptr<NodoBST<K,V>> y = std::move(z->izq);
    z->izq = std::move(y->der);      // B pasa a ser hijo izquierdo de z
    actualizar_altura(z);            // z queda abajo: actualizar primero
    y->der = std::move(z);           // z pasa a ser hijo derecho de y
    z = std::move(y);                // y es la nueva raíz del subárbol
    actualizar_altura(z);
}

/* TODO 2: rotar_izquierda(z)  (simétrica)
       z                    y
      / \                  / \
     A   y      -->       z   C
        / \              / \
       B   C            A   B
*/
template<typename K,typename V>
void rotar_izquierda(std::unique_ptr<NodoBST<K,V>>& z) {
    std::unique_ptr<NodoBST<K,V>> y = std::move(z->der);
    z->der = std::move(y->izq);
    actualizar_altura(z);
    y->izq = std::move(z);
    z = std::move(y);
    actualizar_altura(z);
}

// TODO 3: balancear(n) -- casos LL, RR, LR, RL
// fb = altura(der) - altura(izq)
template<typename K,typename V>
void balancear(std::unique_ptr<NodoBST<K,V>>& n) {
    actualizar_altura(n);
    int fb = factor_balance(n);
    if (fb < -1) {                               // cargado a la izquierda
        if (factor_balance(n->izq) > 0)          // caso LR
            rotar_izquierda(n->izq);
        rotar_derecha(n);                        // caso LL (o LR tras 1a rotacion)
    } else if (fb > 1) {                         // cargado a la derecha
        if (factor_balance(n->der) < 0)          // caso RL
            rotar_derecha(n->der);
        rotar_izquierda(n);                      // caso RR (o RL tras 1a rotacion)
    }
}

// TODO 4: insertar_avl
template<typename K,typename V>
void insertar_avl(std::unique_ptr<NodoBST<K,V>>& raiz,
                  K clave, V valor=V{}) {
    if (!raiz) {
        raiz = std::make_unique<NodoBST<K,V>>(std::move(clave), std::move(valor));
        return;
    }
    if (clave < raiz->clave)
        insertar_avl(raiz->izq, std::move(clave), std::move(valor));
    else if (raiz->clave < clave)
        insertar_avl(raiz->der, std::move(clave), std::move(valor));
    else {
        raiz->valor = std::move(valor);          // clave repetida: actualiza valor
        return;
    }
    balancear(raiz);
}

// YA IMPLEMENTADA --- no modificar
template<typename K,typename V>
NodoBST<K,V>* buscar(const std::unique_ptr<NodoBST<K,V>>& raiz,
                     const K& clave) {
    if (!raiz) return nullptr;
    if (clave==raiz->clave) return raiz.get();
    if (clave<raiz->clave) return buscar(raiz->izq,clave);
    return buscar(raiz->der,clave);
}

// ---------------- Actividad 2A: eliminacion ----------------
template<typename K,typename V>
NodoBST<K,V>* minimo(const std::unique_ptr<NodoBST<K,V>>& n) {
    return n->izq ? minimo(n->izq) : n.get();
}

template<typename K,typename V>
void eliminar_avl(std::unique_ptr<NodoBST<K,V>>& raiz,
                  const K& clave) {
    if (!raiz) return;
    if (clave < raiz->clave)
        eliminar_avl(raiz->izq, clave);
    else if (clave > raiz->clave)
        eliminar_avl(raiz->der, clave);
    else {
        if (!raiz->izq && !raiz->der) {
            raiz = nullptr;                                   // Caso 1: hoja
        } else if (!raiz->izq) {
            raiz = std::move(raiz->der);                      // Caso 2: solo hijo der
        } else if (!raiz->der) {
            raiz = std::move(raiz->izq);                      // Caso 2: solo hijo izq
        } else {                                              // Caso 3: dos hijos
            NodoBST<K,V>* suc = minimo(raiz->der);            // sucesor inorden
            K clave_suc = suc->clave;                         // copia (el nodo se destruira)
            raiz->clave = suc->clave;
            raiz->valor = suc->valor;
            eliminar_avl(raiz->der, clave_suc);
        }
    }
    if (raiz) balancear(raiz);                                // reequilibrar
}
