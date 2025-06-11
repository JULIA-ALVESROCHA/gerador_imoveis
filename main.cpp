/**
 * Sistema de Gestão de Imóveis - AEDs I – Prática (DCE794) - Ciência da Computação (08)
 * Júlia Alves da Rocha - 2025.1.08.015
 * * =======================================================================================
 * DESCRIÇÃO GERAL DO CÓDIGO
 * =======================================================================================
 * Este programa é um sistema de gerenciamento de imóveis baseado em console.
 * Ele permite cadastrar, listar, buscar, excluir e gerar relatórios estatísticos
 * sobre imóveis. Os dados são persistidos (salvos e carregados) em um arquivo de
 * texto chamado "BD_Imoveis2.txt", permitindo que as informações não se percam
 * ao fechar o programa.
 * =======================================================================================
 * * NOTAS DA REVISÃO:
 * - A função carregarArquivo() foi ajustada para reconhecer e parar a leitura
 * ao encontrar a linha "fim" no arquivo de banco de dados.
 * - A função salvarArquivo() foi ajustada para escrever a linha "fim" ao final
 * do arquivo, mantendo a consistência do formato.
 * - Adicionado tratamento de erro mais claro caso o arquivo não possa ser aberto.
 * - Removida a pausa inicial desnecessária após o carregamento do arquivo.
 * - Corrigida a função pausar() para lidar corretamente com o buffer de entrada.
 * - A função relatorioEstatisticas() foi expandida para incluir a porcentagem
 * de salas comerciais com piso de cerâmica.
 */

//Bibliotecas 
#include <iostream>
#include <vector> 
#include <fstream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <limits> // Necessário para a função pausar() corrigida

using namespace std;

//Estrutura de Dados
struct Imovel {
    string tipo;           // casa, apartamento, comercial
    string finalidade;     // venda, aluguel, temporada
    string endereco;       // endereço completo
    string bairro;         // bairro
    string cidade;         // cidade
    double area;           // área em m²
    string valor;          // valor em R$ ou R$/dia (string para flexibilidade)
    double iptu;           // valor do IPTU
    int quartos;           // número de quartos
    int suites;            // número de suítes
    int banheiros;         // número de banheiros
    int vagas;             // número de vagas de garagem
    string cozinha;        // sim/não
    string sala;           // sim/não
    string varanda;        // sim/não
    string areaServico;    // sim/não (área de serviço)
    string piso;           // cerâmica, laminado, etc.
    string conservacao;    // novo, usado, reformado
    string armarios;       // sim/não
    string arCondicionado; // sim/não
    string aquecedor;      // sim/não
    string ventilador;     // sim/não
};

// Variáveis globais
vector<Imovel> imoveis;
const string ARQUIVO = "BD_Imoveis2.txt"; 
const int MAX_REGISTROS = 200;

// Protótipos das funções com nomes autoexplicativos
void mostrarMenu();
void incluirImovel();
void listarImoveis();
void buscarPorRua();
void buscarPorValor();
void buscarPorCaracteristicas();
void buscarPorQuartosSuites();
void relatorioEstatisticas();
void excluirImovel();
void salvarArquivo();
void carregarArquivo();
void limparTela();
void pausar();
string lerSimNao(const string& pergunta);
double converterValorParaDouble(const string& valorStr);

int main() {
    cout << "=== Sistema de Gestao de Imoveis ===" << endl;
    carregarArquivo();
    // A pausa inicial foi removida para um início mais rápido e fluído.
    
    int opcao;
    do {
        mostrarMenu();
        cout << "Escolha uma opcao: ";
        cin >> opcao;
        
        // Tratar entrada inválida para opção
        while(cin.fail()) {
            cout << "Entrada invalida. Por favor, digite um numero: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            cin >> opcao;
        }
        
        switch(opcao) {
            case 1: incluirImovel(); break;
            case 2: listarImoveis(); break;
            case 3: buscarPorRua(); break;
            case 4: buscarPorValor(); break;
            case 5: buscarPorCaracteristicas(); break;
            case 6: buscarPorQuartosSuites(); break;
            case 7: relatorioEstatisticas(); break;
            case 8: excluirImovel(); break;
            case 0:
                cout << "Salvando dados no arquivo " << ARQUIVO << "..." << endl;
                salvarArquivo();
                cout << "Programa encerrado!" << endl;
                break;
            default:
                cout << "Opcao invalida!" << endl;
                pausar();
        }
    } while(opcao != 0);
    
    return 0;
}

//Mostrar ao usuário as opções do menu principal
void mostrarMenu() {
    limparTela();
    cout << "================================" << endl;
    cout << "   SISTEMA DE GESTAO DE IMOVEIS" << endl;
    cout << "================================" << endl;
    cout << "1. Incluir Imovel" << endl;
    cout << "2. Listar Todos os Imoveis" << endl;
    cout << "3. Buscar por Rua/Endereco" << endl;
    cout << "4. Buscar por Faixa de Valor" << endl;
    cout << "5. Buscar por Caracteristicas" << endl;
    cout << "6. Buscar por Quartos e Suites" << endl;
    cout << "7. Relatorio de Estatisticas" << endl;
    cout << "8. Excluir Imovel" << endl;
    cout << "0. Sair" << endl;
    cout << "================================" << endl;
    cout << "Total de imoveis: " << imoveis.size() << "/" << MAX_REGISTROS << endl;
    cout << "================================" << endl;
}

//Tratamento de Erro - ajuda a melhor ler a resposta sim ou não do usuário
string lerSimNao(const string& pergunta) {
    char resposta;
    cout << pergunta << " (s/n): ";
    cin >> resposta;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpar o buffer
    return (resposta == 's' || resposta == 'S') ? "sim" : "não";
}

//Solicita os dados de um novo imóvel ao usuário e o adiciona ao vetor 'imoveis'.
void incluirImovel() {
    limparTela();
    
    //Verifica o tamanho atual do arquivo 
    if(imoveis.size() >= MAX_REGISTROS) {
        cout << "Limite máximo de " << MAX_REGISTROS << " registros atingido!" << endl;
        pausar();
        return;
    }
    
    cout << "=== INCLUIR NOVO IMOVEL ===" << endl;
    
    Imovel novo; 
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpar o buffer antes de começar
    
    cout << "Tipo (casa/apartamento/sala_comercial/terreno/galpao): ";
    getline(cin, novo.tipo);
    
    cout << "Finalidade (venda/aluguel/temporada): ";
    getline(cin, novo.finalidade);
    
    cout << "Endereco: ";
    getline(cin, novo.endereco);
    replace(novo.endereco.begin(), novo.endereco.end(), ' ', '_');
    
    cout << "Bairro: ";
    getline(cin, novo.bairro);
    replace(novo.bairro.begin(), novo.bairro.end(), ' ', '_');
    
    cout << "Cidade: ";
    getline(cin, novo.cidade);
    replace(novo.cidade.begin(), novo.cidade.end(), ' ', '_');
    
    cout << "Area (m²): ";
    cin >> novo.area;
    
    cout << "Valor (Ex: 350000 ou 300/dia): ";
    cin >> novo.valor;
    
    cout << "IPTU (R$): ";
    cin >> novo.iptu;
    
    cout << "Numero de quartos: ";
    cin >> novo.quartos;
    
    cout << "Numero de suites: ";
    cin >> novo.suites;
    
    cout << "Numero de banheiros: ";
    cin >> novo.banheiros;
    
    cout << "Numero de vagas de garagem: ";
    cin >> novo.vagas;
    
    cout << "\n--- CARACTERISTICAS DO IMOVEL ---" << endl;
    novo.cozinha = lerSimNao("Possui cozinha?");
    novo.sala = lerSimNao("Possui sala?");
    novo.varanda = lerSimNao("Possui varanda?");
    novo.areaServico = lerSimNao("Possui area de servico?");
    
    cout << "Tipo de piso (ceramica/laminado/porcelanato/etc): ";
    getline(cin, novo.piso);
    replace(novo.piso.begin(), novo.piso.end(), ' ', '_');
    
    cout << "Conservacao (novo/usado/reformado): ";
    getline(cin, novo.conservacao);
    
    cout << "\n--- EQUIPAMENTOS ---" << endl;
    novo.armarios = lerSimNao("Possui armarios?");
    novo.arCondicionado = lerSimNao("Possui ar-condicionado?");
    novo.aquecedor = lerSimNao("Possui aquecedor?");
    novo.ventilador = lerSimNao("Possui ventilador?");
    
    imoveis.push_back(novo);
    cout << endl << "Imovel incluido com sucesso!" << endl;
    cout << "Total de imoveis: " << imoveis.size() << "/" << MAX_REGISTROS << endl;
    pausar();
}

// Exibe na tela todos os imóveis cadastrados no sistema
void listarImoveis() {
    limparTela();
    if(imoveis.empty()) {
        cout << "Nenhum imovel cadastrado!" << endl;
        pausar();
        return;
    }
    
    cout << "=== LISTA DE TODOS OS IMOVEIS DISPONIVEIS ===" << endl;
    
    for(size_t i = 0; i < imoveis.size(); i++) {
        cout << "\n--- Imovel " << (i+1) << " ---" << endl;
        cout << "Tipo: " << imoveis[i].tipo << endl;
        cout << "Finalidade: " << imoveis[i].finalidade << endl;
        
        string endereco = imoveis[i].endereco;
        replace(endereco.begin(), endereco.end(), '_', ' ');
        cout << "Endereco: " << endereco << endl;
        
        string bairro = imoveis[i].bairro;
        replace(bairro.begin(), bairro.end(), '_', ' ');
        cout << "Bairro: " << bairro << endl;
        
        string cidade = imoveis[i].cidade;
        replace(cidade.begin(), cidade.end(), '_', ' ');
        cout << "Cidade: " << cidade << endl;
        
        cout << "Area: " << imoveis[i].area << " m²" << endl;
        cout << "Valor: R$ " << imoveis[i].valor << endl;
        cout << "IPTU: R$ " << fixed << setprecision(2) << imoveis[i].iptu << endl;
        cout << "Quartos: " << imoveis[i].quartos << " | Suites: " << imoveis[i].suites << endl;
        cout << "Banheiros: " << imoveis[i].banheiros << " | Vagas: " << imoveis[i].vagas << endl;
        
        cout << "Ambientes: ";
        if(imoveis[i].cozinha == "sim") cout << "Cozinha ";
        if(imoveis[i].sala == "sim") cout << "Sala ";
        if(imoveis[i].varanda == "sim") cout << "Varanda ";
        if(imoveis[i].areaServico == "sim") cout << "Area-de-Servico ";
        cout << endl;
        
        string piso = imoveis[i].piso;
        replace(piso.begin(), piso.end(), '_', ' ');
        cout << "Piso: " << piso << " | Conservacao: " << imoveis[i].conservacao << endl;
        
        cout << "Equipamentos: ";
        if(imoveis[i].armarios == "sim") cout << "Armarios ";
        if(imoveis[i].arCondicionado == "sim") cout << "Ar-condicionado ";
        if(imoveis[i].aquecedor == "sim") cout << "Aquecedor ";
        if(imoveis[i].ventilador == "sim") cout << "Ventilador ";
        cout << endl;
    }
    pausar();
}

void buscarPorRua() {
    limparTela();
    if(imoveis.empty()) {
        cout << "Nenhum imovel cadastrado!" << endl;
        pausar();
        return;
    }
    
    cout << "=== BUSCAR POR ENDERECO/RUA ===" << endl;
    string busca;
    
    cout << "Digite parte do endereco: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, busca);
    
    vector<int> encontrados;
    cout << "\nImoveis encontrados:" << endl;
    
    for(size_t i = 0; i < imoveis.size(); i++) {
        string endereco = imoveis[i].endereco;
        replace(endereco.begin(), endereco.end(), '_', ' ');
        
        string enderecoLower = endereco;
        string buscaLower = busca;
        transform(enderecoLower.begin(), enderecoLower.end(), enderecoLower.begin(), ::tolower);
        transform(buscaLower.begin(), buscaLower.end(), buscaLower.begin(), ::tolower);
        
        if(enderecoLower.find(buscaLower) != string::npos) {
            cout << "\n--- Imovel " << (i+1) << " ---" << endl;
            cout << "Tipo: " << imoveis[i].tipo << endl;
            cout << "Endereco: " << endereco << endl;
            cout << "Valor: R$ " << imoveis[i].valor << endl;
            encontrados.push_back(i);
        }
    }
    
    if(encontrados.empty()) {
        cout << "Nenhum imovel encontrado!" << endl;
    }
    
    pausar();
}

// Converte o valor do imóvel (que pode ser "350000" ou "300/dia") para double
double converterValorParaDouble(const string& valorStr) {
    try {
        return stod(valorStr);
    } catch (const invalid_argument&) {
        // Se não for um número simples, tenta extrair o número antes de "/"
        size_t pos = valorStr.find('/');
        if (pos != string::npos) {
            try {
                return stod(valorStr.substr(0, pos));
            } catch (const invalid_argument&) {
                return 0.0;
            }
        }
        return 0.0;
    }
}

void buscarPorValor() {
    limparTela();
    if(imoveis.empty()) {
        cout << "Nenhum imovel cadastrado!" << endl;
        pausar();
        return;
    }
    
    cout << "=== BUSCAR POR FAIXA DE VALOR ===" << endl;
    double valorMin, valorMax;
    string finalidade;
    
    cout << "Finalidade (venda/aluguel/temporada ou ENTER para todas): ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, finalidade);
    
    cout << "Valor minimo (R$): ";
    cin >> valorMin;
    cout << "Valor maximo (R$): ";
    cin >> valorMax;
    
    cout << "\nImoveis encontrados:" << endl;
    bool encontrou = false;
    
    for(size_t i = 0; i < imoveis.size(); i++) {
        double valorImovel = converterValorParaDouble(imoveis[i].valor);
        bool valorOk = (valorImovel >= valorMin && valorImovel <= valorMax);
        bool finalidadeOk = finalidade.empty() || 
                           (imoveis[i].finalidade.find(finalidade) != string::npos);
        
        if(valorOk && finalidadeOk) {
            cout << "\n--- Imovel " << (i+1) << " ---" << endl;
            cout << "Tipo: " << imoveis[i].tipo << endl;
            cout << "Finalidade: " << imoveis[i].finalidade << endl;
            
            string endereco = imoveis[i].endereco;
            replace(endereco.begin(), endereco.end(), '_', ' ');
            cout << "Endereco: " << endereco << endl;
            
            cout << "Valor: R$ " << imoveis[i].valor << endl;
            encontrou = true;
        }
    }
    
    if(!encontrou) {
        cout << "Nenhum imovel encontrado nos criterios especificados!" << endl;
    }
    pausar();
}


void buscarPorCaracteristicas() {
    limparTela();
    if(imoveis.empty()) {
        cout << "Nenhum imovel cadastrado!" << endl;
        pausar();
        return;
    }
    
    cout << "=== BUSCAR POR CARACTERISTICAS ===" << endl;
    cout << "Marque as caracteristicas desejadas:" << endl;
    
    string buscaArmarios = lerSimNao("Armarios?");
    string buscaAr = lerSimNao("Ar-condicionado?");
    string buscaAquecedor = lerSimNao("Aquecedor?");
    string buscaVentilador = lerSimNao("Ventilador?");
    
    cout << "\nImoveis encontrados:" << endl;
    bool encontrou = false;
    
    for(size_t i = 0; i < imoveis.size(); i++) {
        bool match = true;
        
        if(buscaArmarios == "sim" && imoveis[i].armarios != "sim") match = false;
        if(buscaAr == "sim" && imoveis[i].arCondicionado != "sim") match = false;
        if(buscaAquecedor == "sim" && imoveis[i].aquecedor != "sim") match = false;
        if(buscaVentilador == "sim" && imoveis[i].ventilador != "sim") match = false;
        
        if(match) {
            cout << "\n--- Imovel " << (i+1) << " ---" << endl;
            cout << "Tipo: " << imoveis[i].tipo << endl;
            
            string endereco = imoveis[i].endereco;
            replace(endereco.begin(), endereco.end(), '_', ' ');
            cout << "Endereco: " << endereco << endl;
            
            cout << "Valor: R$ " << imoveis[i].valor << endl;
            cout << "Equipamentos: ";
            if(imoveis[i].armarios == "sim") cout << "Armarios ";
            if(imoveis[i].arCondicionado == "sim") cout << "Ar-condicionado ";
            if(imoveis[i].aquecedor == "sim") cout << "Aquecedor ";
            if(imoveis[i].ventilador == "sim") cout << "Ventilador ";
            cout << endl;
            encontrou = true;
        }
    }
    
    if(!encontrou) {
        cout << "Nenhum imovel encontrado com essas caracteristicas!" << endl;
    }
    pausar();
}

void buscarPorQuartosSuites() {
    limparTela();
    if(imoveis.empty()) {
        cout << "Nenhum imovel cadastrado!" << endl;
        pausar();
        return;
    }
    
    cout << "=== BUSCAR POR QUARTOS E SUITES ===" << endl;
    int minQuartos, minSuites;
    
    cout << "Numero minimo de quartos: ";
    cin >> minQuartos;
    cout << "Numero minimo de suites: ";
    cin >> minSuites;
    
    cout << "\nImoveis encontrados:" << endl;
    bool encontrou = false;
    
    for(size_t i = 0; i < imoveis.size(); i++) {
        if(imoveis[i].quartos >= minQuartos && imoveis[i].suites >= minSuites) {
            cout << "\n--- Imovel " << (i+1) << " ---" << endl;
            cout << "Tipo: " << imoveis[i].tipo << endl;
            
            string endereco = imoveis[i].endereco;
            replace(endereco.begin(), endereco.end(), '_', ' ');
            cout << "Endereco: " << endereco << endl;
            
            cout << "Valor: R$ " << imoveis[i].valor << endl;
            cout << "Quartos: " << imoveis[i].quartos << " | Suites: " << imoveis[i].suites << endl;
            encontrou = true;
        }
    }
    
    if(!encontrou) {
        cout << "Nenhum imovel encontrado com esses criterios!" << endl;
    }
    pausar();
}


void relatorioEstatisticas() {
    limparTela();
    if(imoveis.empty()) {
        cout << "Nenhum imovel cadastrado!" << endl;
        pausar();
        return;
    }
    
    cout << "=== RELATORIO DE ESTATISTICAS ===" << endl;
    
    // Contadores
    int totalImoveis = imoveis.size();
    int vendas = 0, alugueis = 0, temporadas = 0;
    int casas = 0, casasComSuites = 0;
    int apartamentos = 0, comerciais = 0, terrenos = 0, galpoes = 0;
    int comArmarios = 0, comAr = 0;
    int comerciaisComCeramica = 0; // Novo contador
    
    for(const auto& imovel : imoveis) {
        // Finalidades
        if(imovel.finalidade == "venda") vendas++;
        else if(imovel.finalidade == "aluguel") alugueis++;
        else if(imovel.finalidade == "temporada") temporadas++;
        
        // Tipos
        if(imovel.tipo == "casa") {
            casas++;
            if(imovel.suites > 0) casasComSuites++;
        }
        else if(imovel.tipo == "apartamento") apartamentos++;
        else if(imovel.tipo == "sala_comercial") {
            comerciais++;
            if (imovel.piso == "ceramica" || imovel.piso == "cerâmica") {
                comerciaisComCeramica++;
            }
        }
        else if(imovel.tipo == "terreno") terrenos++;
        else if(imovel.tipo == "galpao") galpoes++;
        
        // Equipamentos
        if(imovel.armarios == "sim") comArmarios++;
        if(imovel.arCondicionado == "sim") comAr++;
    }
    
    cout << "\n--- PORCENTAGENS POR FINALIDADE ---" << endl;
    cout << "Vendas: " << fixed << setprecision(1) << (totalImoveis > 0 ? (vendas * 100.0 / totalImoveis) : 0) << "% (" << vendas << " imoveis)" << endl;
    cout << "Alugueis: " << fixed << setprecision(1) << (totalImoveis > 0 ? (alugueis * 100.0 / totalImoveis) : 0) << "% (" << alugueis << " imoveis)" << endl;
    cout << "Temporadas: " << fixed << setprecision(1) << (totalImoveis > 0 ? (temporadas * 100.0 / totalImoveis) : 0) << "% (" << temporadas << " imoveis)" << endl;
    
    cout << "\n--- PORCENTAGENS POR TIPO ---" << endl;
    cout << "Casas: " << fixed << setprecision(1) << (totalImoveis > 0 ? (casas * 100.0 / totalImoveis) : 0) << "% (" << casas << " imoveis)" << endl;
    cout << "Apartamentos: " << fixed << setprecision(1) << (totalImoveis > 0 ? (apartamentos * 100.0 / totalImoveis) : 0) << "% (" << apartamentos << " imoveis)" << endl;
    cout << "Salas Comerciais: " << fixed << setprecision(1) << (totalImoveis > 0 ? (comerciais * 100.0 / totalImoveis) : 0) << "% (" << comerciais << " imoveis)" << endl;
    cout << "Terrenos: " << fixed << setprecision(1) << (totalImoveis > 0 ? (terrenos * 100.0 / totalImoveis) : 0) << "% (" << terrenos << " imoveis)" << endl;
    cout << "Galpoes: " << fixed << setprecision(1) << (totalImoveis > 0 ? (galpoes * 100.0 / totalImoveis) : 0) << "% (" << galpoes << " imoveis)" << endl;
    
    cout << "\n--- ESTATISTICAS ESPECIFICAS ---" << endl;
    cout << "Porcentagem de casas com suites: " << fixed << setprecision(1) << (casas > 0 ? (casasComSuites * 100.0 / casas) : 0) << "% (" << casasComSuites << " de " << casas << " casas)" << endl;
    cout << "Porcentagem de salas comerciais com piso de ceramica: " << fixed << setprecision(1) << (comerciais > 0 ? (comerciaisComCeramica * 100.0 / comerciais) : 0) << "% (" << comerciaisComCeramica << " de " << comerciais << " salas)" << endl;

    cout << "\n--- EQUIPAMENTOS ---" << endl;
    cout << "Com armarios: " << fixed << setprecision(1) << (totalImoveis > 0 ? (comArmarios * 100.0 / totalImoveis) : 0) << "% (" << comArmarios << " imoveis)" << endl;
    cout << "Com ar-condicionado: " << fixed << setprecision(1) << (totalImoveis > 0 ? (comAr * 100.0 / totalImoveis) : 0) << "% (" << comAr << " imoveis)" << endl;
    
    cout << "\n--- RESUMO GERAL ---" << endl;
    cout << "Total de imoveis: " << totalImoveis << "/" << MAX_REGISTROS << endl;
    cout << "Registros disponiveis: " << (MAX_REGISTROS - totalImoveis) << endl;
    
    pausar();
}

void excluirImovel() {
    limparTela();
    if(imoveis.empty()) {
        cout << "Nenhum imovel cadastrado!" << endl;
        pausar();
        return;
    }
    
    cout << "=== EXCLUIR IMOVEL ===" << endl;
    
    for(size_t i = 0; i < imoveis.size(); i++) {
        string endereco = imoveis[i].endereco;
        replace(endereco.begin(), endereco.end(), '_', ' ');
        
        cout << i+1 << ". " << imoveis[i].tipo << " - " 
             << endereco << " - R$ " 
             << imoveis[i].valor << endl;
    }
    
    int numero;
    cout << "\nDigite o numero do imovel para excluir (0 para cancelar): ";
    cin >> numero;
    
    if(numero == 0) {
        cout << "Operacao cancelada!" << endl;
    }
    else if(numero < 1 || numero > (int)imoveis.size()) {
        cout << "Numero invalido!" << endl;
    }
    else {
        imoveis.erase(imoveis.begin() + numero - 1);
        cout << "Imovel excluido com sucesso!" << endl;
        cout << "Total de imoveis: " << imoveis.size() << "/" << MAX_REGISTROS << endl;
    }
    pausar();
}

void salvarArquivo() {
    ofstream arquivo(ARQUIVO);
    if(!arquivo.is_open()) {
        cerr << "ERRO FATAL: Nao foi possivel abrir o arquivo " << ARQUIVO << " para escrita." << endl;
        return;
    }
    
    // Escrever dados
    for(const auto& imovel : imoveis) {
        arquivo << imovel.tipo << " "
                << imovel.finalidade << " "
                << imovel.endereco << " "
                << imovel.bairro << " "
                << imovel.cidade << " "
                << imovel.area << " "
                << imovel.valor << " "
                << imovel.iptu << " "
                << imovel.quartos << " "
                << imovel.suites << " "
                << imovel.banheiros << " "
                << imovel.vagas << " "
                << imovel.cozinha << " "
                << imovel.sala << " "
                << imovel.varanda << " "
                << imovel.areaServico << " "
                << imovel.piso << " "
                << imovel.conservacao << " "
                << imovel.armarios << " "
                << imovel.arCondicionado << " "
                << imovel.aquecedor << " "
                << imovel.ventilador << endl;
    }
    
    // Adicionar a linha "fim" no final do arquivo para manter a compatibilidade
    arquivo << "fim" << endl;
    
    arquivo.close();
}

void carregarArquivo() {
    ifstream arquivo(ARQUIVO);
    if(!arquivo.is_open()) {
        cerr << "AVISO: Arquivo " << ARQUIVO << " nao encontrado. Iniciando sistema vazio." << endl;
        return;
    }
    
    cout << "Carregando dados do arquivo " << ARQUIVO << "..." << endl;
    imoveis.clear(); // Limpa dados antigos antes de carregar
    string linha;
    int linhaAtual = 0;
    
    while(getline(arquivo, linha) && imoveis.size() < MAX_REGISTROS) {
        linhaAtual++;
        
        // Pular linha de comentário ou vazia
        if(linha.empty() || linha[0] == '#') {
            continue;
        }
        
        // NOVO: Parar a leitura se encontrar a palavra "fim"
        if (linha.find("fim") != string::npos) {
            break;
        }
        
        istringstream iss(linha);
        Imovel imovel;
        
        // O campo 'valor' é lido como string para aceitar "300/dia"
        if(iss >> imovel.tipo >> imovel.finalidade >> imovel.endereco 
           >> imovel.bairro >> imovel.cidade >> imovel.area >> imovel.valor 
           >> imovel.iptu >> imovel.quartos >> imovel.suites >> imovel.banheiros 
           >> imovel.vagas >> imovel.cozinha >> imovel.sala >> imovel.varanda 
           >> imovel.areaServico >> imovel.piso >> imovel.conservacao 
           >> imovel.armarios >> imovel.arCondicionado >> imovel.aquecedor 
           >> imovel.ventilador) {
            
            imoveis.push_back(imovel);
        } else {
            // Não exibe erro para linhas vazias ou malformadas para não poluir a tela
            // cout << "Aviso: Nao foi possivel ler a linha " << linhaAtual << " do arquivo." << endl;
        }
    }
    
    arquivo.close();
    cout << "Carregados " << imoveis.size() << " imoveis do arquivo." << endl;
    
    if(imoveis.size() >= MAX_REGISTROS) {
        cout << "AVISO: Limite maximo de " << MAX_REGISTROS << " registros atingido!" << endl;
    }
}

void limparTela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pausar() {
    cout << "\nPressione ENTER para continuar...";
    // Limpa o buffer de entrada para garantir que a pausa funcione corretamente
    // depois de operações como 'cin >> var;'.
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}
