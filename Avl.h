typedef float Dado;
typedef unsigned Chave;

//=====================================================[NohAVL]===============================================

class NohAVL {
    friend int calcBal (NohAVL* umNoh);
    friend int calAlt (NohAVL* umNoh);
    friend class Avl;
    public :
        int fatorDeBal();
        NohAVL (Chave c = 0, Dado d = 0);
        ~NohAVL ();
    protected :
        Chave mChave;
        Dado mConteudo;
        NohAVL* mEsq;
        NohAVL* mDir;
        NohAVL* mPai;
        int mAltura;
};

NohAVL::NohAVL  (Chave c, Dado d) {
    mChave = c;
    mConteudo = d;
    mEsq = NULL;
    mDir = NULL;
    mPai = NULL;
    mAltura = 1;
}

NohAVL::~NohAVL () {
    delete mEsq;
    delete mDir;
}

//@return : int $ fator de balaceamento de um Nó.
int NohAVL::fatorDeBal () {
    return calAlt(mEsq) - calAlt(mDir);
}

//@return : int $ calculo de balanceamento de um Nó.
int calcBal (NohAVL* umNohAVL) {
    if (not umNohAVL)
        return 0;
    return umNohAVL->fatorDeBal();
}

//@return : int $ cálculo da altura de um ´nó.
int calAlt (NohAVL* umNohAVL) {
    if (not umNohAVL)
        return 0;
    return umNohAVL->mAltura;
}

// ====================================================[AVL]================================================

class Avl {

    public:
        Avl ();
        ~Avl ();
       
        void insere (Chave chave, Dado valor);
        void remover (Dado valor);

        Dado minimoArv ();
        Dado maximoArv ();

        void percorreEmOrdem ();
        void destroiArvore();
    protected :
        NohAVL* mRaiz;
        
        NohAVL* busca (Dado valor);
        NohAVL* inserAux (NohAVL* umNohAVL, Chave chave, Dado valor);
        NohAVL* removerAux (NohAVL* umNohAVL, Dado valor);

        NohAVL* minimoAux (NohAVL* aux);
        NohAVL* maximoAux (NohAVL* aux);

        void substituiNohAVL (NohAVL* antigo, NohAVL* novo);

        void percorreEmOrdemAux (NohAVL* umNohAVL, unsigned nivel);

        // Balanciamento:
        NohAVL* rotacaoEsq (NohAVL* umNohAVL);
        NohAVL* rotacaoDir (NohAVL* umNohAVL);
        NohAVL* arrumaBalanc (NohAVL* umNohAVL);

};

Avl::Avl () { mRaiz = NULL; }

Avl::~Avl () { delete mRaiz; }

//Finalidade: reinicializa a árvore para evitar estouro de memória.
void Avl::destroiArvore(){ 
    delete mRaiz;
    mRaiz = NULL;
}

//Finalidade : A inserção está de acordo com o valor,
//enquanto a chave indica a ordem em que foi inserida.
void Avl:: insere (Chave chave, Dado valor) {
    mRaiz = inserAux(mRaiz, chave, valor);
}

NohAVL* Avl::inserAux (NohAVL* umNohAVL, Chave chave, Dado valor) {

    if (not umNohAVL)
        return new NohAVL(chave, valor);

    if (umNohAVL->mConteudo > valor) {
        umNohAVL->mEsq = inserAux(umNohAVL->mEsq, chave, valor);
        umNohAVL->mEsq->mPai = umNohAVL;
    
    } else {
        umNohAVL->mDir = inserAux(umNohAVL->mDir, chave, valor);
        umNohAVL->mDir->mPai = umNohAVL;    
    }

    return arrumaBalanc(umNohAVL);
}

void Avl::remover (Dado valor) {
    if (not mRaiz) {
        Serial.println("ERRO NA REMOCAO, CASO: ARVORE VAZIA");
        return;
    }mRaiz = removerAux(mRaiz, valor);
}

NohAVL* Avl::removerAux (NohAVL* umNohAVL, Dado valor) {

    if (not umNohAVL) {
        Serial.println("ERRO NA REMOCAO, CASO: ELEMENTO NAO ENCONTRADO");
    } else {
      if (umNohAVL->mConteudo > valor)
        umNohAVL->mEsq  = removerAux(umNohAVL->mEsq, valor);
  
      else if (umNohAVL->mConteudo < valor)
          umNohAVL->mDir = removerAux(umNohAVL->mDir, valor);
  
      else {
          NohAVL* aux;
  
          if (not umNohAVL->mEsq) {
              aux = umNohAVL->mDir;
              substituiNohAVL(umNohAVL, umNohAVL->mDir);
          
          } else if (not umNohAVL->mDir) {
              aux = umNohAVL->mEsq;
              substituiNohAVL(umNohAVL, umNohAVL->mEsq);
          
          } else {
              NohAVL* sucessor = minimoAux(umNohAVL->mDir);
              aux = sucessor;
  
              if (sucessor->mPai != umNohAVL) {
                  substituiNohAVL(sucessor, sucessor->mDir);
  
                  NohAVL* auxSuc = sucessor->mPai;
                  while (umNohAVL->mDir != auxSuc) {
                      auxSuc->mAltura = 1 + max(calAlt(auxSuc->mEsq), calAlt(auxSuc->mDir));
                      auxSuc = auxSuc->mPai;
                  }
  
                  sucessor->mDir = arrumaBalanc(umNohAVL->mDir);
                  sucessor->mDir->mPai = sucessor;
              }
  
              substituiNohAVL(umNohAVL, sucessor);
              sucessor->mEsq = umNohAVL->mEsq;
              sucessor->mEsq->mPai = sucessor;
          }
          
  
  
          umNohAVL->mEsq = NULL;
          umNohAVL->mDir = NULL;
          umNohAVL->mPai = NULL;
          delete umNohAVL;
  
          umNohAVL = aux;
      }
  
      if (not umNohAVL)
          return umNohAVL;
  
      return arrumaBalanc(umNohAVL);
    }
}

NohAVL* Avl::busca (Dado valor) {
    if (not mRaiz)
        return NULL;

    NohAVL* aux = mRaiz;
    while (aux) {

        if (aux->mConteudo == valor)
            return aux;

        if (aux->mConteudo > valor)
            aux = aux->mEsq;
        else
            aux = aux->mDir;
    }
    return NULL;
}

//return: Dado{float} $ retorna o menor valor da árvore.
Dado Avl::minimoArv () {
    if (not mRaiz) {
        Serial.println("ERRO NO MINIMO, CASO: ARVORE VAZIA");
        return -1;
    }
    return minimoAux(mRaiz)->mConteudo;
}

NohAVL* Avl::minimoAux (NohAVL* aux) {
    if (aux->mEsq)
        return minimoAux(aux->mEsq);
    return aux;
}

//return: Dado{float} $ retorna o maior valor da árvore.
Dado Avl::maximoArv () {
    if (not mRaiz) {
        Serial.println("ERRO NO MAXIMO, CASO: ARVORE VAZIA");
        return -1;
    }
    return maximoAux(mRaiz)->mConteudo;
}

NohAVL* Avl::maximoAux (NohAVL* umNohAVL) {
    while (umNohAVL->mDir)
        umNohAVL = umNohAVL->mDir;
    return umNohAVL;
}

void Avl::substituiNohAVL (NohAVL* antigo, NohAVL* novo) {

    if (mRaiz == antigo)
        mRaiz = novo;

    else if (antigo->mPai->mEsq == antigo)
        antigo->mPai->mEsq = novo;
    else
        antigo->mPai->mDir = novo;

    if (novo)
        novo->mPai = antigo->mPai;
}

void Avl::percorreEmOrdem () {
    percorreEmOrdemAux(mRaiz, 0);
    Serial.println();
}

void Avl::percorreEmOrdemAux (NohAVL* umNohAVL, unsigned nivel) {
    if (umNohAVL) {
        percorreEmOrdemAux(umNohAVL->mEsq, nivel+1);
        
        Serial.print("[");
        Serial.print(umNohAVL->mChave);
        Serial.print(" || ");
        Serial.print(umNohAVL->mConteudo);
        Serial.print("]");
        Serial.print("->");
        Serial.println(nivel);
          
        percorreEmOrdemAux(umNohAVL->mDir, nivel+1);
    }
}

// Métodos de balanciamento:

NohAVL* Avl::rotacaoEsq (NohAVL* umNohAVL) {
    NohAVL* auxRotEsq = umNohAVL->mDir;
    umNohAVL->mDir = auxRotEsq->mEsq;

    if (auxRotEsq->mEsq)
        auxRotEsq->mEsq->mPai = umNohAVL;

    auxRotEsq->mPai = umNohAVL->mPai;

    if (umNohAVL->mPai) {

        if (umNohAVL->mPai->mEsq == umNohAVL)
            umNohAVL->mPai->mEsq = auxRotEsq;
        else
            umNohAVL->mPai->mDir = auxRotEsq;
    }

    auxRotEsq->mEsq = umNohAVL;
    umNohAVL->mPai = auxRotEsq;

    umNohAVL->mAltura = 1 + max(calAlt(umNohAVL->mEsq), calAlt(umNohAVL->mDir));
    auxRotEsq->mAltura = 1 + max(calAlt(auxRotEsq->mEsq), calAlt(auxRotEsq->mDir));

    return auxRotEsq;
}

NohAVL* Avl::rotacaoDir (NohAVL* umNohAVL) {

    NohAVL* auxRotDir = umNohAVL->mEsq;
    umNohAVL->mEsq = auxRotDir->mDir;

    if (auxRotDir->mDir)
        auxRotDir->mDir->mPai = umNohAVL;

    auxRotDir->mPai = umNohAVL->mPai;

    if (umNohAVL->mPai) {

        if (umNohAVL->mPai->mEsq == umNohAVL)
            umNohAVL->mPai->mEsq = auxRotDir;
        else
            umNohAVL->mPai->mDir = auxRotDir;
    }

    auxRotDir->mDir = umNohAVL;
    umNohAVL->mPai = auxRotDir;

    umNohAVL->mAltura = 1 + max(calAlt(umNohAVL->mEsq), calAlt(umNohAVL->mDir));
    auxRotDir->mAltura = 1 + max(calAlt(auxRotDir->mEsq), calAlt(auxRotDir->mDir));

    return auxRotDir;
}

NohAVL* Avl::arrumaBalanc (NohAVL* umNohAVL) {

    umNohAVL->mAltura = 1 + max(calAlt(umNohAVL->mEsq), calAlt(umNohAVL->mDir));
    int bal = umNohAVL->fatorDeBal();

    if ((bal <= 1) and (bal >= -1))
        return umNohAVL;

    if ((bal > 1) and (calcBal(umNohAVL->mEsq) >= 0))
        return rotacaoDir(umNohAVL);

    if ((bal > 1) and (calcBal(umNohAVL->mEsq) < 0)) {
        umNohAVL->mEsq = rotacaoEsq(umNohAVL->mEsq);
        return rotacaoDir(umNohAVL);
    }

    if ( (bal < -1) and (calcBal(umNohAVL->mDir) <= 0) )
        return rotacaoEsq(umNohAVL);

    if ( (bal < -1) and (calcBal(umNohAVL->mDir) > 0) ) {
        umNohAVL->mDir = rotacaoDir(umNohAVL->mDir);
        return rotacaoEsq(umNohAVL);
    }

    Serial.println("ERRO ARRUMA BALANCEAMENTO, CASO : NENHUMA CONDICAO ACEITA");
    return NULL;    
}
