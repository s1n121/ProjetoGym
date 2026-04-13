#include <iostream>
#include <string>

using namespace std;

// 
struct morada {
    string rua;
    int numero;
    string cidade;
};

struct data_nascimento {
    int dia;
    int mes;
    int ano;
};

struct cliente {
    int id;
    string nome;
    struct data_nascimento idade;
    int altura;
    int peso;
    int sexo;
    int telefone;
    int tipo_plano;
    struct morada morada_completa;
};       

int main() {

    
    return 0;
}