# Gerenciador_Imoveis
============================================
          PROJETO: GERENCIADOR DE IMÓVEIS
=============================================

AUTORA:
- Júlia Alves da Rocha - 2025.1.08.015

CURSO:
- AEDs I – Prática (DCE794)

DESCRIÇÃO DO PROJETO:
Este é um sistema de linha de comando desenvolvido em C++ para gerenciar um
catálogo de imóveis para venda e locação. O programa lê os dados de um arquivo
de texto, permite que o utilizador manipule esses dados em memória através de um menu
e, ao final, salva as alterações de volta no arquivo.

---
### COMO COMPILAR E EXECUTAR

1.  **Pré-requisitos:** Certifique-se de que tem um compilador C++ (como o g++ ou o Clang, que vêm com ambientes como MinGW ou Code::Blocks) instalado no seu computador.

2.  **Preparar os Ficheiros:** Coloque os ficheiros `main.cpp` e `BD_Imoveis2.txt` na mesma pasta.

3.  **Compilar:** Abra um terminal (CMD, PowerShell, etc.) na pasta do projeto e execute o seguinte comando:
    ```bash
    g++ -o gestao_imoveis main.cpp -std=c++11
    ```
4.  **Executar:** Após a compilação, um novo ficheiro chamado `gestao_imoveis.exe` (no Windows) ou `gestao_imoveis` (no Linux/macOS) será criado. Execute-o com o comando:
    * **No Windows:** `gestao_imoveis.exe`
    * **No Linux/macOS:** `./gestao_imoveis`

---
### ESTRUTURA DO FICHEIRO DE DADOS (BD_Imoveis2.txt)

O arquivo `BD_Imoveis2.txt` funciona como a base de dados do sistema.

* Cada linha representa um único imóvel.
* Os 22 campos de informação de cada imóvel devem ser separados por um **espaço**.
* A primeira linha do arquivo é um cabeçalho e é ignorada pelo programa.
* A última linha do arquivo **deve** conter apenas a palavra `fim` para sinalizar o término dos dados.
* **Importante:** Campos de texto como o endereço não devem conter espaços. Utilize um sublinhado (`_`) no lugar (ex: `Rua_das_Flores`).

---
### INSTRUÇÕES DE USO DO PROGRAMA

Após executar o programa, um menu com as seguintes opções será exibido:

**1. Incluir Imóvel:**
   - Adiciona um novo registo de imóvel à base de dados.
   - Será solicitado que insira cada uma das informações do imóvel.

**2. Buscar e Excluir Imóvel por Rua:**
   - Procura por um imóvel digitando parte do nome da rua.
   - Se um ou mais imóveis forem encontrados, os seus detalhes serão exibidos.
   - Poderá então escolher o índice do imóvel que deseja excluir.

**3. Buscar Imóveis por Faixa de Valor:**
   - Filtra os imóveis para "aluguel" ou "temporada" dentro de uma faixa de preço (mínimo e máximo) definida por si.

**4. Busca Avançada por Características:**
   - Permite uma busca detalhada combinando vários critérios como tipo, finalidade, bairro, cidade e número mínimo de quartos, suítes e vagas. Pressione Enter num campo para não o incluir no filtro.

**5. Buscar por Nº de Quartos e Suítes:**
   - Encontra imóveis com um número exato de quartos e suítes.

**6. Relatório de Estatísticas:**
   - Exibe um resumo estatístico da base de dados (percentagens por finalidade, contagem de casas com suítes, etc.).
   - Oferece a opção de exportar este relatório para um ficheiro `.txt`.

**7. Listar Todos os Imóveis (com ordenação):**
   - Exibe uma lista completa de todos os imóveis cadastrados.
   - Antes de listar, permite-lhe ordenar os resultados por preço ou por área, em ordem crescente ou decrescente.
   - No final da listagem, oferece a opção de exportar a lista (já ordenada) para um ficheiro `.txt`.

**0. Sair e Salvar:**
   - Encerra o programa e salva todas as alterações (inclusões e exclusões) de volta no ficheiro `BD_Imoveis2.txt`.
   - **Atenção:** Se fechar o programa de outra forma, as alterações feitas na sessão serão perdidas.

---

