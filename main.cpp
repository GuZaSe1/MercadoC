#include <iostream>
#include <fstream>          //Para abrir arquivos .txt
#include <vector>           //Para aceitar vetores de forma mais facil
#include <locale.h>         //Para aceitar palavras no portugues
#include <string>           //Para aceitar strings
#include <iomanip>          //Para formatar a sa�da com precis�o
#include <ctime>            //Para obter e manipular datas
#include <algorithm>        //Para usar o replace
using namespace std;

//Limpar Tela
void LimparTela() {
    system("cls");
}

int opcaoInvalida(){
int numero;
    while (true){
    if (cin.fail()) {
            cin.clear();                //Limpa o estado de falha da entrada
            cin.ignore(1000, '\n');     //Desconsidera at� 1000 caracteres
            cout<<"\n---Digite um valor valido!---\n\n";
        }else{
            break;
        }
    }
    return numero;
}

//Estrutura para aceitar diferentes vari�veis dentro de uma s�
struct Produto {
    string nome;             //Nome do produto
    int quantidade;          //Quantidade em estoque
    float valor_venda;       //Pre�o de venda
};

//Fun��o para verificar se o produto j� existe no cadastro
int encontrarProduto(const vector<Produto>& produtos, const string& nome) {
    for (size_t i = 0; i < produtos.size(); i++) {
        if (produtos[i].nome == nome) {
            return i;  //Retorna o �ndice do produto se encontrado
        }
    }
    return -1;  //Retorna -1 se o produto n�o for encontrado
}

//Fun��o para calcular as datas das parcelas
vector<string> calcularDatasParcelas(int numParcelas) {
    vector<string> datasParcelas;       //Vetor que armazenar� as datas
    time_t t = time(0);                 //Obt�m a data e hora atuais
    tm* now = localtime(&t);            //Converte o tempo em um formato local

    //Calcula a data de cada parcela
    for (int i = 1; i <= numParcelas; i++) {
        now->tm_mon += 1;       //Incrementa 1 m�s para cada parcela
        mktime(now);            //Ajusta a estrutura de tempo
        char buffer[80];
        strftime(buffer, 80, "%d/%m/%Y", now);           //Formata a data
        datasParcelas.push_back(string(buffer));         //Adiciona a data formatada no vetor
    }
    return datasParcelas;
}

//Fun��o para carregar os produtos a partir de um arquivo .txt
void carregarProdutos(vector<Produto>& produtos) {
    ifstream arquivo("arquivo_mercado.txt");    //Abre o arquivo para leitura
    string linha;

    if (!arquivo.is_open()) {
        cout<<"\nErro ao abrir o arquivo!\n";
        return;
    }

    //Le o arquivo linha por linha e extrai os dados dos produtos
    while (getline(arquivo, linha)) {
        size_t pos1 = linha.find(";");                  //Encontra a posi��o do primeiro ';'
        size_t pos2 = linha.find(";", pos1 + 1);        //Encontra a posi��o do segundo ';'
        string nome = linha.substr(0, pos1);            //Extrai o nome
        int quantidade = stoi(linha.substr(pos1 + 1, pos2 - pos1 - 1));         //Extrai a quantidade
        float valor_venda = stof(linha.substr(pos2 + 1));                       //Extrai o valor

        Produto produto = {nome, quantidade, valor_venda};          //Cria o produto com os dados lidos
        produtos.push_back(produto);                                //Adiciona o produto ao vetor de produtos
    }
    arquivo.close();        //Fecha o arquivo
}

//Fun��o para salvar os produtos no arquivo .txt
void salvarProdutos(const vector<Produto>& produtos) {
    ofstream arquivo("arquivo_mercado.txt", ios::trunc);            //Abre o arquivo e sobrescreve o conte�do
    for (const auto& produto : produtos) {
        arquivo << produto.nome << ";" << produto.quantidade << ";" << produto.valor_venda << endl;          //Salva cada produto no arquivo
    }
    arquivo.close();       //Fecha o arquivo
}

//Fun��o para remover o ' ; ' do nome do produto
string removerPontoEVirgula(const string& input) {
    string resultado = input;
    replace(resultado.begin(), resultado.end(), ';', ' ');  //Substitui ' ; ' por espa�o para evitar problemas de formata��o
    return resultado;
}

//Fun��o para cadastrar produtos
void cadastrarProdutos(vector <Produto> & produtos){
    string nome;
    int quantidade;
    float valor_venda;

    cout<<"----------MERCADO----------\n";
    cout<<"\n----Cadatro de Produtos----\n";

    cout<<"\nDigite o nome do produto: ";
    getline(cin >> ws, nome);                   //Para desconsiderar espacos

    nome = removerPontoEVirgula(nome);          //Remove o ';' do nome do produto

    while (true){

    cout<<"\nDigite a quantidade do produto (em unidade): ";
    cin>>quantidade;
        if (cin.fail()) {
            opcaoInvalida();
        }else{
            break;
        }
    }
    while (true){
    cout<<"\nDigite o valor de venda do produto: ";
    cin>>valor_venda;
        if (cin.fail()) {
            opcaoInvalida();
        }else{
            break;
        }
    }

    int indice = encontrarProduto(produtos, nome);          //Verifica se o produto j� est� cadastrado

    if(indice != -1){
        produtos[indice].quantidade += quantidade;                                  //Atualiza a quantidade
        produtos[indice].valor_venda = valor_venda;                                 //Atualiza o valor de venda
        cout<<"\nProduto j� existente. Quantidade atualizada e valor de venda alterado!\n\n";
    }else{
        Produto novo_produto = {nome, quantidade, valor_venda};                     //Adiciona uma nova variavel ao vetor
        produtos.push_back(novo_produto);
        cout<<"\nProduto cadastrado com sucesso!\n\n";
        }
    salvarProdutos(produtos);       //Salva os produtos no arquivo
}

//Fun��o para listar produtos cadastrados
void listarProdutos(const vector<Produto>& produtos) {
    if (produtos.empty()) {             //Caso nao haja nenhum produto cadastrado
        cout<<"\nNenhum produto cadastrado!\n\n";
        return;
    }

    ifstream arquivo("arquivo_mercado.txt");        //Abre o arquivo para leitura
    string linha;

    if (!arquivo.is_open()) {
        cout<<"\nErro ao abrir o arquivo!\n\n";
        return;
    }

    cout<<"---------MERCADO---------\n";
    cout<<"\nLista de Produtos Cadastrados no Mercado:\n";

    //Le e exibe os produtos do arquivo
    while (getline(arquivo, linha)) {
        size_t pos1 = linha.find(";");
        size_t pos2 = linha.find(";", pos1 + 1);
        string nome = linha.substr(0, pos1);                                    //Nome do produto
        int quantidade = stoi(linha.substr(pos1 + 1, pos2 - pos1 - 1));         //Quantidade
        float valor_venda = stof(linha.substr(pos2 + 1));                       //Valor de venda

        //Exibe os dados do produto
        cout<<"\nProduto: " << nome;
        cout<<"\nQuantidade: " << quantidade;
        cout<<"\nValor de venda (Unidade): R$ " << valor_venda << "\n\n";
    }
    arquivo.close();        //Fecha o arquivo
}

//Fun��o para vender produtos
void venderProdutos(vector<Produto>& produtos) {
    if (produtos.empty()) {             //Caso nao haja nenhum produto para vender
        cout<<"\nNenhum produto dispon�vel para venda!\n\n";
        return;
    }

    string nome;
    float quantidade;
    float total = 0;

    cout<<"----------MERCADO----------\n";
    cout<<"\n-----Venda de Produtos-----\n";

    while (true) {
        cout<<"\nDigite o nome do produto ou (4- Voltar para o menu, 5- Finalizar compra): ";
        getline(cin >> ws, nome);

        if (nome == "5") {          //Opcao 5: finalizar a compra
            break;
        }
        if (nome == "4\n") {        //Opcao 4: voltar para o menu
            return;
        }

        nome = removerPontoEVirgula(nome);                  //Remove o ';' do nome do produto

        int indice = encontrarProduto(produtos, nome);      //Encontra o produto no sistema
        if (indice != -1) {
            //Pergunta a quantidade a ser comprada
            cout<<"\nDigite a quantidade do produto que deseja comprar: ";
            cin>>quantidade;
            LimparTela();

            if (cin.fail() || quantidade <= 0) {
                opcaoInvalida();
            }

            if (produtos[indice].quantidade >= quantidade) {
                total += produtos[indice].valor_venda * quantidade;  //Calcula o total
                produtos[indice].quantidade -= quantidade;           //Atualiza o estoque
                cout<<"----------MERCADO----------\n";
                cout<<"\n-----Venda de Produtos-----\n";
                cout<<"\nProduto adicionado ao carrinho!\n";
            } else {
                cout<<"----------MERCADO----------\n";
                cout<<"\n-----Venda de Produtos-----\n";
                cout<<"\nQuantidade insuficiente em estoque!\n";
            }
        } else {
            cout<<"\nProduto n�o encontrado.\n";
        }
    }

    LimparTela();  //Fun��o para limpar a tela
    cout<<"----------MERCADO----------\n";
    cout<<"\nValor total da compra: R$ "<< fixed << setprecision(2) << total << "\n";

    //Escolher forma de pagamento
    int opcaoPagamento;
    while (true){
    cout<<"\nEscolha a forma de pagamento:\n";
    cout<<"1. � vista (5% de desconto)\n";
    cout<<"2. Em 3 vezes sem juros\n";
    cout<<"3. Em 12 vezes com 10% de juros\n\n-";
    cin>>opcaoPagamento;
    if (cin.fail() || (opcaoPagamento < 1 || opcaoPagamento > 3)) {       //Verifica se a entrada foi inv�lida
        opcaoInvalida();
    } else {
        break;
        }
    }

    double valorFinal = total;          //Valor final da compra
    int numParcelas = 1;                //N�mero de parcelas

    //Calcula o valor final e o n�mero de parcelas de acordo com a op��o de pagamento
    switch (opcaoPagamento) {
        case 1:
            valorFinal = total * 0.95;  //5% de desconto
            numParcelas = 1;
            break;
        case 2:
            valorFinal = total;
            numParcelas = 3;
            break;
        case 3:
            valorFinal = total * 1.10;  //10% de juros
            numParcelas = 12;
            break;
    }

    cout<<"\nValor final: R$ "<< fixed << setprecision(2) << valorFinal <<"\n";

    //Exibe as parcelas, se houver
    if (numParcelas > 1) {
        vector<string> datasParcelas = calcularDatasParcelas(numParcelas);
        cout<<"\nParcelado em " << numParcelas << " vezes:\n";
        for (int i = 0; i < numParcelas; i++) {
            cout<< i + 1 << "� parcela: R$ " << fixed << setprecision(2) << valorFinal / numParcelas;
            cout<<" - Vencimento: " << datasParcelas[i] << "\n";
        }
    }

    salvarProdutos(produtos);       //Salva o estoque atualizado ap�s a venda
    cout << "\nCompra finalizada com sucesso!\n\n";
}

//Fun��o principal que exibe o menu e controla o programa
int main() {
    setlocale(LC_ALL, "Portuguese");        //Aceita o uso de acentos
    vector<Produto> produtos;               //Vetor de produtos
    carregarProdutos(produtos);             //Carrega os produtos do arquivo
    int opcao;

    while (true) {
        // MENU
        cout<<"---------MERCADO---------\n";
        cout<<"\nEscolha uma das op��es:\n";
        cout<<"\n1- Cadastrar Produtos\n";
        cout<<"2- Lista de Produtos\n";
        cout<<"3- Venda de Produtos\n";
        cout<<"4- Sair\n";
        cout<<"\nEscolha uma op��o: ";
        cin>>opcao;
        LimparTela();

        // Executa a op��o selecionada
        switch (opcao) {
            case 1:
                cadastrarProdutos(produtos);
                break;
            case 2:
                listarProdutos(produtos);
                break;
            case 3:
                venderProdutos(produtos);
                break;
            case 4:
                cout<<"---------MERCADO---------\n";
                cout<<"\nVoc� saiu com sucesso!\n";
                return 0;
            default:
                cout<<"\n-----Op��o inv�lida!-----\n\n";
                if (cin.fail()) {
                    opcaoInvalida();
                }
            }
        }
    return 0;
}
