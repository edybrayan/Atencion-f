import random
import tkinter as tk
from tkinter import ttk
from ttkthemes import ThemedStyle

class Cliente:
    def __init__(self, tipo_cliente):
        self.tipo_cliente = tipo_cliente


class Ventanilla:
    def __init__(self, numero):
        self.numero = numero
        self.tiempo_atencion = 0
        self.tiempo_sin_atender = 0

    def atender_cliente(self, cliente):
        tiempo = random.uniform(1, 10)
        self.tiempo_atencion += tiempo
        return tiempo

    def sin_atender(self):
        veces = random.randint(1, 5)
        tiempo = random.uniform(5, 30)
        self.tiempo_sin_atender += veces * tiempo
        return veces * tiempo


def simular_atencion(num_ventanillas, num_clientes, tree):
    ventanillas = [Ventanilla(i) for i in range(1, num_ventanillas + 1)]

    clientes = []
    tipos_cliente = [
        "Tarjeta - Cuentas comunes",
        "Tarjeta - Personas naturales VIP",
        "Tarjeta - Personas jurídicas comunes",
        "Tarjeta - Personas jurídicas VIP",
        "Sin tarjeta",
        "Preferencial - Mayores de 60 años",
        "Preferencial - Deficiencia física",
        "Preferencial - Necesidades especiales"
    ]
    for _ in range(num_clientes):
        tipo_cliente = random.choices(
            tipos_cliente,
            weights=[0.28, 0.21, 0.07, 0.07, 0.2, 0.06, 0.02, 0.02],
            k=1
        )[0]
        clientes.append(Cliente(tipo_cliente))

    for cliente in clientes:
        ventanilla = min(ventanillas, key=lambda v: v.tiempo_atencion + v.tiempo_sin_atender)
        tiempo_atencion = ventanilla.atender_cliente(cliente)
        tiempo_sin_atender = ventanilla.sin_atender()
        tree.insert("", 'end', values=(cliente.tipo_cliente, ventanilla.numero, tiempo_atencion, tiempo_sin_atender))

    total_tiempo_atencion = sum(v.tiempo_atencion for v in ventanillas)
    total_tiempo_sin_atender = sum(v.tiempo_sin_atender for v in ventanillas)

    tk.messagebox.showinfo("Resultados", f"Tiempo total de atención en todas las ventanillas: {total_tiempo_atencion} minutos\n"
                                        f"Tiempo total de no atención en todas las ventanillas: {total_tiempo_sin_atender} minutos")


def main():
    root = tk.Tk()
    style = ThemedStyle(root)
    style.set_theme("arc")  # Set the theme

    tk.Label(root, text="Número de ventanillas:").grid(row=0)
    tk.Label(root, text="Número de clientes:").grid(row=1)

    ventanillas_entry = tk.Entry(root)
    clientes_entry = tk.Entry(root)

    ventanillas_entry.grid(row=0, column=1)
    clientes_entry.grid(row=1, column=1)

    tree = ttk.Treeview(root)
    tree["columns"] = ("Cliente", "Ventanilla", "Tiempo Atencion", "Tiempo Sin Atender")
    tree.column("#0", width=0, minwidth=0, stretch=tk.NO)
    tree.heading("Cliente", text="Tipo Cliente", anchor=tk.W)
    tree.heading("Ventanilla", text="Ventanilla", anchor=tk.W)
    tree.heading("Tiempo Atencion", text="Tiempo Atencion", anchor=tk.W)
    tree.heading("Tiempo Sin Atender", text="Tiempo Sin Atender", anchor=tk.W)

    tree.grid(row=3, column=0, columnspan=2)

    ttk.Button(root, text="Simular", command=lambda: simular_atencion(int(ventanillas_entry.get()), int(clientes_entry.get()), tree)).grid(row=2, column=1, sticky=tk.W, pady=4)

    root.mainloop()


if __name__ == "__main__":
    main()
