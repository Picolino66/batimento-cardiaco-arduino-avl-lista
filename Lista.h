/* 
  --Trabalho de Estrutura de Dados
  --Sistema de coleta de batimentos cadiácos.
  --Copyright 2018 by Andrew, Isaias e Débora
  --Arquivo contém a implementação da classes Lista;
*/

typedef float Dados;

//=========[Classe Noh]============================================================================

class Noh {
    friend class Lista;
    public :
        Noh (unsigned ordem = 0 ,Dados valor = 0) : mOrdem(ordem), mConteudo(valor), mProximo(NULL) {};
    private :
        unsigned mOrdem;
        Dados mConteudo;
        Noh* mProximo;
};


//==========[Classe Lista]=========================================================================

class Lista {
    public :
        Lista () : mPrimeiro(NULL), mUltimo(NULL), mTamanho(0) {};

        ~Lista () {
             Noh* aux = mPrimeiro;
             Noh* ant = NULL;
             while (aux) {
                ant = aux;
                aux = aux->mProximo;
                delete ant;
             }
             mPrimeiro = NULL;
             mUltimo = NULL;
             mTamanho = 0;
         };


        //Finalidade: Inserir na Lista com Nós recebendo
        // a ordem em que foi inserido o valor e o dado da leitura.
        void insere (unsigned ordem, Dados valor) {
            Noh* novo = new Noh(ordem ,valor);
            
            if (mPrimeiro == NULL) {
                mPrimeiro = novo;
                mUltimo = novo;
            } else {
                mUltimo->mProximo = novo;
                mUltimo = novo;
            }
            ++mTamanho;
        };

        //@return : unsigned $ tamanho da Lista.
        unsigned getTam () {
            return mTamanho;
        }

        void imprime () {
            Noh* aux = mPrimeiro;
            while (aux != NULL) {
                Serial.print("Dados : ");
                Serial.print(aux->mConteudo);
                aux = aux->mProximo;
            }
            Serial.print("Tamanho : ");
            Serial.println(mTamanho);
        };

        // Finalidade: remove todos os nós da lista.
        void removeTodos () {
            Noh* ant = NULL;
            Noh* aux = mPrimeiro;

            while (aux != NULL) {
                ant = aux;
                aux = aux->mProximo;
                delete ant;
            }

            mPrimeiro = NULL;
            mUltimo = NULL;
            mTamanho = 0;
        }

        //@return: float $ A média do conteúdo da lista.
        float media () {
            Noh* aux = mPrimeiro;
            float media = 0;
            while (aux != NULL) {
                media = media + aux->mConteudo;
                aux = aux->mProximo;
            }
            media = media / 30;
            return media;  
        };
        
         private :
            Noh* mPrimeiro;
            Noh* mUltimo;
            unsigned mTamanho;
};
