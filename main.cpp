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

const string CLR_RESET = "\033[0m";
const string FG_BLACK = "\033[30m";
const string FG_WHITE = "\033[97m";
const string BG_BLUE = "\033[44m";
const string BG_GREEN = "\033[42m";
const string BG_RED = "\033[41m";
const string MENU_MARGIN = "    ";

void printInfo(const string& mensagem) {
    cout << BG_BLUE << FG_WHITE << " " << mensagem << " " << CLR_RESET << endl;
}

void printSuccess(const string& mensagem) {
    cout << BG_GREEN << FG_BLACK << " " << mensagem << " " << CLR_RESET << endl;
}

void printError(const string& mensagem) {
    cout << BG_RED << FG_WHITE << " " << mensagem << " " << CLR_RESET << endl;
}

void mostrarMenuComMargem() {
    cout << "\n";
    cout << MENU_MARGIN << "+----------------------------------+" << endl;
    cout << MENU_MARGIN << "| O que deseja fazer agora?        |" << endl;
    cout << MENU_MARGIN << "+----------------------------------+" << endl;
    cout << MENU_MARGIN << "| 1 - Cadastrar proximo cliente    |" << endl;
    cout << MENU_MARGIN << "| 2 - Buscar cliente por CPF       |" << endl;
    cout << MENU_MARGIN << "| 3 - Ver mapa de mesas            |" << endl;
    cout << MENU_MARGIN << "| 4 - Atender cliente existente    |" << endl;
    cout << MENU_MARGIN << "| 5 - Receber pagamento            |" << endl;
    cout << MENU_MARGIN << "| 6 - Sair do programa             |" << endl;
    cout << MENU_MARGIN << "+----------------------------------+" << endl;
}

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
        printError("Mesa " + to_string(mesa) + " esta cheia. " + nome + " nao pode sentar nessa mesa.");
        return;
    }
    
    Cadeira_ocupada* novo = new Cadeira_ocupada;
    novo->nome_cliente = nome;
    novo->prox = mesas[mesa];

    mesas[mesa] = novo;

    printSuccess(nome + " sentou na mesa " + to_string(mesa));
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
    printError("Cliente nao encontrado.");
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

            printSuccess("Pedido atualizado para " + cliente.nome_cliente + "!");
            return;
        }
    }
    printError("Cliente nao encontrado na lista de espera.");
}

void receberPagamentoESair(string busca) {
    int mesa_destino = -1;
    string nome_confirmado = "";

    for (auto it = lista_geral_clientes.begin(); it != lista_geral_clientes.end(); ) {
        if (it->nome_cliente == busca || it->cpf == busca) {
            if (it->pedido == "Aguardando..." || it->pedido.empty()) {
                printError("Nao e possivel receber pagamento de " + it->nome_cliente + " porque o pedido ainda nao foi realizado.");
                return;
            }
            
            nome_confirmado = it->nome_cliente;
            // aqui ele faz a função hash de novo para saber onde o cliente está.
            mesa_destino = funcaoHash(nome_confirmado);
            
            printSuccess("Pagamento de " + nome_confirmado + " realizado com sucesso!");
            
            it = lista_geral_clientes.erase(it); // Remove da lista geral também
            break; 
        } else {
            ++it; // Continua procurando
        }
    }

    if (mesa_destino == -1) {
        printError("Cliente nao encontrado no registro geral.");
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
            printInfo("Cadeira liberada na Mesa " + to_string(mesa_destino) + ".");
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

    printInfo("------RECEPCAO DO RESTAURANTE------");

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
            mostrarMenuComMargem();
            
            cout << MENU_MARGIN << "Escolha: ";
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
                printError("Opcao invalida. Tente novamente.");
            }
        }
    }

    return 0;
}