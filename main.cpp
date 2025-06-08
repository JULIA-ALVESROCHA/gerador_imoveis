/** * AEDs I – Prática (DCE794) 
 * Júlia Alves da Rocha - 2025.1.08.015
 * @details Esta versão corrige todos os problemas críticos de validação, fluxo de execução
 * e codificação de caracteres. Implementa validação robusta, controle de fluxo adequado
 * e tratamento completo de erros de entrada, com uma estrutura de dados mais eficiente (CSV).
 */

// --- BIBLIOTECAS ---

#include <iostream>
#include <vector>        // Possibilita o uso de um "vetor inteligente"
#include <fstream>       // manipulação de arquivos - ler e escrever
#include <string>        
#include <sstream>       // Para ler dados de uma string como se fosse um fluxo (stringstream).
#include <iomanip>       // Para formatar a saída, como definir casas decimais (setprecision).
#include <limits>        
#include <algorithm>     // Para usar algoritmos como std::sort e std::transform.
#include <cctype>        // Para funções de manipulação de caracteres, como std::tolower.
#include <clocale>       // Para setlocale, importante para a configuração de idioma.

// --- BIBLIOTECAS ESPECÍFICAS DO WINDOWS PARA CORRIGIR CARACTERES ---
#ifdef _WIN32
#include <windows.h>
#endif


// --- CONSTANTES GLOBAIS ---
const int MAX_IMOVEIS = 200;
const std::string NOME_ARQUIVO = "BD_Imoveis.csv"; 


// --- ESTRUTURA DE DADOS --- 
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


// --- PROTÓTIPOS DAS FUNÇÕES ---
void configurarConsole();
void exibirMenu();
void pausarParaContinuar();
void pausarComMensagem(const std::string& mensagem);
void limparBuffer();
void carregarDados(std::vector<Imovel> &imoveis);
void salvarDados(const std::vector<Imovel> &imoveis);
void incluirImovel(std::vector<Imovel> &imoveis);
void buscarExcluirImovel(std::vector<Imovel> &imoveis);
void buscarPorValor(const std::vector<Imovel> &imoveis);
void buscarPorCaracteristicas(const std::vector<Imovel> &imoveis);
void buscarPorQuartosSuites(const std::vector<Imovel> &imoveis);
void gerarRelatorioEstatisticas(const std::vector<Imovel> &imoveis);
void listarTodosImoveis(std::vector<Imovel> &imoveis);
void imprimirDetalhesImovel(const Imovel& imovel, int indice = -1);
void exportarParaTxt(const std::vector<Imovel>& imoveis, const std::string& nomeArquivo);

// Funções de validação de entrada - Para melhorar o tratamento de erros.
int lerOpcaoMenu(int minimo, int maximo);
int lerEntradaBinaria(const std::string& prompt);
std::string lerStringValida(const std::string& prompt, const std::vector<std::string>& opcoesValidas);
std::string lerString(const std::string& prompt);
template <typename T>
void lerEntradaNumerica(const std::string& prompt, T& valor, T minimo, T maximo);


// --- FUNÇÃO PRINCIPAL (main) ---
int main() {
    configurarConsole();
    std::vector<Imovel> baseDeImoveis; // cria o vetor que guarda os imoveis
    carregarDados(baseDeImoveis); // carrega os dados do arquivo.txt pro vetor
    
    std::cout << "\nBem-vindo(a) ao Sistema de Gestão de Imóveis!" << std::endl;
    pausarParaContinuar();

    int opcao; //Armazena a escolha feita no menu
    do {
        exibirMenu();
        opcao = lerOpcaoMenu(0, 7);

        switch (opcao) {
            case 1: incluirImovel(baseDeImoveis); break;
            case 2: buscarExcluirImovel(baseDeImoveis); break;
            case 3: buscarPorValor(baseDeImoveis); break;
            case 4: buscarPorCaracteristicas(baseDeImoveis); break;
            case 5: buscarPorQuartosSuites(baseDeImoveis); break;
            case 6: gerarRelatorioEstatisticas(baseDeImoveis); break;
            case 7: listarTodosImoveis(baseDeImoveis); break;
            case 0: // caso que sireciona a saída do programa
                std::cout << "\nSalvando dados e encerrando o programa..." << std::endl;
                salvarDados(baseDeImoveis);
                std::cout << "Dados salvos com sucesso!" << std::endl;
                std::cout << "Obrigado por usar o sistema!" << std::endl;
                break;
        }
        
        if (opcao != 0) {
            pausarParaContinuar();
        }

    } while (opcao != 0);

    return 0;
}


// --- IMPLEMENTAÇÃO DAS FUNÇÕES ---

//configurarConsole() - configuração pro console aceitar caracteres especiais
void configurarConsole() {
    #ifdef _WIN32
    setlocale(LC_ALL, "Portuguese");
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    #endif
}

void exibirMenu() { //função para exibir o menu ao usuário
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    std::cout << "========================================" << std::endl;
    std::cout << "      Sistema de Gestão de Imóveis      " << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "1. Incluir Imóvel" << std::endl;
    std::cout << "2. Buscar e Excluir Imóvel" << std::endl;
    std::cout << "3. Buscar por Faixa de Valor" << std::endl;
    std::cout << "4. Buscar por Características" << std::endl;
    std::cout << "5. Buscar por Quartos e Suítes" << std::endl;
    std::cout << "6. Gerar Relatório e Estatísticas" << std::endl;
    std::cout << "7. Listar Todos os Imóveis" << std::endl;
    std::cout << "0. Sair e Salvar" << std::endl;
    std::cout << "========================================" << std::endl;
}

// Tratamento de Erro - melhora a usabilidade mostrando por mais tempo as operações.
void pausarParaContinuar() {
    std::cout << "\n--- Pressione ENTER para continuar ---"; //espera o enter
    std::cin.get();
}

//Tratamento de Erro - exibe a mensagem de erro ou usuário e sugere o tentar novamente.
void pausarComMensagem(const std::string& mensagem) {
    std::cout << "\n" << mensagem << std::endl;
    std::cout << "--- Pressione ENTER para tentar novamente ---";
    std::cin.get();
}

void limparBuffer() {
    // Limpa o estado de erro do cin (caso exista, como ao digitar texto num campo numérico)
    std::cin.clear();
    // Ignora todos os caracteres que sobraram no buffer de entrada até encontrar uma nova linha.
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Lê a opção do menu e garante que ela esteja dentro de um intervalo válido.
int lerOpcaoMenu(int minimo, int maximo) {
    int valor;
    while (true) {
        std::cout << "Escolha uma opção: ";
        std::cin >> valor;
        
        // Verifica se a leitura foi bem-sucedida E se o valor está no intervalo.
        if (std::cin.good() && valor >= minimo && valor <= maximo) {
            limparBuffer(); 
            return valor;
        }
        
        std::cout << ">>> ERRO: Opção inválida!" << std::endl;
        limparBuffer(); // Limpa a entrada incorreta (ex: "abc").
        pausarComMensagem("Por favor, digite um número entre " + std::to_string(minimo) + " e " + std::to_string(maximo) + ".");
        exibirMenu(); 
}

//Tratamento de Erro - validação estrita á 0 ou 1 nos casos de sim/nao
int lerEntradaBinaria(const std::string& prompt) {
    int valor;
    while (true) {
        std::cout << prompt << " (0 = Não, 1 = Sim): ";
        std::cin >> valor;
        
        if (std::cin.good() && (valor == 0 || valor == 1)) {
            return valor;
        }
        
        std::cout << ">>> ERRO: Entrada inválida!" << std::endl;
        limparBuffer();
        pausarComMensagem("Digite apenas 0 para NÃO ou 1 para SIM.");
    }
}

std::string lerStringValida(const std::string& prompt, const std::vector<std::string>& opcoesValidas) {
    std::string entrada;
    while (true) {
        std::cout << prompt << " (";
        for (size_t i = 0; i < opcoesValidas.size(); ++i) {
            std::cout << opcoesValidas[i] << (i < opcoesValidas.size() - 1 ? "/" : "");
        }
        std::cout << "): ";
        std::getline(std::cin, entrada);
        
        // Compara sem diferenciar maiúsculas/minúsculas
        for (const auto& opcao : opcoesValidas) {
            if (strcasecmp(entrada.c_str(), opcao.c_str()) == 0) {
                return opcao; // Retorna a opção com a capitalização original.
            }
        }
                
        std::cout << ">>> ERRO: Opção inválida!" << std::endl;
        pausarComMensagem("Escolha uma das opções listadas.");
    }
}

std::string lerString(const std::string& prompt) {
    std::string entrada;
    while (true) {
        std::cout << prompt << ": ";
        std::getline(std::cin, entrada);
        if (!entrada.empty()) {
            return entrada;
        }
        std::cout << ">>> ERRO: Este campo não pode estar vazio." << std::endl;
        pausarComMensagem("Por favor, insira uma informação.");
    }
}

template <typename T>
void lerEntradaNumerica(const std::string& prompt, T& valor, T minimo, T maximo) {
    while (true) {
        std::cout << prompt << " (entre " << minimo << " e " << maximo << "): ";
        std::cin >> valor;
        
        if (std::cin.good() && valor >= minimo && valor <= maximo) {
            limparBuffer();
            return;
        }
        
        std::cout << ">>> ERRO: Valor inválido!" << std::endl;
        limparBuffer();
        pausarComMensagem("Digite um número no intervalo permitido.");
    }
}

void carregarDados(std::vector<Imovel>& imoveis) {
    std::ifstream arquivo(NOME_ARQUIVO);
    if (!arquivo.is_open()) {
        std::cout << "Arquivo " << NOME_ARQUIVO << " não encontrado. Iniciando com base vazia." << std::endl;
        return;
    }
    std::string linha;
    while (std::getline(arquivo, linha) && imoveis.size() < MAX_IMOVEIS) {
        std::stringstream ss(linha);
        Imovel imovel;
        std::string campo;
        try {
            std::getline(ss, imovel.tipo, ',');
            std::getline(ss, imovel.finalidade, ',');
            std::getline(ss, imovel.endereco, ',');
            std::getline(ss, imovel.bairro, ',');
            std::getline(ss, imovel.cidade, ',');
            std::getline(ss, campo, ','); imovel.area = std::stod(campo);
            std::getline(ss, campo, ','); imovel.valor = std::stod(campo);
            std::getline(ss, campo, ','); imovel.iptu = std::stod(campo);
            std::getline(ss, campo, ','); imovel.quartos = std::stoi(campo);
            std::getline(ss, campo, ','); imovel.suites = std::stoi(campo);
            std::getline(ss, campo, ','); imovel.banheiros = std::stoi(campo);
            std::getline(ss, campo, ','); imovel.vagas = std::stoi(campo);
            std::getline(ss, campo, ','); imovel.cozinha = std::stoi(campo);
            std::getline(ss, campo, ','); imovel.sala = std::stoi(campo);
            std::getline(ss, campo, ','); imovel.varanda = std::stoi(campo);
            std::getline(ss, campo, ','); imovel.area_servico = std::stoi(campo);
            std::getline(ss, imovel.piso, ',');
            std::getline(ss, imovel.conservacao, ',');
            std::getline(ss, campo, ','); imovel.armarios = std::stoi(campo);
            std::getline(ss, campo, ','); imovel.ar_condicionado = std::stoi(campo);
            std::getline(ss, campo, ','); imovel.aquecedor = std::stoi(campo);
            std::getline(ss, campo); imovel.ventilador = std::stoi(campo);
            imoveis.push_back(imovel);
        } catch (const std::exception& e) {
            // Se uma linha estiver mal formatada (ex: falta uma vírgula), este bloco impede que o programa quebre.
            std::cout << "Aviso: Linha corrompida no arquivo ignorada -> " << linha << std::endl;
        }
    }
    arquivo.close();
}

void salvarDados(const std::vector<Imovel>& imoveis) {
    std::ofstream arquivo(NOME_ARQUIVO);
    if (!arquivo.is_open()) {
        std::cerr << "ERRO FATAL: Não foi possível salvar os dados!" << std::endl;
        return;
    }
    for (const auto& imovel : imoveis) {
        arquivo << imovel.tipo << "," << imovel.finalidade << "," << imovel.endereco << ","
                << imovel.bairro << "," << imovel.cidade << "," << imovel.area << ","
                << imovel.valor << "," << imovel.iptu << "," << imovel.quartos << ","
                << imovel.suites << "," << imovel.banheiros << "," << imovel.vagas << ","
                << imovel.cozinha << "," << imovel.sala << "," << imovel.varanda << ","
                << imovel.area_servico << "," << imovel.piso << "," << imovel.conservacao << ","
                << imovel.armarios << "," << imovel.ar_condicionado << "," << imovel.aquecedor << ","
                << imovel.ventilador << std::endl;
    }
    arquivo.close();
}

void incluirImovel(std::vector<Imovel>& imoveis) {
    if (imoveis.size() >= MAX_IMOVEIS) {
        pausarComMensagem("ERRO: Limite máximo de imóveis atingido (" + std::to_string(MAX_IMOVEIS) + ").");
        return;
    }
    std::cout << "\n=== INCLUIR NOVO IMÓVEL ===" << std::endl;
    Imovel novoImovel;
    
    
    novoImovel.tipo = lerStringValida("Tipo do imóvel", {"Casa", "Apartamento", "Sobrado", "Kitnet"});
    novoImovel.finalidade = lerStringValida("Finalidade", {"Venda", "Aluguel"});
    novoImovel.endereco = lerString("Endereço");
    novoImovel.bairro = lerString("Bairro");
    novoImovel.cidade = lerString("Cidade");
    
    //estrutura com delimitação do tipo valores permitidos 
    lerEntradaNumerica("Área (m²)", novoImovel.area, 1.0, 10000.0);
    lerEntradaNumerica("Valor (R$)", novoImovel.valor, 1.0, 50000000.0);
    lerEntradaNumerica("IPTU (R$)", novoImovel.iptu, 0.0, 100000.0);
    
    lerEntradaNumerica("Número de quartos", novoImovel.quartos, 0, 20);
    lerEntradaNumerica("Número de suítes", novoImovel.suites, 0, novoImovel.quartos);
    lerEntradaNumerica("Número de banheiros", novoImovel.banheiros, 0, 20);
    lerEntradaNumerica("Número de vagas", novoImovel.vagas, 0, 10);
    
    novoImovel.cozinha = lerEntradaBinaria("Possui cozinha");
    novoImovel.sala = lerEntradaBinaria("Possui sala");
    novoImovel.varanda = lerEntradaBinaria("Possui varanda");
    novoImovel.area_servico = lerEntradaBinaria("Possui área de serviço");
    
    novoImovel.piso = lerStringValida("Tipo de piso", {"Ceramica", "Porcelanato", "Madeira", "Laminado", "Marmore"});
    novoImovel.conservacao = lerStringValida("Estado de conservação", {"Novo", "Usado", "Reformado"});
    
    novoImovel.armarios = lerEntradaBinaria("Possui armários");
    novoImovel.ar_condicionado = lerEntradaBinaria("Possui ar condicionado");
    novoImovel.aquecedor = lerEntradaBinaria("Possui aquecedor");
    novoImovel.ventilador = lerEntradaBinaria("Possui ventilador");
    
    imoveis.push_back(novoImovel);
    std::cout << "\n✓ Imóvel incluído com sucesso!" << std::endl;
}

void buscarExcluirImovel(std::vector<Imovel>& imoveis) {
    if (imoveis.empty()) {
        pausarComMensagem("Nenhum imóvel cadastrado no sistema.");
        return;
    }
    std::cout << "\n=== BUSCAR E EXCLUIR IMÓVEL ===" << std::endl;
    std::string busca = lerString("Digite parte do endereço para buscar");
    
    std::string buscaLower = busca;
    std::transform(buscaLower.begin(), buscaLower.end(), buscaLower.begin(),
        [](unsigned char c){ return std::tolower(c); });

    std::vector<int> indices;
    for (size_t i = 0; i < imoveis.size(); ++i) {
        std::string enderecoLower = imoveis[i].endereco;
        std::transform(enderecoLower.begin(), enderecoLower.end(), enderecoLower.begin(), ::tolower);
        if (enderecoLower.find(buscaLower) != std::string::npos) {
            indices.push_back(i);
        }
    }

    if (indices.empty()) {
        pausarComMensagem("Nenhum imóvel encontrado com esse endereço.");
        return;
    }

    std::cout << "\nImóveis encontrados:" << std::endl;
    for (size_t i = 0; i < indices.size(); ++i) {
        imprimirDetalhesImovel(imoveis[indices[i]], i + 1);
    }
    
    std::cout << "Digite o número do imóvel para excluir (ou 0 para cancelar)" << std::endl;
    int opcao = lerOpcaoMenu(0, indices.size());
    if (opcao == 0) {
        std::cout << "Operação cancelada." << std::endl;
        return;
    }
    int indiceParaExcluir = indices[opcao - 1];

    std::cout << "\nTem certeza que deseja excluir este imóvel?" << std::endl;
    imprimirDetalhesImovel(imoveis[indiceParaExcluir]);
    int confirmar = lerEntradaBinaria("Confirmar exclusão");

    if (confirmar == 1) {
        imoveis.erase(imoveis.begin() + indiceParaExcluir);
        std::cout << "✓ Imóvel excluído com sucesso!" << std::endl;
    } else {
        std::cout << "Exclusão cancelada." << std::endl;
    }
}

void imprimirDetalhesImovel(const Imovel& imovel, int indice) {
    if (indice != -1) {
        std::cout << "\n--- Imóvel Nº " << indice << " ---" << std::endl;
    }
    std::cout << "Tipo: " << imovel.tipo << " | Finalidade: " << imovel.finalidade << std::endl;
    std::cout << "Endereço: " << imovel.endereco << ", " << imovel.bairro << ", " << imovel.cidade << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Área: " << imovel.area << "m² | Valor: R$ " << imovel.valor << " | IPTU: R$ " << imovel.iptu << std::endl;
    std::cout << "Quartos: " << imovel.quartos << " | Suítes: " << imovel.suites << " | Banheiros: " << imovel.banheiros << " | Vagas: " << imovel.vagas << std::endl;
    std::cout << "Piso: " << imovel.piso << " | Conservação: " << imovel.conservacao << std::endl;
    std::cout << "Extras: Cozinha(" << (imovel.cozinha ? "SIM" : "NÃO") << "), Sala(" << (imovel.sala ? "SIM" : "NÃO") << "), Varanda(" << (imovel.varanda ? "SIM" : "NÃO")
              << "), A. Serviço(" << (imovel.area_servico ? "SIM" : "NÃO") << "), Armários(" << (imovel.armarios ? "SIM" : "NÃO") << "), Ar Cond.(" << (imovel.ar_condicionado ? "SIM" : "NÃO")
              << "), Aquecedor(" << (imovel.aquecedor ? "SIM" : "NÃO") << "), Ventilador(" << (imovel.ventilador ? "SIM" : "NÃO") << ")" << std::endl;
}

void buscarPorValor(const std::vector<Imovel>& imoveis) {
    if (imoveis.empty()) {
        pausarComMensagem("Nenhum imóvel cadastrado no sistema.");
        return;
    }
    std::cout << "\n=== BUSCAR POR FAIXA DE VALOR ===" << std::endl;
    double valorMin, valorMax;
    lerEntradaNumerica("Valor mínimo (R$)", valorMin, 0.0, 50000000.0);
    lerEntradaNumerica("Valor máximo (R$)", valorMax, valorMin, 50000000.0);
    std::vector<int> encontrados;
    for (size_t i = 0; i < imoveis.size(); ++i) {
        if (imoveis[i].valor >= valorMin && imoveis[i].valor <= valorMax) {
            encontrados.push_back(i);
        }
    }
    if (encontrados.empty()) {
        pausarComMensagem("Nenhum imóvel encontrado nesta faixa de valor.");
        return;
    }
    std::cout << "\nImóveis encontrados (" << encontrados.size() << "):" << std::endl;
    for (int indice : encontrados) {
        imprimirDetalhesImovel(imoveis[indice], indice + 1);
    }
}

void buscarPorCaracteristicas(const std::vector<Imovel>& imoveis) {
    if (imoveis.empty()) {
        pausarComMensagem("Nenhum imóvel cadastrado no sistema.");
        return;
    }
    std::cout << "\n=== BUSCAR POR CARACTERÍSTICAS ===" << std::endl;
    limparBuffer();
    std::string tipo = lerStringValida("Tipo desejado", {"Casa", "Apartamento", "Sobrado", "Kitnet", "Todos"});
    std::string finalidade = lerStringValida("Finalidade", {"Venda", "Aluguel", "Todas"});
    std::string cidade = lerString("Cidade (ou ENTER para todas)");
    std::vector<int> encontrados;
    for (size_t i = 0; i < imoveis.size(); ++i) {
        bool match = true;
        if (tipo != "Todos" && imoveis[i].tipo != tipo) {
            match = false;
        }
        if (finalidade != "Todas" && imoveis[i].finalidade != finalidade) {
            match = false;
        }
        if (!cidade.empty()) {
            std::string cidadeLower = cidade;
            std::string imovelCidadeLower = imoveis[i].cidade;
            std::transform(cidadeLower.begin(), cidadeLower.end(), cidadeLower.begin(), ::tolower);
            std::transform(imovelCidadeLower.begin(), imovelCidadeLower.end(), imovelCidadeLower.begin(), ::tolower);
            if (imovelCidadeLower.find(cidadeLower) == std::string::npos) {
                match = false;
            }
        }
        if (match) {
            encontrados.push_back(i);
        }
    }
    if (encontrados.empty()) {
        pausarComMensagem("Nenhum imóvel encontrado com esses critérios.");
        return;
    }
    std::cout << "\nImóveis encontrados (" << encontrados.size() << "):" << std::endl;
    for (int indice : encontrados) {
        imprimirDetalhesImovel(imoveis[indice], indice + 1);
    }
}

void buscarPorQuartosSuites(const std::vector<Imovel>& imoveis) {
    if (imoveis.empty()) {
        pausarComMensagem("Nenhum imóvel cadastrado no sistema.");
        return;
    }
    std::cout << "\n=== BUSCAR POR QUARTOS E SUÍTES ===" << std::endl;
    int quartos, suites;
    lerEntradaNumerica("Número exato de quartos", quartos, 0, 20);
    lerEntradaNumerica("Número exato de suítes", suites, 0, quartos);
    std::vector<int> encontrados;
    for (size_t i = 0; i < imoveis.size(); ++i) {
        if (imoveis[i].quartos == quartos && imoveis[i].suites == suites) {
            encontrados.push_back(i);
        }
    }
    if (encontrados.empty()) {
        pausarComMensagem("Nenhum imóvel encontrado com essa configuração.");
        return;
    }
    std::cout << "\nImóveis encontrados (" << encontrados.size() << "):" << std::endl;
    for (int indice : encontrados) {
        imprimirDetalhesImovel(imoveis[indice], indice + 1);
    }
}

void gerarRelatorioEstatisticas(const std::vector<Imovel>& imoveis) {
    if (imoveis.empty()) {
        pausarComMensagem("Nenhum imóvel cadastrado para gerar estatísticas.");
        return;
    }
    std::cout << "\n=== RELATÓRIO E ESTATÍSTICAS ===" << std::endl;
    double total = imoveis.size();
    int countVenda = 0, countAluguel = 0;
    double somaValores = 0;
    for(const auto& imovel : imoveis) {
        if (imovel.finalidade == "Venda") countVenda++;
        else if (imovel.finalidade == "Aluguel") countAluguel++;
        somaValores += imovel.valor;
    }
    std::cout << std::fixed << std::setprecision(1);
    std::cout << "Distribuição por Finalidade:" << std::endl;
    std::cout << " - Venda: " << (countVenda / total) * 100.0 << "% (" << countVenda << " imóveis)" << std::endl;
    std::cout << " - Aluguel: " << (countAluguel / total) * 100.0 << "% (" << countAluguel << " imóveis)" << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\nValor Médio dos Imóveis: R$ " << (somaValores / total) << std::endl;
}

void listarTodosImoveis(std::vector<Imovel>& imoveis) {
    if (imoveis.empty()) {
        pausarComMensagem("Nenhum imóvel cadastrado no sistema.");
        return;
    }
    std::cout << "\n=== LISTAR TODOS OS IMÓVEIS ===" << std::endl;
    std::vector<Imovel> imoveisParaListar = imoveis;
    std::cout << "\nComo deseja ordenar a lista?" << std::endl;
    std::cout << "1. Por Preço (menor para maior)" << std::endl;
    std::cout << "2. Por Preço (maior para menor)" << std::endl;
    std::cout << "3. Por Área (menor para maior)" << std::endl;
    std::cout << "4. Por Área (maior para menor)" << std::endl;
    std::cout << "0. Ordem padrão (sem ordenação)" << std::endl;
    int escolha_ordem = lerOpcaoMenu(0, 4);
    switch(escolha_ordem) {
        case 1:
            std::sort(imoveisParaListar.begin(), imoveisParaListar.end(), [](const Imovel& a, const Imovel& b) { return a.valor < b.valor; });
            break;
        case 2:
            std::sort(imoveisParaListar.begin(), imoveisParaListar.end(), [](const Imovel& a, const Imovel& b) { return a.valor > b.valor; });
            break;
        case 3:
            std::sort(imoveisParaListar.begin(), imoveisParaListar.end(), [](const Imovel& a, const Imovel& b) { return a.area < b.area; });
            break;
        case 4:
            std::sort(imoveisParaListar.begin(), imoveisParaListar.end(), [](const Imovel& a, const Imovel& b) { return a.area > b.area; });
            break;
    }
    std::cout << "\n--- Lista de Imóveis ---" << std::endl;
    for (size_t i = 0; i < imoveisParaListar.size(); ++i) {
        imprimirDetalhesImovel(imoveisParaListar[i], i + 1);
    }
    std::cout << "\nDeseja exportar esta lista para um arquivo de texto? (s/n)" << std::endl;
    int exportar = lerEntradaBinaria("Sua escolha");
    if (exportar == 1) {
        std::string nomeArquivo = lerString("Nome do arquivo (ex: relatorio.txt)");
        exportarParaTxt(imoveisParaListar, nomeArquivo);
    }
}

void exportarParaTxt(const std::vector<Imovel>& imoveis, const std::string& nomeArquivo) {
    std::ofstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        pausarComMensagem("ERRO: Não foi possível criar o arquivo de exportação.");
        return;
    }
    arquivo << "Relatório de Imóveis Cadastrados\n";
    arquivo << "================================\n\n";
    for(size_t i = 0; i < imoveis.size(); ++i) {
        const auto& imovel = imoveis[i];
        arquivo << "--- Imóvel Nº " << (i + 1) << " ---\n";
        arquivo << "Tipo: " << imovel.tipo << " | Finalidade: " << imovel.finalidade << "\n";
        arquivo << "Endereço: " << imovel.endereco << ", " << imovel.bairro << ", " << imovel.cidade << "\n";
        arquivo << std::fixed << std::setprecision(2);
        arquivo << "Área: " << imovel.area << "m² | Valor: R$ " << imovel.valor << " | IPTU: R$ " << imovel.iptu << "\n";
        arquivo << "Quartos: " << imovel.quartos << " | Suítes: " << imovel.suites << " | Banheiros: " << imovel.banheiros << " | Vagas: " << imovel.vagas << "\n";
        arquivo << "Piso: " << imovel.piso << " | Conservação: " << imovel.conservacao << "\n";
        arquivo << "Extras: Cozinha(" << (imovel.cozinha ? "SIM" : "NÃO") << "), Sala(" << (imovel.sala ? "SIM" : "NÃO") << "), Varanda(" << (imovel.varanda ? "SIM" : "NÃO")
              << "), A. Serviço(" << (imovel.area_servico ? "SIM" : "NÃO") << "), Armários(" << (imovel.armarios ? "SIM" : "NÃO") << "), Ar Cond.(" << (imovel.ar_condicionado ? "SIM" : "NÃO")
              << "), Aquecedor(" << (imovel.aquecedor ? "SIM" : "NÃO") << "), Ventilador(" << (imovel.ventilador ? "SIM" : "NÃO") << ")\n\n";
    }
    arquivo.close();
    std::cout << "\n✓ Lista exportada com sucesso para '" << nomeArquivo << "'!" << std::endl;
}
