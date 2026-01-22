#include <iostream>
#include <stdio.h>
#include <string.h>
#include <iomanip>

using namespace std;

// =======================
// ESTRUCTURAS
// =======================

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

struct Detalle {
    char codEmp[10];
    char nombYApe[51];
    int cantArt;
    float totalRec;
};

// =======================
// CREACIÓN DE ARCHIVOS
// =======================

void crearEmpleados() {
    FILE* empleados = fopen("Empleados.dat", "wb");

    Empleado vecE[4] = {
        { "EE", "Juan Gomez", 50 },
        { "AA", "Pablo Perez", 100 },
        { "ZZ", "Ariel Gimenez", 10 },
        { "NN", "Roberto Diaz", 200 }
    };

    fwrite(vecE, sizeof(Empleado), 4, empleados);
    fclose(empleados);
}

void crearVentas() {
    FILE* ventas = fopen("Ventas.dat", "wb");

    Venta vecV[11] = {
        { "EE", 1, 20141001, 40.0 },
        { "ZZ", 5, 20141001, 30.0 },
        { "EE", 2, 20141002, 15.0 },
        { "AA", 5, 20141002, 30.0 },
        { "AA", 5, 20141002, 30.0 },
        { "EE", 2, 20141002, 15.0 },
        { "ZZ", 3, 20141004, 70.0 },
        { "ZZ", 3, 20141010, 70.0 },
        { "EE", 2, 20141011, 15.0 },
        { "ZZ", 4, 20141015, 60.0 },
        { "ZZ", 5, 20141015, 30.0 }
    };

    fwrite(vecV, sizeof(Venta), 11, ventas);
    fclose(ventas);
}

// =======================
// MOSTRAR ARCHIVOS
// =======================

void mostrarEmpleados() {
    FILE* empleados = fopen("Empleados.dat", "rb");
    Empleado e;

    cout << "\nEMPLEADOS\n";
    cout << "---------------------------------------------\n";
    while (fread(&e, sizeof(Empleado), 1, empleados) == 1) {
        cout << e.codEmp << " - "
            << e.nombYApe << " - "
            << e.cantProdVend << endl;
    }
    fclose(empleados);
}

void mostrarVentas() {
    FILE* ventas = fopen("Ventas.dat", "rb");
    Venta v;

    cout << "\nVENTAS\n";
    cout << "---------------------------------------------\n";
    while (fread(&v, sizeof(Venta), 1, ventas) == 1) {
        cout << v.codEmp << " - "
            << v.codProd << " - "
            << v.fecha << " - "
            << v.precioVenta << endl;
    }
    fclose(ventas);
}

// =======================
// FUNCIONES AUXILIARES
// =======================

void inicializar(int& len) {
    len = 0;
}

void agregar(Detalle arr[], int& len, Detalle valor) {
    arr[len] = valor;
    len++;
}

int buscar(Detalle arr[], int len, char cod[]) {
    for (int i = 0; i < len; i++) {
        if (strcmp(arr[i].codEmp, cod) == 0) {
            return i;
        }
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

// =======================
// CARGA DE DATOS
// =======================

void cargarEmpleados(Detalle arr[], int& len) {
    FILE* empleados = fopen("Empleados.dat", "rb");
    Empleado e;

    while (fread(&e, sizeof(Empleado), 1, empleados) == 1) {
        Detalle d;
        strcpy(d.codEmp, e.codEmp);
        strcpy(d.nombYApe, e.nombYApe);
        d.cantArt = e.cantProdVend;
        d.totalRec = 0;

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
        }
    }
    fclose(ventas);
}

// =======================
// MOSTRAR DETALLE (TABLA)
// =======================

void mostrarDetalle(Detalle arr[], int len) {
    cout << "\n===============================================\n";
    cout << left
        << setw(12) << "CodEmp"
        << setw(25) << "Nombre y Apellido"
        << setw(15) << "Cant.Art"
        << setw(15) << "Total $" << endl;
    cout << "-----------------------------------------------\n";

    for (int i = 0; i < len; i++) {
        cout << left
            << setw(12) << arr[i].codEmp
            << setw(25) << arr[i].nombYApe
            << setw(15) << arr[i].cantArt
            << setw(15) << fixed << setprecision(2) << arr[i].totalRec
            << endl;
    }
    cout << "===============================================\n";
}

// =======================
// RESOLUCIÓN TP
// =======================

void resolucionTp() {
    Detalle detalle[50];
    int len;

    inicializar(len);
    cargarEmpleados(detalle, len);
    cargarVentas(detalle, len);
    ordenar(detalle, len);
    mostrarDetalle(detalle, len);
}

// =======================
// MAIN
// =======================

int main() {
    crearEmpleados();
    crearVentas();

    mostrarEmpleados();
    mostrarVentas();

    resolucionTp();

    return 0;
}