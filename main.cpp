#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>

using namespace std;

typedef struct node{

    unsigned char caractere;
    int quantidade;
    node *p;
    node *p2;
    bool interno = 0;

    // imaginar como um no de uma arvore, direita e P e esquerda P2

}node;

typedef struct dicionario {
    unsigned char caractere;
    std::vector<bool> codigo;

} dicionario;

node* contador(string caminho) {

    ifstream arquivo(caminho, ios::binary); // Abre em modo binário para não ignorar caracteres como \n
    unsigned char c;
    char temp;
    node *primeiro = NULL;
    node *ponteiro = NULL;

    if (!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo" << endl;
        return NULL;
    }

    // Lê o arquivo caractere por caractere até o fim
    while (arquivo.get(temp)){

        c = (unsigned char)temp;

        if (primeiro == NULL){
            primeiro = new node;
            primeiro->caractere = c;
            primeiro->quantidade = 1;
            primeiro->p = NULL;
            primeiro->p2 = NULL;
            primeiro->interno = false;
        }
        else{
            ponteiro = primeiro;
            // Procura o caractere na lista ligada

            while (ponteiro->p != NULL && ponteiro->caractere != c)
                ponteiro = ponteiro->p;

            if (ponteiro->caractere == c)
                ponteiro->quantidade++;

            else{
                // Se não achou, cria um novo nó no fim da lista
                ponteiro->p = new node;
                ponteiro->p->caractere = c;
                ponteiro->p->quantidade = 1;
                ponteiro->p->p = NULL;
                ponteiro->p->p2 = NULL;
                ponteiro->p->interno = false;
            }
        }
    }
    arquivo.close();
    return primeiro;
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

int salvarBinario(string caminho, node* copiaListaOrdenada, node* raizArvore){

    // aqui ocorre primeiro a montagem do dicionario, depois o salvamento em binario

    std::vector<bool> codigo; // o codigo que vou indo salvando
    std::vector<dicionario> listaDicionariol; // cria a lista de dicionario
    node* p;
    dicionario aux;

    if (!raizArvore){
        cout << "ERRO ARVORE VAIZIA" << endl;
        return 1;
    }

    // se a arvore tem apenas um no / elemento
    if (raizArvore->interno == 0){
        aux.caractere = raizArvore->caractere;
        codigo.push_back(1);
        aux.codigo = codigo;
        listaDicionariol.push_back(aux);
    }
    else{
        do{
            // checa se ja esta no fim da arvore, no caso de ter 1 caractere so este seria interno e o unico que precisa ser ADD no dicionario
            if (raizArvore->interno == 1){
                p = raizArvore->p;
                if (p->interno == 0){

                // ele vai sempre para a esquerda, se for interno ele coloca o 1 final do codigo, salva no dicionario o caractere que tem no node
                // bem como o codigo salvo ate agora, dai ele ja tira esse 1 adicionado

                    codigo.push_back(1);
                    aux.caractere = p->caractere;
                    aux.codigo = codigo;
                    listaDicionariol.push_back(aux);
                    codigo.pop_back();
                }
                codigo.push_back(0);
            }
            else{
                // so vai entrar aqui uma vez, no ultimo caractere da arvore, depois disso p2 == NULL entao sai do loop;
                aux.caractere = raizArvore->caractere;
                aux.codigo = codigo;
                listaDicionariol.push_back(aux);
            }
            raizArvore = raizArvore->p2;
            // o codigo vai percorrendo, ele entra no p onde fica a maioria dos caracteres e salva ele no dicionario
            // tem que adicinoar para o ultimo caractere que esta presente no p2
        }while (raizArvore);
    }
    // DICIONARIO CONPLETO
    // AGORA TENHO QUE SALVAR O CABECALHO

    ifstream arquivoOriginal;
    arquivoOriginal.open(caminho, ios::in | ios::binary); // ABRE O ARQUIVO ORIGINAL NA FORMA DE BINARIO

    string nomeArquivoCompactado;
    cout << "Digite um nome para o arquivo de saida com .bin" << endl;
    getline (cin, nomeArquivoCompactado);

    ofstream arquivoCompactado(nomeArquivoCompactado, ios::binary);

    if (!arquivoOriginal.is_open()){
        cout << "ERRO: FALHA EM ABRIR O ARQUIVO ORIGINAL " << endl;
        return 1;
    }
    else{
        cout << "ARQUIVO ORIGINAL ABERTO COM SUCESSO " << endl;
    }
    if (!arquivoCompactado.is_open()){
        cout << "ERRO: FALHA AO CRIAR O ARQUIVO .BIN DE SAIDA" << endl;
        return 1;
    }
    else{
        cout << "ARQUIVO COMPACTADO CRIADO COM SUCESSO " << endl;
    }

    // salvando o cabecalho, primeiro o numero total de caracteres diferentes, 4 Bytes
    int numCaracteresDiferentes = 0;
    p = copiaListaOrdenada;

    while(p){
        numCaracteresDiferentes++;
        p = p->p;
    }

    arquivoCompactado.write((char*)&numCaracteresDiferentes, sizeof(int32_t));

    cout << "numero de caracteres diferentes : " << numCaracteresDiferentes << endl;

    // salvar os respectivos caracteres (1 byte ) e sua frequencia ( 4 bytes )

    p = copiaListaOrdenada;

    while (p){
        arquivoCompactado.write((char*)&p->caractere, sizeof(char));
        arquivoCompactado.write((char*)&p->quantidade, sizeof(int32_t));
        p = p->p;
    }

    // agora preciso adicionar e ir substituindo caractere por caractere, a parte foda

    char temp;
    unsigned char buffer = 0  ; // se iniciar isso como 1, fica 10000000, e como meu formato e 1,01,001... da pra usar
    int i, tam, j;
    int contadorBuffer = 0;

    while (arquivoOriginal.get(temp)){

        for (i = 0; listaDicionariol[i].caractere != temp; i++){
            if (i == numCaracteresDiferentes)
                break;
        }; // acha o index do dicionario onde o caractere esta

        tam = listaDicionariol[i].codigo.size();

        for (j = 0; j < tam; j++){

            if (listaDicionariol[i].codigo[j] == 0){
                contadorBuffer++;
            }
            if (listaDicionariol[i].codigo[j] == 1){
                buffer = buffer|(1<<contadorBuffer);
                contadorBuffer++;
            }
            if (contadorBuffer == 8){
                arquivoCompactado.write((char*)&buffer, sizeof(char));
                buffer = 0;
                contadorBuffer = 0;
            }
        }
    }
    if (0 != contadorBuffer)
        arquivoCompactado.write((char*)&buffer, sizeof(char));

    arquivoOriginal.close();
    arquivoCompactado.close();
    return 1;
}

node* copiar(node* raiz){
    node* ponto = NULL;
    node* copia = NULL;

    if (raiz == NULL){
        return NULL;
    }
    else{
        ponto = new node;
        ponto->caractere = raiz->caractere;
        ponto->quantidade = raiz->quantidade;
        raiz = raiz ->p;
        copia = ponto;

        while (raiz != NULL){
            ponto->p = new node;
            ponto->p->caractere = raiz->caractere;
            ponto->p->quantidade = raiz->quantidade;
            ponto->p->p = NULL;
            raiz = raiz->p;
            ponto = ponto->p;
        }
        return copia;
    }
}

int descompactar() {
    string caminhoArquivoBinario;
    string nomeArquivoSaida;
    int numCaracteresDiferentes;
    unsigned char tempChar;
    int tempFreq;
    node* prim = NULL;
    node* auxNode;
    int i;

    // Variável para controlar o fim exato do arquivo
    int totalCaracteresOriginal = 0;

    cout << "Digite o caminho do arquivo .bin: " << endl;
    getline(cin, caminhoArquivoBinario);

    ifstream arquivoCompactado(caminhoArquivoBinario, ios::binary);
    if (!arquivoCompactado.is_open()) {
        cout << "Erro ao abrir o binario" << endl;
        return 1;
    }

    cout << "Digite o nome do arquivo de saida (.txt): " << endl;
    getline(cin, nomeArquivoSaida);
    ofstream arquivoDeSaida(nomeArquivoSaida);

    // 1. LER CABEÇALHO: Número de caracteres diferentes
    arquivoCompactado.read((char*)&numCaracteresDiferentes, sizeof(int32_t));

    // 2. LER CARACTERES E FREQUÊNCIAS (E somar o total)
    for (i = 0; i < numCaracteresDiferentes; i++) {
        arquivoCompactado.read((char*)&tempChar, sizeof(char));
        arquivoCompactado.read((char*)&tempFreq, sizeof(int32_t));

        totalCaracteresOriginal += tempFreq; // Acumula o total de caracteres

        node* p = new node;
        p->caractere = tempChar;
        p->quantidade = tempFreq;
        p->p = NULL;
        p->p2 = NULL;
        p->interno = 0;

        if (prim == NULL) {
            prim = p;
        } else {
            auxNode = prim;
            while (auxNode->p != NULL) auxNode = auxNode->p;
            auxNode->p = p;
        }
    }

    // 3. RECONSTRUIR A ÁRVORE E O DICIONÁRIO
    node* raizArvore = montarArvore(prim);

    vector<dicionario> listaDicionariol;
    vector<bool> codigoCaminho;
    node* p_aux = raizArvore;

    // Lógica de reconstrução do dicionário (Igual à sua do salvarBinario)
    if (p_aux->interno == 0) {
        dicionario d;
        d.caractere = p_aux->caractere;
        codigoCaminho.push_back(1);
        d.codigo = codigoCaminho;
        listaDicionariol.push_back(d);
    } else {
        do {
            if (p_aux->interno == 1) {
                if (p_aux->p->interno == 0) {
                    dicionario d;
                    codigoCaminho.push_back(1);
                    d.caractere = p_aux->p->caractere;
                    d.codigo = codigoCaminho;
                    listaDicionariol.push_back(d);
                    codigoCaminho.pop_back();
                }
                codigoCaminho.push_back(0);
            } else {
                dicionario d;
                d.caractere = p_aux->caractere;
                d.codigo = codigoCaminho;
                listaDicionariol.push_back(d);
            }
            p_aux = p_aux->p2;
        } while (p_aux);
    }

    // 4. LER O TEXTO COMPACTADO (Bit a Bit)
    unsigned char buffer;
    vector<bool> codigoAtual;
    int caracteresProcessados = 0;

    // Lê byte a byte do arquivo
    while (arquivoCompactado.read((char*)&buffer, sizeof(unsigned char)) && caracteresProcessados < totalCaracteresOriginal) {

        // Percorre os 8 bits do byte (Direita para Esquerda)
        for (int b = 0; b < 8; b++) {
            bool bit = (buffer & (1 << b)) != 0;
            codigoAtual.push_back(bit);

            // TESTA O DICIONÁRIO: A cada bit novo, verificamos se o código formou um caractere
            for (const auto& item : listaDicionariol) {
                if (item.codigo == codigoAtual) {
                    arquivoDeSaida.put(item.caractere);
                    caracteresProcessados++;
                    codigoAtual.clear(); // Limpa para o próximo caractere
                    break;
                }
            }

            // Para assim que recuperar todos os caracteres do original
            if (caracteresProcessados == totalCaracteresOriginal) break;
        }
    }

    arquivoCompactado.close();
    arquivoDeSaida.close();
    cout << "processo concluido com sucesso " << endl;
    return 0;
}

void compactar(){
    // AQUI OCORREM AS MULTIPLAS ETAPAS NECESSESAIRAS PARA COMPACTAR O ARQUIVO

    node* prim = NULL;          // ponteiro para a lista de NODES principal que e usada por varias etapas para ir realizando o processo
    node* copiaOrdenada = NULL; // copia da lista ordenada para facilitar a criacao do dicionario, ver depois de retirar a necessidade disso
    string caminho;             // caminho do arquivo .txt que sera compactado

    // 1 ETAPA - PEGAR O CAMINHO DO ARQUIVO TXT E CONTAR OS CARACTERES CRIANDO A FILA DE NODES

    cout << "Digite o caminho completo sem "" do arquivo .txt a ser compactado: " << endl;
    getline(cin, caminho);
    system("cls");

    prim = contador(caminho);

    cout << "1 - Nodes criadas" << endl;

    // ETAPA 2 ORDENAR POR QUANTIDADE DE CARACTERER, MENOR PARA O MAIOR, CRIAR COPIA PARA DEPOIS

    prim = ordenar(prim);
    copiaOrdenada = copiar(prim);

    cout << "2 - Nodes ordenadas e copia criada" << endl;

    // ETAPA 3 MONTAR A ARVORE DE HUFFMAN

    prim = montarArvore(prim);

    cout << "3 - arvore montada " << endl;

    // ETAPA 4 MONTAR O DICIONARIO E CRIAR O BINARIO

    salvarBinario(caminho, copiaOrdenada, prim);

    cout << "4 - Salvo o arquivo compactado .bin " << endl;
}

int main()
{
    int op;

    do{
        cout << "------------------------------------------------" << endl;
        cout << " 1 - compactar um arquivo .txt " << endl;
        cout << " 2 - descompactar um arquivo .bin " << endl;
        cout << " 3 - encerrar o programa " << endl;
        cout << " digite a opcao desejada: " << endl;
        cin >> op;

        getchar();
        system("cls");

        switch(op){
            case 1:{
                compactar(); // passar toda a parte de compactar para ele, uma funcao propria que vai chamando as outras
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
    return 1;
}

// TAREFAS
// pegar o mesmo nome do arquivo compactado e usar ele como saida na hora de descompactar
// tirar a necessidade de colocar o .txt, e de tirar os ""
// adicionar um check para o tipo de arquivo na hora de pegar o caminho do .txt
// modificar, em vez de lista encadeada enquanto le os caracteres usar um vetor, depois ordenar ele
// mudar a ordem de escrita dos buffer, ta escrevendo direita esquerda, deixar esquerda direita


