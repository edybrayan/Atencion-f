#include <iostream>
#include <random>
#include <vector>

class Cliente {
public:
    std::string tipo_cliente;

    Cliente(const std::string& tipo_cliente) : tipo_cliente(tipo_cliente) {}
};

class Ventanilla {
public:
    int numero;
    double tiempo_atencion;
    double tiempo_sin_atender;

    Ventanilla(int numero) : numero(numero), tiempo_atencion(0), tiempo_sin_atender(0) {}

    void atenderCliente(Cliente cliente) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(1, 10);
        double tiempo = dis(gen);
        tiempo_atencion += tiempo;
        std::cout << "Ventanilla " << numero << ": Atendiendo cliente tipo " << cliente.tipo_cliente << " durante " << tiempo << " minutos\n";
    }

    void sinAtender() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis_veces(1, 5);
        std::uniform_real_distribution<> dis_tiempo(5, 30);
        int veces = dis_veces(gen);
        double tiempo = dis_tiempo(gen);
        tiempo_sin_atender += veces * tiempo;
        std::cout << "Ventanilla " << numero << ": No atenderá durante un total de " << veces << " veces, por un tiempo total de " << veces * tiempo << " minutos\n";
    }
};

void simularAtencion(int num_ventanillas, int num_clientes) {
    std::vector<Ventanilla> ventanillas;
    for (int i = 1; i <= num_ventanillas; i++) {
        ventanillas.push_back(Ventanilla(i));
    }

    std::vector<Cliente> clientes;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution<> dis_tipo({ 0.7, 0.2, 0.1 });
    std::discrete_distribution<> dis_subtipo({ 0.4, 0.3, 0.15, 0.15 });
    std::discrete_distribution<> dis_subtipo_preferencial({ 0.6, 0.2, 0.2 });

    for (int i = 0; i < num_clientes; i++) {
        int tipo = dis_tipo(gen);
        if (tipo == 0) {
            int subtipo = dis_subtipo(gen);
            std::string tipo_cliente;
            if (subtipo == 0) {
                tipo_cliente = "Tarjeta - Cuentas comunes";
            } else if (subtipo == 1) {
                tipo_cliente = "Tarjeta - Personas naturales VIP";
            } else if (subtipo == 2) {
                tipo_cliente = "Tarjeta - Personas jurídicas comunes";
            } else if (subtipo == 3) {
                tipo_cliente = "Tarjeta - Personas jurídicas VIP";
            }
            clientes.push_back(Cliente(tipo_cliente));
        } else if (tipo == 1) {
            clientes.push_back(Cliente("Sin tarjeta"));
        } else if (tipo == 2) {
            int subtipo = dis_subtipo_preferencial(gen);
            std::string tipo_cliente;
            if (subtipo == 0) {
                tipo_cliente = "Preferencial - Mayores de 60 años";
            } else if (subtipo == 1) {
                tipo_cliente = "Preferencial - Deficiencia física";
            } else if (subtipo == 2) {
                tipo_cliente = "Preferencial - Necesidades especiales";
            }
            clientes.push_back(Cliente(tipo_cliente));
        }
    }

    for (const auto& cliente : clientes) {
        Ventanilla* ventanilla = &ventanillas[0];
        for (auto& v : ventanillas) {
            if ((v.tiempo_atencion + v.tiempo_sin_atender) < (ventanilla->tiempo_atencion + ventanilla->tiempo_sin_atender)) {
                ventanilla = &v;
            }
        }
        ventanilla->atenderCliente(cliente);
        ventanilla->sinAtender();
    }

    double total_tiempo_atencion = 0;
    double total_tiempo_sin_atender = 0;
    for (const auto& v : ventanillas) {
        total_tiempo_atencion += v.tiempo_atencion;
        total_tiempo_sin_atender += v.tiempo_sin_atender;
    }

    std::cout << "\nTiempo total de atención en todas las ventanillas: " << total_tiempo_atencion << " minutos\n";
    std::cout << "Tiempo total de no atención en todas las ventanillas: " << total_tiempo_sin_atender << " minutos\n";
}

int main() {
    int num_ventanillas, num_clientes;
    std::cout << "Ingrese el número de ventanillas: ";
    std::cin >> num_ventanillas;
    std::cout << "Ingrese el número de clientes a simular: ";
    std::cin >> num_clientes;

    simularAtencion(num_ventanillas, num_clientes);

    return 0;
}