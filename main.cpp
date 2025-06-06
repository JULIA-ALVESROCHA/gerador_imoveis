/** * AEDs I – Prática (DCE794) 
 * Júlia Alves da Rocha - 2025.1.08.015
 * @details Este projeto gerencia um cadastro de imóveis, permitindo
 * inclusão, busca, exclusão, listagem ordenada e relatórios.
 * Melhorias incluem tratamento de erro aprimorado e ordenação de listas.
 */

#include <iostream> // manipulação de entradas e saídas
#include <vector> // Permite a utilização de "vetor inteligente"
#include <fstream> // manipulação de leitura e escrita
#include <string> 
#include <sstream> // ler os dados de uma string como se fosse uma arquivo
#include <iomanip>   // Para manipulações de saída, como std::setprecision (definir casas decimais).
#include <limits>    // Para acessar informações sobre tipos de dados, como o tamanho máximo de um stream.
#include <algorithm> // Para usar algoritmos prontos, como o std::sort para ordenação.
#include <cctype>    // Para funções de caracteres, como std::tolower (converter para minúsculo).

// --- BIBLIOTECA ESPECIAL PARA CORREÇÃO DE CARACTERES NO WINDOWS ---
#ifdef _WIN32
#include <windows.h>
#endif
// ----------------------------------------------------------------

// --- Constantes Globais ---
const int MAX_IMOVEIS = 200; 
const std::string NOME_ARQUIVO = "BD_Imoveis2.txt";

// --- Estrutura de Dados ---
struct Imovel {
    std::string tipo;
    std::string finalidade;
    std::string endereco;
    std::string bairro;
    std::string cidade;
    double area;
    double valor;
    double iptu;
    int quartos;
    int suites;
    int banheiros;
    int vagas;
    int cozinha;
    int sala;
    int varanda;
    int area_servico;
    std::string piso;
    std::string conservacao;
    int armarios;
    int ar_condicionado;
    int aquecedor;
    int ventilador;
};

// --- Protótipos das Funções ---
void exibirMenu();
void carregarDados(std::vector<Imovel> &imoveis);
void salvarDados(const std::vector<Imovel> &imoveis);
void incluirImovel(std::vector<Imovel> &imoveis);
void buscarExcluirImovel(std::vector<Imovel> &imoveis);
void buscarPorValor(const std::vector<Imovel> &imoveis);
void buscarPorCaracteristicas(const std::vector<Imovel> &imoveis);
void buscarPorQuartosSuites(const std::vector<Imovel> &imoveis);
void gerarRelatorioEstatisticas(const std::vector<Imovel> &imoveis);
void listarTodosImoveis(std::vector<Imovel> &imoveis);
void imprimirDetalhesImovel(const Imovel& imovel);
void exportarParaTxt(const std::vector<Imovel>& imoveis, const std::string& nomeArquivo);

// Tratamento de erro: Necessário quando o comportamento do usuário possa influenciar o sistema.
template <typename T> // mostra que é um templete genérico a ser usado no programa
void lerEntradaNumerica(const std::string& prompt, T& valor); // por meio do - T& valor - transforma a entrada no tipo do vetor valor.

// --- Função Principal ---
int main() {
    // --- CÓDIGO PARA CORRIGIR ACENTUAÇÃO NO CONSOLE DO WINDOWS ---
    #ifdef _WIN32
    // Define a página de código de saída do console para UTF-8
    SetConsoleOutputCP(CP_UTF8);
    // Define a página de código de entrada do console para UTF-8
    SetConsoleCP(CP_UTF8);
    #endif
    // -----------------------------------------------------------

    std::vector<Imovel> baseDeImoveis;
    carregarDados(baseDeImoveis);

    int opcao;
    do {
        exibirMenu();
        std::cin >> opcao;

        while(std::cin.fail()) {
            std::cout << "Entrada inválida. Por favor, insira um número." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            exibirMenu();
            std::cin >> opcao;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (opcao) {
            case 1: incluirImovel(baseDeImoveis); break;
            case 2: buscarExcluirImovel(baseDeImoveis); break;
            case 3: buscarPorValor(baseDeImoveis); break;
            case 4: buscarPorCaracteristicas(baseDeImoveis); break;
            case 5: buscarPorQuartosSuites(baseDeImoveis); break;
            case 6: gerarRelatorioEstatisticas(baseDeImoveis); break;
            case 7: listarTodosImoveis(baseDeImoveis); break;
            case 0:
                std::cout << "Salvando dados e encerrando o programa..." << std::endl;
                salvarDados(baseDeImoveis);
                break;
            default:
                std::cout << "Opção inválida! Tente novamente." << std::endl;
                break;
        }
        if (opcao != 0) {
            std::cout << "\nPressione Enter para continuar...";
            std::cin.get();
        }

    } while (opcao != 0);

    return 0;
}

// --- Implementação das Funções (Corpo) ---

void exibirMenu() {
    // Comando para limpar a tela de forma compatível com Windows e outros sistemas
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    std::cout << "========================================" << std::endl;
    std::cout << "      Sistema de Gestão de Imóveis      " << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "1. Incluir Imóvel" << std::endl;
    std::cout << "2. Buscar e Excluir Imóvel por Rua" << std::endl;
    std::cout << "3. Buscar Imóveis por Faixa de Valor" << std::endl;
    std::cout << "4. Busca Avançada por Características" << std::endl;
    std::cout << "5. Buscar por Nº de Quartos e Suítes" << std::endl;
    std::cout << "6. Relatório de Estatísticas" << std::endl;
    std::cout << "7. Listar Todos os Imóveis (com ordenação)" << std::endl;
    std::cout << "0. Sair e Salvar" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Escolha uma opção: ";
}

template <typename T>
void lerEntradaNumerica(const std::string& prompt, T& valor) {
    while (true) {
        std::cout << prompt;
        std::cin >> valor;
        if (std::cin.good()) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
        std::cout << "Entrada inválida. Por favor, insira um valor numérico." << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

void incluirImovel(std::vector<Imovel> &imoveis) {
    std::cout << "--- Inclusão de Novo Imóvel ---" << std::endl;
    if (imoveis.size() >= MAX_IMOVEIS) {
        std::cout << "Erro: A base de dados atingiu o limite máximo de " << MAX_IMOVEIS << " imóveis." << std::endl;
        return;
    }

    Imovel novo;
    std::cout << "Tipo (ex: casa, apartamento): "; 
    std::cin >> novo.tipo;
    std::cout << "Finalidade (ex: venda, aluguel): "; 
    std::cin >> novo.finalidade;
    std::cout << "Endereço (sem espaços, ex: Rua_Principal): "; 
    std::cin >> novo.endereco;
    std::cout << "Bairro: "; 
    std::cin >> novo.bairro;
    std::cout << "Cidade: "; 
    std::cin >> novo.cidade;
    
    // Usando a função de leitura segura para números
    lerEntradaNumerica("Área (m2): ", novo.area);
    lerEntradaNumerica("Valor (R$): ", novo.valor);
    lerEntradaNumerica("IPTU (R$): ", novo.iptu);
    lerEntradaNumerica("Quartos: ", novo.quartos);
    lerEntradaNumerica("Suítes: ", novo.suites);
    lerEntradaNumerica("Banheiros: ", novo.banheiros);
    lerEntradaNumerica("Vagas de garagem: ", novo.vagas);
    lerEntradaNumerica("Tem cozinha (1=sim/0=nao): ", novo.cozinha);
    lerEntradaNumerica("Tem sala (1=sim/0=nao): ", novo.sala);
    lerEntradaNumerica("Tem varanda (1=sim/0=nao): ", novo.varanda);
    lerEntradaNumerica("Tem área de serviço (1=sim/0=nao): ", novo.area_servico);
    
    std::cout << "Tipo de piso (ex: ceramica): "; 
    std::cin >> novo.piso;
    std::cout << "Conservação (ex: novo, bom): "; 
    std::cin >> novo.conservacao;
    
    lerEntradaNumerica("Tem armários (1=sim/0=nao): ", novo.armarios);
    lerEntradaNumerica("Tem ar-condicionado (1=sim/0=nao): ", novo.ar_condicionado);
    lerEntradaNumerica("Tem aquecedor (1=sim/0=nao): ", novo.aquecedor);
    lerEntradaNumerica("Tem ventilador (1=sim/0=nao): ", novo.ventilador);

    imoveis.push_back(novo);
    std::cout << "\nImóvel incluído com sucesso!" << std::endl;
}

void listarTodosImoveis(std::vector<Imovel>& imoveis) {
    std::cout << "--- Listagem de Todos os Imóveis ---" << std::endl;
    if (imoveis.empty()) {
        std::cout << "Nenhum imóvel cadastrado." << std::endl;
        return;
    }

    std::vector<Imovel> imoveisParaListar = imoveis; // Copia para não alterar a original
    int escolha_ordem;
    
    std::cout << "\nComo deseja ordenar a lista?" << std::endl;
    std::cout << "1. Por Preço (menor para maior)" << std::endl;
    std::cout << "2. Por Preço (maior para menor)" << std::endl;
    std::cout << "3. Por Área (menor para maior)" << std::endl;
    std::cout << "4. Por Área (maior para menor)" << std::endl;
    std::cout << "0. Ordem padrão (sem ordenação)" << std::endl;
    std::cout << "Escolha uma opção: ";
    std::cin >> escolha_ordem;
    std::cin.ignore();

    switch(escolha_ordem) {
        case 1: // Preço crescente
            std::sort(imoveisParaListar.begin(), imoveisParaListar.end(), 
                [](const Imovel& a, const Imovel& b) { return a.valor < b.valor; });
            std::cout << "Lista ordenada por PREÇO CRESCENTE." << std::endl;
            break;
        case 2: // Preço decrescente
            std::sort(imoveisParaListar.begin(), imoveisParaListar.end(), 
                [](const Imovel& a, const Imovel& b) { return a.valor > b.valor; });
            std::cout << "Lista ordenada por PREÇO DECRESCENTE." << std::endl;
            break;
        case 3: // Área crescente
            std::sort(imoveisParaListar.begin(), imoveisParaListar.end(), 
                [](const Imovel& a, const Imovel& b) { return a.area < b.area; });
            std::cout << "Lista ordenada por ÁREA CRESCENTE." << std::endl;
            break;
        case 4: // Área decrescente
            std::sort(imoveisParaListar.begin(), imoveisParaListar.end(), 
                [](const Imovel& a, const Imovel& b) { return a.area > b.area; });
            std::cout << "Lista ordenada por ÁREA DECRESCENTE." << std::endl;
            break;
        case 0:
        default:
            std::cout << "Lista na ORDEM PADRÃO." << std::endl;
            break;
    }

    for (const auto& imovel : imoveisParaListar) {
        imprimirDetalhesImovel(imovel);
    }
    
    std::cout << "\nDeseja exportar esta lista para um arquivo TXT? (s/n): ";
    char escolha_export;
    std::cin >> escolha_export;
    std::cin.ignore();

    if (std::tolower(escolha_export) == 's') {
        std::string nomeArquivoExport;
        std::cout << "Digite o nome do arquivo para exportação (ex: relatorio.txt): ";
        getline(std::cin, nomeArquivoExport);
        exportarParaTxt(imoveisParaListar, nomeArquivoExport);
    }
}

void exportarParaTxt(const std::vector<Imovel>& imoveis, const std::string& nomeArquivo) {
    std::ofstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        std::cerr << "Erro: Não foi possível criar o arquivo de exportação." << std::endl;
        return;
    }

    arquivo << "========================================" << std::endl;
    arquivo << "      Relatório de Imóveis Cadastrados   " << std::endl;
    arquivo << "========================================" << std::endl;
    
    arquivo << std::fixed << std::setprecision(2);

    for (const auto& imovel : imoveis) {
        arquivo << "----------------------------------------" << std::endl;
        arquivo << "Tipo: " << imovel.tipo << " | Finalidade: " << imovel.finalidade << std::endl;
        arquivo << "Endereço: " << imovel.endereco << ", " << imovel.bairro << ", " << imovel.cidade << std::endl;
        arquivo << "Área: " << imovel.area << "m² | Valor: R$ " << imovel.valor << " | IPTU: R$ " << imovel.iptu << std::endl;
        arquivo << "Quartos: " << imovel.quartos << " | Suítes: " << imovel.suites << " | Banheiros: " << imovel.banheiros << " | Vagas: " << imovel.vagas << std::endl;
        arquivo << "Piso: " << imovel.piso << " | Conservação: " << imovel.conservacao << std::endl;
        arquivo << "Extras: Cozinha(" << imovel.cozinha << "), Sala(" << imovel.sala << "), Varanda(" << imovel.varanda 
                << "), A. Serviço(" << imovel.area_servico << "), Armários(" << imovel.armarios << "), Ar Cond.(" << imovel.ar_condicionado 
                << "), Aquecedor(" << imovel.aquecedor << "), Ventilador(" << imovel.ventilador << ")" << std::endl;
    }
    arquivo << "----------------------------------------" << std::endl;
    arquivo.close();

    std::cout << "Lista de imóveis exportada com sucesso para '" << nomeArquivo << "'!" << std::endl;
}

void carregarDados(std::vector<Imovel> &imoveis) {
    std::ifstream arquivo(NOME_ARQUIVO);
    if (!arquivo.is_open()) {
        std::cout << "Aviso: Arquivo '" << NOME_ARQUIVO << "' não encontrado. Um novo será criado ao sair." << std::endl;
        return;
    }

    std::string linha;
    std::getline(arquivo, linha); // Pula a primeira linha (cabeçalho)

    while (std::getline(arquivo, linha) && !linha.empty()) {
        std::stringstream ss(linha);
        Imovel imovel;

        ss >> imovel.tipo;
        if (imovel.tipo == "fim") break;

        ss >> imovel.finalidade >> imovel.endereco >> imovel.bairro >> imovel.cidade
           >> imovel.area >> imovel.valor >> imovel.iptu >> imovel.quartos
           >> imovel.suites >> imovel.banheiros >> imovel.vagas >> imovel.cozinha
           >> imovel.sala >> imovel.varanda >> imovel.area_servico >> imovel.piso
           >> imovel.conservacao >> imovel.armarios >> imovel.ar_condicionado
           >> imovel.aquecedor >> imovel.ventilador;
        
        imoveis.push_back(imovel);
    }
    arquivo.close();
    std::cout << "Dados carregados com sucesso. Total de " << imoveis.size() << " imóveis." << std::endl;
}

void salvarDados(const std::vector<Imovel> &imoveis) {
    std::ofstream arquivo(NOME_ARQUIVO);
    if (!arquivo.is_open()) {
        std::cerr << "Erro fatal: Não foi possível abrir o arquivo '" << NOME_ARQUIVO << "' para escrita." << std::endl;
        return;
    }

    arquivo << "Tipo Finalidade Endereco Bairro Cidade Area Valor IPTU Quartos Suites Banheiros Vagas Cozinha Sala Varanda Area_de_Servico Piso Conservacao Armarios Ar_Condicionado Aquecedor Ventilador\n";
    
    arquivo << std::fixed << std::setprecision(2);

    for (const auto& imovel : imoveis) {
        arquivo << imovel.tipo << " " << imovel.finalidade << " " << imovel.endereco << " "
                << imovel.bairro << " " << imovel.cidade << " " << imovel.area << " "
                << imovel.valor << " " << imovel.iptu << " " << imovel.quartos << " "
                << imovel.suites << " " << imovel.banheiros << " " << imovel.vagas << " "
                << imovel.cozinha << " " << imovel.sala << " " << imovel.varanda << " "
                << imovel.area_servico << " " << imovel.piso << " " << imovel.conservacao << " "
                << imovel.armarios << " " << imovel.ar_condicionado << " "
                << imovel.aquecedor << " " << imovel.ventilador << "\n";
    }
    arquivo << "fim\n";
    arquivo.close();
}

void imprimirDetalhesImovel(const Imovel& imovel) {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Tipo: " << imovel.tipo << " | Finalidade: " << imovel.finalidade << std::endl;
    std::cout << "Endereço: " << imovel.endereco << ", " << imovel.bairro << ", " << imovel.cidade << std::endl;
    std::cout << "Área: " << std::fixed << std::setprecision(2) << imovel.area << "m² | Valor: R$ " << imovel.valor << " | IPTU: R$ " << imovel.iptu << std::endl;
    std::cout << "Quartos: " << imovel.quartos << " | Suítes: " << imovel.suites << " | Banheiros: " << imovel.banheiros << " | Vagas: " << imovel.vagas << std::endl;
    std::cout << "Piso: " << imovel.piso << " | Conservação: " << imovel.conservacao << std::endl;
    std::cout << "Extras: Cozinha(" << imovel.cozinha << "), Sala(" << imovel.sala << "), Varanda(" << imovel.varanda 
              << "), A. Serviço(" << imovel.area_servico << "), Armários(" << imovel.armarios << "), Ar Cond.(" << imovel.ar_condicionado 
              << "), Aquecedor(" << imovel.aquecedor << "), Ventilador(" << imovel.ventilador << ")" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
}

void buscarExcluirImovel(std::vector<Imovel>& imoveis) {
    std::cout << "--- Buscar e Excluir Imóvel por Rua ---" << std::endl;
    std::cout << "Digite o nome da rua para busca (ex: Rua_Principal): ";
    std::string rua;
    std::cin >> rua;

    std::vector<size_t> indicesEncontrados;
    for (size_t i = 0; i < imoveis.size(); ++i) {
        if (imoveis[i].endereco.find(rua) != std::string::npos) {
            indicesEncontrados.push_back(i);
        }
    }

    if (indicesEncontrados.empty()) {
        std::cout << "Nenhum imóvel encontrado no endereço '" << rua << "'." << std::endl;
    } else {
        std::cout << "\nForam encontrados " << indicesEncontrados.size() << " imóvel(is). Digite o índice para selecionar ou -1 para cancelar:" << std::endl;
        for(size_t i = 0; i < indicesEncontrados.size(); ++i) {
            std::cout << "\n--- Opção " << i << " ---";
            imprimirDetalhesImovel(imoveis[indicesEncontrados[i]]);
        }
        
        int escolha;
        std::cout << "Escolha a opção para excluir (-1 para cancelar): ";
        std::cin >> escolha;

        if (escolha >= 0 && static_cast<size_t>(escolha) < indicesEncontrados.size()) {
            char confirmacao;
            std::cout << "Tem certeza que deseja excluir este imóvel? (s/n): ";
            std::cin >> confirmacao;
            if (std::tolower(confirmacao) == 's') {
                imoveis.erase(imoveis.begin() + indicesEncontrados[escolha]);
                std::cout << "Imóvel excluído com sucesso!" << std::endl;
            } else {
                std::cout << "Exclusão cancelada." << std::endl;
            }
        } else {
            std::cout << "Seleção inválida ou cancelada." << std::endl;
        }
    }
}

void buscarPorValor(const std::vector<Imovel> &imoveis) {
    std::cout << "--- Buscar por Faixa de Valor (Aluguel/Temporada) ---" << std::endl;
    double minValor, maxValor;
    lerEntradaNumerica("Digite o valor mínimo de locação: ", minValor);
    lerEntradaNumerica("Digite o valor máximo de locação: ", maxValor);

    int encontrados = 0;
    for (const auto& imovel : imoveis) {
        bool ehLocacao = (imovel.finalidade == "aluguel" || imovel.finalidade == "temporada" || imovel.finalidade == "locacao");
        if (ehLocacao && imovel.valor >= minValor && imovel.valor <= maxValor) {
            imprimirDetalhesImovel(imovel);
            encontrados++;
        }
    }

    if (encontrados == 0) {
        std::cout << "Nenhum imóvel para locação encontrado nessa faixa de valor." << std::endl;
    } else {
        std::cout << "Total de " << encontrados << " imóveis encontrados." << std::endl;
    }
}

void buscarPorCaracteristicas(const std::vector<Imovel>& imoveis) {
    std::cout << "--- Busca Avançada por Características ---" << std::endl;
    std::cout << "(Pressione Enter para ignorar um critério)" << std::endl;

    Imovel filtro;
    int filtroQuartos, filtroSuites, filtroVagas;
    
    std::cout << "Tipo (casa, apartamento...): "; 
    std::getline(std::cin, filtro.tipo);
    std::cout << "Finalidade (venda, aluguel...): "; 
    std::getline(std::cin, filtro.finalidade);
    std::cout << "Bairro: "; 
    std::getline(std::cin, filtro.bairro);
    std::cout << "Cidade: "; 
    std::getline(std::cin, filtro.cidade);
    
    std::string temp;
    std::cout << "Nº mínimo de Quartos: ";
    std::getline(std::cin, temp);
    filtroQuartos = temp.empty() ? -1 : std::stoi(temp);

    std::cout << "Nº mínimo de Suítes: ";
    std::getline(std::cin, temp);
    filtroSuites = temp.empty() ? -1 : std::stoi(temp);

    std::cout << "Nº mínimo de Vagas: ";
    std::getline(std::cin, temp);
    filtroVagas = temp.empty() ? -1 : std::stoi(temp);

    int encontrados = 0;
    std::cout << "\n--- Resultados da Busca ---" << std::endl;
    for (const auto& imovel : imoveis) {
        bool match = true;
        if (!filtro.tipo.empty() && imovel.tipo != filtro.tipo) match = false;
        if (!filtro.finalidade.empty() && imovel.finalidade != filtro.finalidade) match = false;
        if (!filtro.bairro.empty() && imovel.bairro != filtro.bairro) match = false;
        if (!filtro.cidade.empty() && imovel.cidade != filtro.cidade) match = false;
        if (filtroQuartos != -1 && imovel.quartos < filtroQuartos) match = false;
        if (filtroSuites != -1 && imovel.suites < filtroSuites) match = false;
        if (filtroVagas != -1 && imovel.vagas < filtroVagas) match = false;

        if (match) {
            imprimirDetalhesImovel(imovel);
            encontrados++;
        }
    }

    if (encontrados == 0) {
        std::cout << "Nenhum imóvel encontrado com os critérios especificados." << std::endl;
    } else {
        std::cout << "Total de " << encontrados << " imóveis encontrados." << std::endl;
    }
}

void buscarPorQuartosSuites(const std::vector<Imovel> &imoveis) {
    std::cout << "--- Buscar por Número de Quartos e Suítes ---" << std::endl;
    int numQuartos, numSuites;
    lerEntradaNumerica("Digite o número exato de quartos: ", numQuartos);
    lerEntradaNumerica("Digite o número exato de suítes: ", numSuites);

    int encontrados = 0;
    for (const auto& imovel : imoveis) {
        if (imovel.quartos == numQuartos && imovel.suites == numSuites) {
            imprimirDetalhesImovel(imovel);
            encontrados++;
        }
    }

    if (encontrados == 0) {
        std::cout << "Nenhum imóvel encontrado com essa configuração." << std::endl;
    } else {
        std::cout << "Total de " << encontrados << " imóveis encontrados." << std::endl;
    }
}

void gerarRelatorioEstatisticas(const std::vector<Imovel> &imoveis) {
    if (imoveis.empty()) {
        std::cout << "Não há imóveis cadastrados para gerar estatísticas." << std::endl;
        return;
    }

    std::cout << "--- Relatório de Estatísticas ---" << std::endl;
    double total = static_cast<double>(imoveis.size());
    
    int countVenda = 0, countAluguel = 0, countTemporada = 0;
    int countCasasComSuite = 0;
    int countSalasComerciais = 0, countSalasCeramica = 0;
    
    // Variáveis para estatísticas de valor e área
    double somaValores = 0.0, somaAreas = 0.0;
    double menorValor = imoveis[0].valor, maiorValor = imoveis[0].valor;
    double menorArea = imoveis[0].area, maiorArea = imoveis[0].area;

    for (const auto& imovel : imoveis) {
        // Contagem por finalidade
        if (imovel.finalidade == "venda") countVenda++;
        else if (imovel.finalidade == "aluguel") countAluguel++;
        else if (imovel.finalidade == "temporada") countTemporada++;
        
        // Casas com suíte
        if (imovel.tipo == "casa" && imovel.suites > 0) countCasasComSuite++;
        
        // Salas comerciais
        if (imovel.tipo == "sala_comercial") {
            countSalasComerciais++;
            if (imovel.piso == "ceramica") countSalasCeramica++;
        }
        
        // Estatísticas de valor e área
        somaValores += imovel.valor;
        somaAreas += imovel.area;
        
        if (imovel.valor < menorValor) menorValor = imovel.valor;
        if (imovel.valor > maiorValor) maiorValor = imovel.valor;
        if (imovel.area < menorArea) menorArea = imovel.area;
        if (imovel.area > maiorArea) maiorArea = imovel.area;
    }

    std::cout << std::fixed << std::setprecision(1);
    std::cout << "\n1. Porcentagem por Finalidade:" << std::endl;
    std::cout << "   - Venda: " << (countVenda / total) * 100.0 << "%" << std::endl;
    std::cout << "   - Aluguel: " << (countAluguel / total) * 100.0 << "%" << std::endl;
    std::cout << "   - Temporada: " << (countTemporada / total) * 100.0 << "%" << std::endl;
    
    std::cout << "\n2. Casas com pelo menos uma suíte: " << countCasasComSuite << " imóveis" << std::endl;
    
    std::cout << "\n3. Salas comerciais:" << std::endl;
    std::cout << "   - Total: " << countSalasComerciais << " imóveis" << std::endl;
    if (countSalasComerciais > 0) {
        std::cout << "   - Com piso cerâmico: " << countSalasCeramica << " (" 
                  << (static_cast<double>(countSalasCeramica) / countSalasComerciais) * 100.0 << "%)" << std::endl;
    }
    
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n4. Estatísticas de Valores:" << std::endl;
    std::cout << "   - Valor médio: R$ " << (somaValores / total) << std::endl;
    std::cout << "   - Menor valor: R$ " << menorValor << std::endl;
    std::cout << "   - Maior valor: R$ " << maiorValor << std::endl;
    
    std::cout << "\n5. Estatísticas de Área:" << std::endl;
    std::cout << "   - Área média: " << (somaAreas / total) << " m²" << std::endl;
    std::cout << "   - Menor área: " << menorArea << " m²" << std::endl;
    std::cout << "   - Maior área: " << maiorArea << " m²" << std::endl;
    
    std::cout << "\n6. Total de imóveis cadastrados: " << static_cast<int>(total) << std::endl;
    
    // Opção para exportar relatório
    std::cout << "\nDeseja exportar este relatório para um arquivo TXT? (s/n): ";
    char escolha;
    std::cin >> escolha;
    std::cin.ignore();
    
    if (std::tolower(escolha) == 's') {
        std::string nomeArquivo;
        std::cout << "Digite o nome do arquivo (ex: relatorio_estatisticas.txt): ";
        std::getline(std::cin, nomeArquivo);
        
        std::ofstream arquivo(nomeArquivo);
        if (arquivo.is_open()) {
            arquivo << "========================================" << std::endl;
            arquivo << "      RELATÓRIO DE ESTATÍSTICAS - IMÓVEIS   " << std::endl;
            arquivo << "========================================" << std::endl;
            
            arquivo << std::fixed << std::setprecision(1);
            arquivo << "\n1. Porcentagem por Finalidade:" << std::endl;
            arquivo << "   - Venda: " << (countVenda / total) * 100.0 << "%" << std::endl;
            arquivo << "   - Aluguel: " << (countAluguel / total) * 100.0 << "%" << std::endl;
            arquivo << "   - Temporada: " << (countTemporada / total) * 100.0 << "%" << std::endl;
            
            arquivo << "\n2. Casas com pelo menos uma suíte: " << countCasasComSuite << " imóveis" << std::endl;
            
            arquivo << "\n3. Salas comerciais:" << std::endl;
            arquivo << "   - Total: " << countSalasComerciais << " imóveis" << std::endl;
            if (countSalasComerciais > 0) {
                arquivo << "   - Com piso cerâmico: " << countSalasCeramica << " (" 
                        << (static_cast<double>(countSalasCeramica) / countSalasComerciais) * 100.0 << "%)" << std::endl;
            }
            
            arquivo << std::fixed << std::setprecision(2);
            arquivo << "\n4. Estatísticas de Valores:" << std::endl;
            arquivo << "   - Valor médio: R$ " << (somaValores / total) << std::endl;
            arquivo << "   - Menor valor: R$ " << menorValor << std::endl;
            arquivo << "   - Maior valor: R$ " << maiorValor << std::endl;
            
            arquivo << "\n5. Estatísticas de Área:" << std::endl;
            arquivo << "   - Área média: " << (somaAreas / total) << " m²" << std::endl;
            arquivo << "   - Menor área: " << menorArea << " m²" << std::endl;
            arquivo << "   - Maior área: " << maiorArea << " m²" << std::endl;
            
            arquivo << "\n6. Total de imóveis cadastrados: " << static_cast<int>(total) << std::endl;
            arquivo << "\n========================================" << std::endl;
            
            arquivo.close();
            std::cout << "Relatório exportado com sucesso para '" << nomeArquivo << "'!" << std::endl;
        } else {
            std::cout << "Erro: Não foi possível criar o arquivo de relatório." << std::endl;
        }
    }
}
