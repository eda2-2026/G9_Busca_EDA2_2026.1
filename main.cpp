#include <string>
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

void BuscaDadosSequencial(string cpf_digitado) {
    for (const auto& cliente : lista_geral_clientes) {
        if (cliente.cpf == cpf_digitado) {
            int m = funcaoHash(cliente.nome_cliente);
            cout << "Informacoes do cliente: " << endl;
            cout << "Nome: " << cliente.nome_cliente << endl;
            cout << "Pedido: " << cliente.pedido << endl;
            cout << "CPF: " << cliente.cpf << endl;
            cout << "Localizacao: Mesa " << m << endl;
            cout << "============================================" << endl;
            return;
        }
    }
    cout << "Cliente nao encontrado." << endl;
}

void AtenderClienteExistente(string busca) {
    for (auto& cliente : lista_geral_clientes) {
        // Busca por nome ou CPF para facilitar
        if (cliente.nome_cliente == busca || cliente.cpf == busca) {
            cout << "\nAtendendo: " << cliente.nome_cliente << endl;
            cout << "1-Pizza, 2-Burger, 3-Sushi. Escolha: ";
            int prato;
            cin >> prato;

            if(prato == 1) cliente.pedido = "Pizza";
            else if(prato == 2) cliente.pedido = "Burger";
            else if(prato == 3) cliente.pedido = "Sushi";

            cout << "Pedido atualizado para " << cliente.nome_cliente << "!" << endl;
            return;
        }
    }
    cout << "Cliente nao encontrado na lista de espera." << endl;
}

void receberPagamentoESair(string busca) {
    int mesa_destino = -1;
    string nome_confirmado = "";

    for (auto it = lista_geral_clientes.begin(); it != lista_geral_clientes.end(); ) {
        if (it->nome_cliente == busca || it->cpf == busca) {
            if (it->pedido == "Aguardando..." || it->pedido.empty()) {
                cout << "Nao e possivel receber pagamento de " << it->nome_cliente
                     << " porque o pedido ainda nao foi realizado." << endl;
                return;
            }
            
            nome_confirmado = it->nome_cliente;
            // aqui ele faz a função hash de novo para saber onde o cliente está.
            mesa_destino = funcaoHash(nome_confirmado);
            
            cout << "\n>>> Pagamento de " << nome_confirmado << " realizado com sucesso!" << endl;
            
            it = lista_geral_clientes.erase(it); // Remove da lista geral também
            break; 
        } else {
            ++it; // Continua procurando
        }
    }

    if (mesa_destino == -1) {
        cout << "Cliente nao encontrado no registro geral." << endl;
        return;
    }
    Cadeira_ocupada* atual = mesas[mesa_destino];
    Cadeira_ocupada* anterior = nullptr; // so para mudar o ponteiro depois de encontrar o cliente
    while (atual != nullptr) { // aqui vai fazer a busca na lista encadeada da mesa
        if (atual->nome_cliente == nome_confirmado) {
            if (anterior == nullptr) {
                mesas[mesa_destino] = atual->prox;
            } 
            else {
                anterior->prox = atual->prox;
            }
            delete atual; 
            cout << "Cadeira liberada na Mesa " << mesa_destino << "." << endl;
            return;
        }
        anterior = atual;
        atual = atual->prox;
    }
}

int main() {

    for(int i = 0; i < NUM_MESAS; i++) {
        mesas[i] = nullptr;
    }

    string nome, cpf;
    int opcao;
    bool rodando = true;

    cout << "------RECEPCAO DO RESTAURANTE------" << endl;

    while(rodando) {
        // 1. Cadastro do Cliente
        cout << "\n--- Novo Cliente Chegando ---" << endl;
        cout << "Nome: ";
        getline(cin >> ws, nome);
        if(nome == "sair") break;
        
        cout << "CPF: ";
        cin >> cpf;

        // Salva na Lista Sequencial e na Tabela Hash
        InfoCliente novo = {nome, "Aguardando...", cpf};
        lista_geral_clientes.push_back(novo);
        sentarCliente(nome);

        // 2. Menu de Decisao Imediata
        bool decidindo = true;
        while(decidindo) {
            cout << "\nO que deseja fazer agora?" << endl;
            cout << "1 - Cadastrar proximo cliente" << endl;
            cout << "2 - Buscar cliente por CPF" << endl;
            cout << "3 - Ver mapa de mesas" << endl;
            cout << "4 - Atender cliente existente" << endl;
            cout << "5 - Receber pagamento" << endl;
            cout << "6 - Sair do programa" << endl;
            
            cout << "Escolha: ";
            cin >> opcao;

            if(opcao== 1) {
                decidindo = false; 
            }
            else if(opcao == 2) {
                string busca;
                cout << "Digite o CPF: ";
                cin >> busca;
                BuscaDadosSequencial(busca);
            }
            else if(opcao == 3) {
                mostrarMesas();
            }
            else if(opcao == 4) {
                string busca;
                cout << "Digite o nome ou CPF do cliente: ";
                getline(cin >> ws, busca);
                AtenderClienteExistente(busca);
            }
            else if(opcao == 5) {
                cout << "Digite o nome ou CPF para pagamento: ";
                string busca;
                getline(cin >> ws, busca);
                receberPagamentoESair(busca);
            }
            else if(opcao == 6) {
                decidindo = false;
                rodando = false;
            }
            
            else {
                cout << "Opcao invalida. Tente novamente." << endl;
            }
        }
    }

    return 0;
}