//  Sistema de Gestao de Socios - Ginasio
//  Modulo 5 - Estruturas de Dados Compostas
//  Disciplina: Programacao e Sistemas de Informacao

#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <windows.h>

using namespace std;

//  CONSTANTES

const int MAX_SOCIOS = 100;
const int LARGURA = 120;

//  STRUCTS

struct Morada
{
    string rua;
    int numero;
    string cidade;
};

struct DataNascimento
{
    int dia;
    int mes;
    int ano;
};

struct Cliente
{
    int id;
    string nome;
    DataNascimento data_nascimento;
    int altura; // cm
    int peso;   // kg
    int sexo;   // 1=Masculino  2=Feminino  3=Outro
    string telefone;
    int tipo_plano; // 1=Basico  2=Standard  3=Premium
    Morada morada;
    bool ativo; // false = eliminado (slot livre)
};

//  ARRAY GLOBAL E CONTADORES

Cliente socios[MAX_SOCIOS];
int totalSocios = 0; // slots ocupados (incluindo eliminados)
int proximoId = 1;   // auto-increment

//  UTILITARIOS DE CONSOLA

// Imprime uma linha horizontal com o caracter dado
void linhaHorizontal(char c = '-')
{
    cout << string(LARGURA, c) << "\n";
}

// Centraliza texto numa linha de LARGURA caracteres
void textoCentrado(const string &texto, char preenchimento = ' ')
{
    int padding = (LARGURA - (int)texto.size()) / 2;
    if (padding < 0)
        padding = 0;
    cout << string(padding, preenchimento) << texto << "\n";
}

// Limpa o ecra (compativel com Windows e Linux)
void limparEcra()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Pausa ate o utilizador premir Enter
void pausar()
{
    cout << "\nPrima ENTER para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// Cabecalho principal
void cabecalho()
{
    limparEcra();
    linhaHorizontal('=');
    textoCentrado("SISTEMA DE GESTAO DE SOCIOS - GINASIO FITNESS+");
    linhaHorizontal('=');
    cout << "\n";
}

//  LEITURA SEGURA DE DADOS

// Le um inteiro com validacao de intervalo
int lerInteiro(const string &prompt, int minVal, int maxVal)
{
    int valor;
    while (true)
    {
        cout << prompt;
        if (cin >> valor && valor >= minVal && valor <= maxVal)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return valor;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  [ERRO] Introduza um numero entre " << minVal
             << " e " << maxVal << ".\n";
    }
}

// Le uma string nao vazia
string lerString(const string& prompt, bool apenasLetras = false) {
    string valor;
    while (true) {
        cout << prompt;
        getline(cin, valor);

        // remover espaços no início/fim
        size_t inicio = valor.find_first_not_of(" \t");
        size_t fim    = valor.find_last_not_of(" \t");
        if (inicio != string::npos)
            valor = valor.substr(inicio, fim - inicio + 1);

        if (valor.empty()) {
            cout << "  [ERRO] O campo nao pode estar vazio.\n";
            continue;
        }

        // validação extra (se for nome)
        if (apenasLetras) {
            bool valido = true;

            for (char c : valor) {
                if (!isalpha(c) && c != ' ') {
                    valido = false;
                    break;
                }
            }

            if (!valido) {
                cout << "  [ERRO] Use apenas letras e espacos.\n";
                continue;
            }
        }

        return valor;
    }
}

// Le um numero de telefone (9 digitos)
string lerTelefone(const string &prompt)
{
    string tel;
    while (true)
    {
        cout << prompt;
        getline(cin, tel);
        bool valido = (tel.size() == 9);
        if (valido)
        {
            for (char c : tel)
                if (!isdigit(c))
                {
                    valido = false;
                    break;
                }
        }
        if (valido)
            return tel;
        cout << "  [ERRO] O telefone deve ter exatamente 9 digitos numericos.\n";
    }
}

// Valida uma data de nascimento
bool dataValida(int dia, int mes, int ano)
{
    if (ano < 1900 || ano > 2025)
        return false;
    if (mes < 1 || mes > 12)
        return false;
    int diasMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    // Ano bissexto
    if ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0))
        diasMes[1] = 29;
    return (dia >= 1 && dia <= diasMes[mes - 1]);
}

DataNascimento lerData(const string &prompt)
{
    DataNascimento d;
    cout << prompt << "\n";
    while (true)
    {
        d.dia = lerInteiro("    Dia   (1-31)  : ", 1, 31);
        d.mes = lerInteiro("    Mes   (1-12)  : ", 1, 12);
        d.ano = lerInteiro("    Ano (1900-2025): ", 1900, 2025);
        if (dataValida(d.dia, d.mes, d.ano))
            return d;
        cout << "  [ERRO] Data invalida. Tente novamente.\n";
    }
}

//  FUNCOES AUXILIARES

// verificar se a morada e valida
bool moradaValida(const string &rua, int numero, const string &cidade)
{
    // Regras simples:
    // - Rua tem pelo menos 3 caracteres
    // - Cidade tem pelo menos 2 caracteres
    // - Número já é validado no intervalo

    if (rua.length() < 3)
        return false;
    if (cidade.length() < 2)
        return false;

    // Verifica se a rua tem pelo menos uma letra
    bool temLetra = false;
    for (char c : rua)
    {
        if (isalpha(c))
        {
            temLetra = true;
            break;
        }
    }

    return temLetra;
}

// Converte numero de plano para string descritiva
string nomePlano(int plano)
{
    switch (plano)
    {
    case 1:
        return "Basico   ";
    case 2:
        return "Standard ";
    case 3:
        return "Premium  ";
    default:
        return "Desconhec";
    }
}

// Converte numero de sexo para string
string nomeSexo(int sexo)
{
    switch (sexo)
    {
    case 1:
        return "Masculino";
    case 2:
        return "Feminino ";
    case 3:
        return "Outro    ";
    default:
        return "-        ";
    }
}

// Formata data para string
string formatarData(const DataNascimento &d)
{
    char buf[11];
    snprintf(buf, sizeof(buf), "%02d/%02d/%04d", d.dia, d.mes, d.ano);
    return string(buf);
}

// Conta socios ativos
int contarAtivos()
{
    int count = 0;
    for (int i = 0; i < totalSocios; i++)
        if (socios[i].ativo)
            count++;
    return count;
}

// Procura socio por ID; retorna indice ou -1
int procurarPorId(int id)
{
    for (int i = 0; i < totalSocios; i++)
        if (socios[i].ativo && socios[i].id == id)
            return i;
    return -1;
}

// Confirma uma acao critica (S/N)
bool confirmar(const string &mensagem)
{
    string resposta;
    cout << mensagem << " (S/N): ";
    getline(cin, resposta);
    return (!resposta.empty() &&
            (resposta[0] == 'S' || resposta[0] == 's'));
}

//  IMPRIMIR DETALHE DE UM SOCIO

void imprimirSocio(const Cliente &c)
{
    linhaHorizontal();
    cout << "  ID do Socio    : " << c.id << "\n";
    cout << "  Nome           : " << c.nome << "\n";
    cout << "  Data Nascimento: " << formatarData(c.data_nascimento) << "\n";
    cout << "  Sexo           : " << nomeSexo(c.sexo) << "\n";
    cout << "  Altura         : " << c.altura << " cm" << "\n";
    cout << "  Peso           : " << c.peso << " kg" << "\n";
    cout << "  Telefone       : " << c.telefone << "\n";
    cout << "  Plano          : " << nomePlano(c.tipo_plano) << "\n";
    cout << "  Morada         : " << c.morada.rua << ", N."
         << c.morada.numero << ", " << c.morada.cidade << "\n";
    linhaHorizontal();
}

//  OPERACAO 1 - ADICIONAR SOCIO

void adicionarSocio()
{
    cabecalho();
    textoCentrado("ADICIONAR NOVO SOCIO");
    cout << "\n";

    if (contarAtivos() >= MAX_SOCIOS)
    {
        cout << "  [AVISO] Capacidade maxima atingida ("
             << MAX_SOCIOS << " socios).\n";
        pausar();
        return;
    }

    // Procura um slot livre (slot de socio eliminado) ou usa o proximo
    int idx = totalSocios;
    for (int i = 0; i < totalSocios; i++)
    {
        if (!socios[i].ativo)
        {
            idx = i;
            break;
        }
    }
    if (idx == totalSocios)
        totalSocios++;

    Cliente &c = socios[idx];
    c.id = proximoId++;
    c.ativo = true;

    cout << "  Novo ID atribuido: " << c.id << "\n\n";

    c.nome = lerString("  Nome completo  : ", true);
    c.data_nascimento = lerData("  Data de nascimento:");
    c.sexo = lerInteiro("  Sexo (1=Masc 2=Fem 3=Outro): ", 1, 3);
    c.altura = lerInteiro("  Altura (cm)    : ", 50, 250);
    c.peso = lerInteiro("  Peso   (kg)    : ", 10, 300);
    c.telefone = lerTelefone("  Telefone (9 dig): ");

    cout << "\n  Plano de subscricao:\n";
    cout << "    1 - Basico   (acesso sala)\n";
    cout << "    2 - Standard (sala + aulas)\n";
    cout << "    3 - Premium  (sala + aulas + SPA)\n";
    c.tipo_plano = lerInteiro("  Opcao: ", 1, 3);

    cout << "\n  Morada:\n";

    while (true)
    {
        string rua = lerString("    Rua          : ");
        int numero = lerInteiro("    Numero       : ", 1, 99999);
        string cidade = lerString("    Cidade       : ");

        if (moradaValida(rua, numero, cidade))
        {
            c.morada.rua = rua;
            c.morada.numero = numero;
            c.morada.cidade = cidade;
            break;
        }

        cout << "  [ERRO] Morada invalida. Introduza uma rua e cidade validas.\n\n";
    }

    cout << "\n";
    linhaHorizontal();
    cout << "  Socio registado com sucesso! ID: " << c.id << "\n";
    linhaHorizontal();
    pausar();
}

//  OPERACAO 2 - LISTAR SOCIOS

void listarSocios()
{
    cabecalho();
    textoCentrado("LISTA DE SOCIOS");
    cout << "\n";

    int ativos = contarAtivos();
    if (ativos == 0)
    {
        cout << "  Nao existem socios registados.\n";
        pausar();
        return;
    }

    // Cabecalho da tabela
    cout << left
         << setw(6) << "ID"
         << setw(30) << "Nome"
         << setw(12) << "Nasc."
         << setw(10) << "Sexo"
         << setw(7) << "Alt(cm)"
         << setw(7) << "Peso(kg)"
         << setw(11) << "Telefone"
         << setw(11) << "Plano"
         << "Cidade"
         << "\n";
    linhaHorizontal('-');

    for (int i = 0; i < totalSocios; i++)
    {
        if (!socios[i].ativo)
            continue;
        const Cliente &c = socios[i];
        cout << left
             << setw(6) << c.id
             << setw(30) << c.nome.substr(0, 28)
             << setw(12) << formatarData(c.data_nascimento)
             << setw(10) << nomeSexo(c.sexo)
             << setw(7) << c.altura
             << setw(7) << c.peso
             << setw(11) << c.telefone
             << setw(11) << nomePlano(c.tipo_plano)
             << c.morada.cidade
             << "\n";
    }
    linhaHorizontal('-');
    cout << "  Total de socios ativos: " << ativos << "\n";
    pausar();
}

//  OPERACAO 3 - CONSULTAR SOCIO

void consultarSocio()
{
    cabecalho();
    textoCentrado("CONSULTAR SOCIO");
    cout << "\n";

    if (contarAtivos() == 0)
    {
        cout << "  Nao existem socios registados.\n";
        pausar();
        return;
    }

    cout << "  Pesquisar por:\n";
    cout << "    1 - Numero de socio (ID)\n";
    cout << "    2 - Nome (pesquisa parcial)\n";
    int opcao = lerInteiro("  Opcao: ", 1, 2);

    if (opcao == 1)
    {
        int id = lerInteiro("  Introduza o ID: ", 1, 999999);
        int idx = procurarPorId(id);
        if (idx == -1)
        {
            cout << "\n  [AVISO] Socio com ID " << id << " nao encontrado.\n";
        }
        else
        {
            imprimirSocio(socios[idx]);
        }
    }
    else
    {
        string termo = lerString("  Introduza parte do nome: ");
        // Pesquisa case-insensitive
        string termoLower = termo;
        transform(termoLower.begin(), termoLower.end(),
                  termoLower.begin(), ::tolower);

        bool encontrou = false;
        for (int i = 0; i < totalSocios; i++)
        {
            if (!socios[i].ativo)
                continue;
            string nomeLower = socios[i].nome;
            transform(nomeLower.begin(), nomeLower.end(),
                      nomeLower.begin(), ::tolower);
            if (nomeLower.find(termoLower) != string::npos)
            {
                imprimirSocio(socios[i]);
                encontrou = true;
            }
        }
        if (!encontrou)
            cout << "\n  [AVISO] Nenhum socio encontrado com esse nome.\n";
    }
    pausar();
}

//  OPERACAO 4 - ALTERAR SOCIO

void alterarSocio()
{
    cabecalho();
    textoCentrado("ALTERAR DADOS DE SOCIO");
    cout << "\n";

    if (contarAtivos() == 0)
    {
        cout << "  Nao existem socios registados.\n";
        pausar();
        return;
    }

    int id = lerInteiro("  Introduza o ID do socio: ", 1, 999999);
    int idx = procurarPorId(id);
    if (idx == -1)
    {
        cout << "\n  [AVISO] Socio com ID " << id << " nao encontrado.\n";
        pausar();
        return;
    }

    imprimirSocio(socios[idx]);
    cout << "\n  O que pretende alterar?\n";
    cout << "    1 - Nome\n";
    cout << "    2 - Data de nascimento\n";
    cout << "    3 - Sexo\n";
    cout << "    4 - Altura\n";
    cout << "    5 - Peso\n";
    cout << "    6 - Telefone\n";
    cout << "    7 - Plano de subscricao\n";
    cout << "    8 - Morada\n";
    cout << "    0 - Cancelar\n";
    int campo = lerInteiro("  Campo: ", 0, 8);

    Cliente &c = socios[idx];
    switch (campo)
    {
    case 0:
        return;
    case 1:
        c.nome = lerString("  Novo nome: ");
        break;
    case 2:
        c.data_nascimento = lerData("  Nova data de nascimento:");
        break;
    case 3:
        c.sexo = lerInteiro("  Sexo (1=Masc 2=Fem 3=Outro): ", 1, 3);
        break;
    case 4:
        c.altura = lerInteiro("  Nova altura (cm): ", 50, 250);
        break;
    case 5:
        c.peso = lerInteiro("  Novo peso (kg): ", 10, 300);
        break;
    case 6:
        c.telefone = lerTelefone("  Novo telefone: ");
        break;
    case 7:
        cout << "    1-Basico  2-Standard  3-Premium\n";
        c.tipo_plano = lerInteiro("  Novo plano: ", 1, 3);
        break;
    case 8:
        c.morada.rua = lerString("  Nova rua: ");
        c.morada.numero = lerInteiro("  Novo numero: ", 1, 99999);
        c.morada.cidade = lerString("  Nova cidade: ");
        break;
    }

    cout << "\n  Dados alterados com sucesso!\n";
    pausar();
}

//  OPERACAO 5 - ELIMINAR SOCIO

void eliminarSocio()
{
    cabecalho();
    textoCentrado("ELIMINAR SOCIO");
    cout << "\n";

    if (contarAtivos() == 0)
    {
        cout << "  Nao existem socios registados.\n";
        pausar();
        return;
    }

    int id = lerInteiro("  Introduza o ID do socio a eliminar: ", 1, 999999);
    int idx = procurarPorId(id);
    if (idx == -1)
    {
        cout << "\n  [AVISO] Socio com ID " << id << " nao encontrado.\n";
        pausar();
        return;
    }

    imprimirSocio(socios[idx]);
    cout << "\n";

    if (confirmar("  Tem a certeza que pretende eliminar este socio?"))
    {
        socios[idx].ativo = false;
        cout << "\n  Socio eliminado com sucesso.\n";
    }
    else
    {
        cout << "\n  Operacao cancelada.\n";
    }
    pausar();
}

//  MENU PRINCIPAL

void menuPrincipal()
{
    int opcao = -1;
    do
    {
        cabecalho();
        cout << "  Socios ativos: " << contarAtivos()
             << " / " << MAX_SOCIOS << "\n\n";

        linhaHorizontal();
        textoCentrado("MENU PRINCIPAL");
        linhaHorizontal();
        cout << "\n";
        cout << "    [1]  Adicionar novo socio\n";
        cout << "    [2]  Listar todos os socios\n";
        cout << "    [3]  Consultar socio\n";
        cout << "    [4]  Alterar dados de socio\n";
        cout << "    [5]  Eliminar socio\n";
        cout << "    [0]  Sair\n";
        cout << "\n";
        linhaHorizontal();

        opcao = lerInteiro("  Opcao: ", 0, 5);

        switch (opcao)
        {
        case 1:
            adicionarSocio();
            break;
        case 2:
            listarSocios();
            break;
        case 3:
            consultarSocio();
            break;
        case 4:
            alterarSocio();
            break;
        case 5:
            eliminarSocio();
            break;
        case 0:
            cabecalho();
            textoCentrado("Obrigado por utilizar o sistema. Ate breve!");
            cout << "\n\n";
            break;
        }
    } while (opcao != 0);
}

//  MAIN

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    cout << R"(

 ___            ___
/   \          /   \
\_   \        /  __/
 _\   \      /  /__
 \___  \____/   __/
     \_       _/
       | @ @  \_
       |
     _/     /\
    /o)  (o/\ \_
    \_____/ /
      \____/

)" << endl;

    cout << "\nPrima ENTER para continuar...";
    cin.get();

    menuPrincipal();
    return 0;
}
