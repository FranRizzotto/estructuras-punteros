#include <iostream>
#include <stdio.h>
#include <string.h>
#include <iomanip>

using namespace std;

//==================================================
// ARCHIVOS BASE
//==================================================

struct Empleado {
    char codEmp[10];
    char nombYApe[51];
    int cantProdVend;
};

struct Venta {
    char codEmp[10];
    int codProd;
    int fecha;
    float precioVenta;
};

//==================================================
// PILA DE PRODUCTOS (LISTA ORDENADA POR FECHA)
//==================================================

struct InfoP {
    int codP;
    int fecha;
};

struct NodoP {
    InfoP infoP;
    NodoP* sig;
};

//==================================================
// VECTOR DETALLE
//==================================================

struct Detalle {
    char codEmp[10];
    char nombYApe[51];
    int cantArt;
    float totalRec;
    NodoP* inicioLista;
};

//==================================================
// CREAR Y MOSTRAR ARCHIVOS
//==================================================

void crearEmpleados() {
    FILE* empleados = fopen("Empleados.dat", "wb");

    Empleado vecE[4] = {
        {"EE", "Juan Gomez", 50},
        {"AA", "Pablo Perez", 100},
        {"ZZ", "Ariel Gimenez", 10},
        {"NN", "Roberto Diaz", 200}
    };

    fwrite(vecE, sizeof(Empleado), 4, empleados);
    fclose(empleados);
}

void mostrarEmpleados() {
    FILE* empleados = fopen("Empleados.dat", "rb");
    Empleado e;

    cout << "\nEMPLEADOS\n";
    cout << "------------------------------------------\n";
    while (fread(&e, sizeof(Empleado), 1, empleados) == 1) {
        cout << e.codEmp << " | "
            << setw(20) << left << e.nombYApe
            << " | " << e.cantProdVend << endl;
    }
    fclose(empleados);
}

void crearVentas() {
    FILE* ventas = fopen("Ventas.dat", "wb");

    Venta vecV[11] = {
        {"EE", 1, 20141001, 40},
        {"ZZ", 5, 20141001, 30},
        {"EE", 2, 20141002, 15},
        {"AA", 5, 20141002, 30},
        {"AA", 5, 20141002, 30},
        {"EE", 2, 20141002, 15},
        {"ZZ", 3, 20141004, 70},
        {"ZZ", 3, 20141010, 70},
        {"EE", 2, 20141011, 15},
        {"ZZ", 4, 20141015, 60},
        {"ZZ", 5, 20141015, 30}
    };

    fwrite(vecV, sizeof(Venta), 11, ventas);
    fclose(ventas);
}

void mostrarVentas() {
    FILE* ventas = fopen("Ventas.dat", "rb");
    Venta v;

    cout << "\nVENTAS\n";
    cout << "------------------------------------------\n";
    while (fread(&v, sizeof(Venta), 1, ventas) == 1) {
        cout << v.codEmp << " | "
            << v.codProd << " | "
            << v.fecha << " | $"
            << fixed << setprecision(2) << v.precioVenta << endl;
    }
    fclose(ventas);
}

//==================================================
// FUNCIONES PILA (LISTA ORDENADA)
//==================================================

void pushOrdenado(NodoP*& inicio, InfoP valor) {
    NodoP* nuevo = new NodoP;
    nuevo->infoP = valor;
    nuevo->sig = NULL;

    if (inicio == NULL || valor.fecha > inicio->infoP.fecha) {
        nuevo->sig = inicio;
        inicio = nuevo;
        return;
    }

    NodoP* act = inicio;
    while (act->sig != NULL && act->sig->infoP.fecha >= valor.fecha) {
        act = act->sig;
    }

    nuevo->sig = act->sig;
    act->sig = nuevo;
}

void mostrarLista(NodoP* inicio) {
    if (inicio == NULL) {
        cout << "   (Sin productos)\n";
        return;
    }

    cout << "   CodProd   Fecha\n";
    while (inicio != NULL) {
        cout << "   "
            << setw(8) << inicio->infoP.codP
            << inicio->infoP.fecha << endl;
        inicio = inicio->sig;
    }
}

void liberarPila(NodoP*& inicio) {
    while (inicio != NULL) {
        NodoP* aux = inicio;
        inicio = inicio->sig;
        delete aux;
    }
}

//==================================================
// FUNCIONES GENERALES
//==================================================

void inicializar(int& len) {
    len = 0;
}

void agregar(Detalle arr[], int& len, Detalle d) {
    arr[len++] = d;
}

int buscar(Detalle arr[], int len, char cod[]) {
    for (int i = 0; i < len; i++) {
        if (strcmp(arr[i].codEmp, cod) == 0)
            return i;
    }
    return -1;
}

void ordenar(Detalle arr[], int len) {
    for (int i = 0; i < len - 1; i++) {
        for (int j = 0; j < len - 1 - i; j++) {
            if (arr[j].totalRec < arr[j + 1].totalRec) {
                Detalle aux = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = aux;
            }
        }
    }
}

//==================================================
// CARGA DE DATOS
//==================================================

void cargarEmpleados(Detalle arr[], int& len) {
    FILE* empleados = fopen("Empleados.dat", "rb");
    Empleado e;

    while (fread(&e, sizeof(Empleado), 1, empleados) == 1) {
        Detalle d;
        strcpy(d.codEmp, e.codEmp);
        strcpy(d.nombYApe, e.nombYApe);
        d.cantArt = 0;
        d.totalRec = 0;
        d.inicioLista = NULL;
        agregar(arr, len, d);
    }
    fclose(empleados);
}

void cargarVentas(Detalle arr[], int len) {
    FILE* ventas = fopen("Ventas.dat", "rb");
    Venta v;

    while (fread(&v, sizeof(Venta), 1, ventas) == 1) {
        int pos = buscar(arr, len, v.codEmp);
        if (pos != -1) {
            arr[pos].totalRec += v.precioVenta;
            arr[pos].cantArt++;

            InfoP info;
            info.codP = v.codProd;
            info.fecha = v.fecha;
            pushOrdenado(arr[pos].inicioLista, info);
        }
    }
    fclose(ventas);
}

//==================================================
// MOSTRAR REPORTE FINAL
//==================================================

void mostrarDetalle(Detalle arr[], int len) {
    cout << "\n================== REPORTE FINAL ==================\n";

    for (int i = 0; i < len; i++) {
        cout << "\nEmpleado: " << arr[i].codEmp
            << " - " << arr[i].nombYApe << endl;

        cout << "Total artículos vendidos: " << arr[i].cantArt << endl;
        cout << "Total recaudado: $"
            << fixed << setprecision(2) << arr[i].totalRec << endl;

        cout << "Productos vendidos:\n";
        mostrarLista(arr[i].inicioLista);
    }

    cout << "\n==================================================\n";
}

//==================================================
// RESOLUCIÓN TP
//==================================================

void resolucionTp() {
    Detalle detalle[50];
    int len;

    inicializar(len);
    cargarEmpleados(detalle, len);
    cargarVentas(detalle, len);
    ordenar(detalle, len);
    mostrarDetalle(detalle, len);

    for (int i = 0; i < len; i++) {
        liberarPila(detalle[i].inicioLista);
    }
}

//==================================================
// MAIN
//==================================================

int main() {
    crearEmpleados();
    crearVentas();

    mostrarEmpleados();
    mostrarVentas();

    resolucionTp();
    return 0;
}