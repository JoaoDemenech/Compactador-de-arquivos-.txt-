#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>

using namespace std;

typedef struct node{

    char caractere;
    int quantidade;
    node *p;
    node *p2;
    bool interno = 0;
    // imaginar como um no de uma arvore, direita e P e esquerda P2
}node;

typedef struct dicionario {
    char caractere;
    std::vector<bool> codigo;
} dicionario;


std::vector<dicionario> listaDicionario;

node* contador(string caminho){

    ifstream arquivo;
    string linha;
    int contadorLinha = 0;
    int tamLinha;
    node *primeiro = NULL;
    node *ponteiro = NULL;

    arquivo.open(caminho);


    if (!arquivo.is_open()){
        cout << "erro ao abrir o arquivo " <<endl;
        return NULL;
    }
    else{
        cout << "ARQUIVO ABERTO COM SUCESSO " << endl;
    }

    // PEGA A PRIMEIRA LINHA
    while (getline(arquivo, linha)){
        tamLinha = linha.length();
        contadorLinha = 0;
        //ACESA CADA CARACTERE DA LINHA
        do{
            // CARACTERES NAO VALIDOS NAO SAO COMPACTADOS
            // !!! COMO VOU RECONSTRUIR O TXT DEPOIS? VER ISSO !!!
            if ( linha[contadorLinha] < 33 || linha[contadorLinha] > 126 ){
                contadorLinha++;
            }
            else{
                if (primeiro == NULL){
                    primeiro = new node;
                    primeiro->caractere = linha[contadorLinha];
                    primeiro->quantidade = 1;
                    primeiro->p = NULL;
                    primeiro->p2 = NULL;
                    cout<< "PRIMEIRO NODE ADICIONADO" << endl;
                }
                else{
                    ponteiro = primeiro;

                    while (ponteiro->p != NULL && ponteiro->caractere != linha[contadorLinha]){
                        ponteiro = ponteiro -> p;
                    }
                    if (ponteiro->caractere == linha[contadorLinha])
                        ponteiro->quantidade++;

                    else{
                        ponteiro->p = new node;
                        ponteiro->p->caractere = linha[contadorLinha];
                        ponteiro->p->quantidade = 1;
                        ponteiro->p->p = NULL;
                        ponteiro->p->p2 = NULL;
                    }
                }
            }
                contadorLinha++;
        }while(contadorLinha < tamLinha);
    }
return primeiro;
}

void imprimir(node* p){
    while (NULL != p){
        cout << p->caractere <<" "<<p->quantidade <<endl;
        p = p->p;
    }
    cout << "------------------" << endl;
}

node* remover(node* marcada, node* primeiro){
    if (primeiro == marcada){
        if (primeiro ->p != NULL){
            primeiro = primeiro->p;
            return primeiro;
        }
        else
            return NULL;
    }
    else{
        node* p = primeiro;
        while (primeiro->p != marcada){
            primeiro = primeiro->p;
        }
        primeiro->p = primeiro->p->p;
        return p;
    }
}

node* ordenar(node* primeiro){
    node* ordenado = NULL;  // onde adiciona as nodes em ordem de frequencia, menor ao maior
    node* marcador = NULL;  // marca a node com menor valor de frequencia
    node* p = primeiro;     // percore a lista nao ordenada
    int menorFrequencia = primeiro->quantidade;

    while (NULL != primeiro){
        p = primeiro;
        menorFrequencia = p ->quantidade;

        while (NULL != p){ // acha o menor valor na fila nao ordenada
            if (p->quantidade <= menorFrequencia ){
                menorFrequencia = p->quantidade;
                marcador = p;
            }
            p = p->p;
        }
        if (NULL == ordenado){
            ordenado = marcador;
            primeiro = remover ( marcador, primeiro);
            ordenado->p = NULL;
        }
        else{
            p = ordenado;
            while (p->p != NULL){
                p = p->p;
            }
            p->p = marcador;
            primeiro = remover ( marcador, primeiro);
            p->p->p = NULL;
        }
    }
    return ordenado;
}

node* montarArvore(node* inicio){
    // RAIZ ta sempre apontando pra raiz da arvore
    // inicio para a lista ordenada de nos que ainda nao estao na arvore

    node* raiz = inicio;
    node* no = NULL;

    if (NULL == inicio){
        cout << "ERRO, LISTA VAZIA" << endl;
        return NULL;
    }

    if (inicio->p ==NULL){
        return (raiz);
    }

    else{
        no = new node;
        no->quantidade = inicio->quantidade + inicio->p->quantidade;
        no->p2 = inicio;
        no->p = inicio->p;
        inicio = inicio->p->p;
        no->p->p = NULL;
        no->p2->p = NULL;
        raiz = no;
        raiz->interno = 1;

        while (NULL != inicio){
            no = new node;
            no->quantidade = inicio->quantidade + raiz->quantidade;
            no->p2 = raiz;
            no->p = inicio;
            inicio = inicio->p;
            no->p->p = NULL;
            raiz = no;
            raiz->interno = 1;
        }
        return(raiz);
    }
}

void imprimirArvore(node *raiz){

    if (raiz->p2 != NULL){
        imprimirArvore(raiz->p2);
    }
    if (raiz->p != NULL){
        imprimirArvore(raiz->p);
    }
    cout << raiz->quantidade <<endl;
}

int contarEsquerdaArvore(node* raiz){
    if (raiz->p2 != NULL)
        return (1 + contarEsquerdaArvore(raiz->p2));

    else
        return 1;
}

void gerarCodigos(node* raiz, std::vector<bool> caminho) { // e nao sei disso tmb
    if (raiz == NULL) return;

    // Se for nó folha, salva o caractere e o código acumulado
    if (raiz->interno == 0) {
        dicionario d;
        d.caractere = raiz->caractere;
        d.codigo = caminho;
        listaDicionario.push_back(d);
        return;
    }

    // Caminho para a esquerda (p2) recebe bit 0
    if (raiz->p2 != NULL) {
        caminho.push_back(0);
        gerarCodigos(raiz->p2, caminho);
        caminho.pop_back(); // Remove o bit para tentar o outro lado
    }

    // Caminho para a direita (p) recebe bit 1
    if (raiz->p != NULL) {
        caminho.push_back(1);
        gerarCodigos(raiz->p, caminho);
        caminho.pop_back();
    }
}

void salvarBinario(string caminhoOriginal, node* listaOrdenada) { // reescrever / estudar isso que nao sei como funciona

    string nomeSaida;
    cout << "Digite o nome do arquivo de saida com .bin no final: ";
    cin >> nomeSaida;

    ofstream arquivoSaida(nomeSaida, ios::binary);
    ifstream arquivoOriginal(caminhoOriginal);

    if (!arquivoSaida.is_open() || !arquivoOriginal.is_open()) {
        cout << "Erro ao abrir arquivos!" << endl;
        return;
    }

    // 1. GRAVAR CABEÇALHO (Quantos caracteres diferentes e suas frequências)

    int totalDiferentes = 0;
    node* temp = listaOrdenada;
    while(temp){
            totalDiferentes++;
            temp = temp->p;
    }

    arquivoSaida.write((char*)&totalDiferentes, sizeof(int));

    temp = listaOrdenada;
    while(temp) {
        arquivoSaida.put(temp->caractere);
        arquivoSaida.write((char*)&temp->quantidade, sizeof(int));
        temp = temp->p;
    }

    // 2. GRAVAR CONTEÚDO COMPACTADO
    unsigned char buffer = 0;
    int contadorBits = 0;
    char c;

    while (arquivoOriginal.get(c)) {
        // Busca o código do caractere no dicionário
        for (const auto& d : listaDicionario) {
            if (d.caractere == c) {
                for (bool bit : d.codigo) {
                    buffer = (buffer << 1) | bit;
                    contadorBits++;

                    if (contadorBits == 8) {
                        arquivoSaida.put(buffer);
                        buffer = 0;
                        contadorBits = 0;
                    }
                }
                break;
            }
        }
    }

    // Grava os bits restantes (padding)
    if (contadorBits > 0) {
        buffer = buffer << (8 - contadorBits);
        arquivoSaida.put(buffer);
    }

    cout << "Arquivo compactado com sucesso!" << endl;
    arquivoSaida.close();
    arquivoOriginal.close();
}

node* copiar(node* raiz){
    node* ponto = NULL;
    node* copia = NULL;

    if (raiz == NULL){
        return NULL;
    }
    else{
        ponto = new node;
        ponto -> caractere = raiz->caractere;
        raiz = raiz ->p;
        copia = ponto;

        while (raiz != NULL){
            ponto->p = new node;
            ponto->p->caractere = raiz->caractere;
            ponto->p->p = NULL;
            raiz = raiz->p;
            ponto = ponto->p;
        }
        return copia;
    }
}

int descompactar(){

    string caminhoArquivoBinario;
    string nomeArquivoSaida;
    int numCaracteresDiferentes;
    char tempChar;
    int tempFreq;

    // PRIMEIRO PEGAR E ABRIR O ARQUIVO BINARIO
    // ----------------------------------------------------------------------------------------

    cout << "digine o caminho completo do arquivo binario a ser descompactado: " <<endl;
    getline(cin, caminhoArquivoBinario);

    ifstream arquivoCompactado;
    arquivoCompactado.open(caminhoArquivoBinario, ios::in | ios::binary);

    if (!arquivoCompactado.is_open()){
        cout<< "erro ao abrir o arquivo compactado" <<endl;
        return 1;
    }
    else{
        cout<< "arquivo compactado aberto com sucesso " << endl;
    }

    // DEPOIS ABRIR O ARQUIVO TXT DE DESCOMPACTACAO
    // ----------------------------------------------------------------------------------------

    cout << "digite o nome do arquivo de saida com o .txt" << endl;
    getline(cin, nomeArquivoSaida);

    ofstream arquivoDeSaida(nomeArquivoSaida);

    if (!arquivoDeSaida.is_open()){
        cout << "Erro ao criar o arquivo de saida .txt"<<endl;
        return 1;
    }
    else{
        cout<< "arquivo de saida / descompactado aberto com sucesso " << endl;
    }

    // DEPOIS LER O ARQUIVO BINARIO E MONTAR A ARVORE NOVAMENTE

    arquivoCompactado.read((char*)&numCaracteresDiferentes, sizeof(int));
    cout << numCaracteresDiferentes << endl;

    for (int i = 0; i < numCaracteresDiferentes; i++){
        arquivoCompactado.read((char*)&tempChar, sizeof(char));
        arquivoCompactado.read((char*)&tempFreq, sizeof(int));
        cout << tempChar << endl;
        cout << tempFreq << endl;
    }

    return 1;
}


int main()
{
    node* prim = NULL;
    node* copiaOrdenada = NULL;
    string caminho;
    int op;

    do{
        cout << "------------------------------------------------" << endl;
        cout << " 1 - compactar um arquivo .txt " << endl;
        cout << " 2 - descompactar um arquivo .bin " << endl;
        cout << " 3 - encerrar o programa " << endl;
        cout << " digite a opcao desejada: " << endl;
        cin >> op;

        switch(op){
            case 1:{
                //compactar(); // passar toda a parte de compactar para ele, uma funcao propria que vai chamando as outras
                break;
            }
            case 2:{
                descompactar();
                break;
            }
            case 3:{
                break;
            }
            default:{
                cout << "opcao invalida, tente novamente " << endl;
                break;
            }
        }
    }while (3 != op);

    // 1 ETAPA - PEGAR O CAMINHO DO ARQUIVO TXT
    // ADICIONAR DEPOIS UM CHECK PARA TIPO DE ARQUIVO
    /*

    cout << "Digite o caminho completo sem "" do arquivo TXT a ser compactado: " << endl;
    getline(cin, caminho);
    system("cls");

    prim = contador(caminho);

    // ETAPA 1 FEITA COM SUCESSO

    // ETAPA 2 ORDENAR POR QUANTIDADE DE CARACTERER, MENOR PARA O MAIOR, CRIAR COPIA PARA DEPOIS

    prim = ordenar(prim);
    copiaOrdenada = copiar(prim);

    imprimir(prim); // IMPRIMIR OS CARACTERES EM ORDEM DE FREQUENCIA

    // criei uma copia da lista ordenada para ajudar a montar o dicionario
    // ETAPA 2 CONCLUIDA, ESTA EM ORDEM

    // ETAPA 3 MONTAR A ARVORE DE HUFFMAN

    prim = montarArvore(prim);
    // ETAPA 4 montar o dicionairo das substituicoes

    std::vector<bool> caminhoAux;
    gerarCodigos(prim, caminhoAux);

    // ETAPA 5 salvar em binario junto do dicionairo
    // BITS TOTAIS / CARACTERE E SUA FREQUENCIA / SEQUENCIA COMPLETA

    salvarBinario(caminho, copiaOrdenada);
    */
    return 1;
}

// TAREFAS
// pegar o mesmo nome do arquivo compactado e usar ele como saida na hora de descompactar
// tirar a necessidade de colocar o .txt, e de tirar os ""
// colocar o dicionario como algo local, juntar a criacao dele com a compactacao / salvar em binario, ou de ler em binario e criar um .txt

