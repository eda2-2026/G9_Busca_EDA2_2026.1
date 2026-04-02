// a ideia inicial é criar a lógica para um hash em que cada bucket é uma lista encadeada com o limite de 4 elementos. 
// o hash deve suportar as operações de inserção, busca e remoção de elementos. 
// no cenário proposto, cada bucket vai funcionar como uma mesa de restaurante, onde cada cliente (elemento) é atendido em ordem de chegada (inserção).
// a função de hash vai determinar em qual mesa o cliente deve sentar, e a lista encadeada vai garantir que os clientes sejam atendidos na ordem correta.
// ideias a implementar: 
// atender cliente - buscar sequencialmente em outra tabela todas as informações do cliente, como o pedido, método de pagamento, etc. 
// verificar se o pedido do cliente existe no cardápio (pode ser uma outra estrutura)
// remover cliente - quando o cliente terminar de comer, ele deve ser removido da mesa (lista encadeada) e as informações do cliente devem ser arquivadas em outra estrutura para fins de histórico e análise de dados.
// criar uma nova função hash? vai que uma fica lotada e as outras vazias? talvez seja interessante ter mais de uma função hash para distribuir melhor os clientes entre as mesas

#include <iostream>
using namespace std;

struct Cadeira_ocupada {
    string nome_cliente;
    Cadeira_ocupada* prox;
};

const int NUM_MESAS = 5;
Cadeira_ocupada* mesas[NUM_MESAS];

int funcaoHash(string nome) {
    int soma = 0;
    for(char c : nome) {
        soma += c;
    }
    return soma % NUM_MESAS;
}

void sentarCliente(string nome) {
    int mesa = funcaoHash(nome);

    Cadeira_ocupada* novo = new Cadeira_ocupada;
    novo->nome_cliente = nome;
    novo->prox = mesas[mesa];

    mesas[mesa] = novo;

    cout << nome << " sentou na mesa " << mesa << endl;
}

void mostrarMesas() {
    for(int i = 0; i < NUM_MESAS; i++) {
        cout << "Mesa " << i << ": ";

        Cadeira_ocupada* atual = mesas[i];

        while(atual != nullptr) {
            cout << atual->nome_cliente << " -> ";
            atual = atual->prox;
        }

        cout << "NULL" << endl;
    }
}

int main() {

    for(int i = 0; i < NUM_MESAS; i++) {
        mesas[i] = nullptr;
    }
    // exemplos só pra ver rodando

    sentarCliente("Ana");
    sentarCliente("Lucas");
    sentarCliente("Carlos");
    sentarCliente("Marina");

    cout << endl;
    mostrarMesas();

    return 0;
}