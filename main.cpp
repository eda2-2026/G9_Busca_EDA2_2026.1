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
#include <vector>
using namespace std;

struct Cadeira_ocupada {
    string nome_cliente;
    Cadeira_ocupada* prox;
};

const int NUM_MESAS = 5;
Cadeira_ocupada* mesas[NUM_MESAS];

struct InfoCliente{
    string nome_cliente;
    string pedido;
    string cpf;
};

vector<InfoCliente> lista_geral_clientes;

int funcaoHash(string nome) {
    int soma = 0;
    for(char c : nome) {
        soma += c;
    }
    return soma % NUM_MESAS;
}

void sentarCliente(string nome) {
    int mesa = funcaoHash(nome);

    int ocupadas = 0;
    Cadeira_ocupada* atual = mesas[mesa];
    while (atual != nullptr){
        ocupadas++;
        atual = atual -> prox;
    }

    if (ocupadas >= 4){
        cout << "Mesa " << mesa << " esta cheia. " << nome << " nao pode sentar nessa mesa." << endl;
        return;
    }
    
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

void BuscaDadosSequencial(string nome) {
    for (const auto& cliente : lista_geral_clientes) {
        if (cliente.nome_cliente == nome) {
            cout << "Informacoes do cliente: " << endl;
            cout << "Nome: " << cliente.nome_cliente << endl;
            cout << "Pedido: " << cliente.pedido << endl;
            cout << "CPF: " << cliente.cpf << endl;
            return;
        }
    }
    cout << "Cliente nao encontrado." << endl;
}

int main() {

    for(int i = 0; i < NUM_MESAS; i++) {
        mesas[i] = nullptr;
    }
    // exemplos só pra ver rodando

    string nome, cpf;

    cout << "------RECEPCAO DO RESTAURANTE------" << endl;

    //entrada de dados
    for(int i = 0; i < 3; i++) {
        cout << "Digite o nome do cliente: ";
        cin >> nome;
        cout << "Digite o CPF do cliente: ";
        cin >> cpf;

        // Cadastra na lista 
        InfoCliente novo;
        novo.nome_cliente = nome;
        novo.cpf = cpf;
        novo.pedido = "Aguardando atendimento"; 
        lista_geral_clientes.push_back(novo);

        // Tenta sentar o cliente
        sentarCliente(nome);
    }

    cout << "-----MAPA ATUAL-----" << endl;
    mostrarMesas();


    //testando a busca sequencial
    cout << "-----BUSCA SEQUENCIAL-----" << endl;
    cin >> nome;
    BuscaDadosSequencial(nome);

    return 0;
}