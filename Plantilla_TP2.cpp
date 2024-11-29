#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>  // Para usar stringstream
#include <string>
using namespace std;
// Estructura para representar un movimiento
struct Movimiento
{
    string nombre;
    string contrasenia;
    string monto;
    string tipo;
    string actividad;
};

// Estructura para representar un plazo fijo
struct PlazoFijo
{
    float monto = 0;
    int dias = 0;
    float interes = 0;
};

// Estructura para representar una cuenta bancaria
struct Cuenta
{
    // Datos de la cuenta bancaria
    string nombre;
    string contrasenia;
    float saldo = 0;
    float dolares = 0;
    vector<Movimiento> movimientos;
    vector<PlazoFijo> plazos_fijos;
};

struct Leer_plazos{
        string nombre;
        string contrasenia;
        float monto = 0;
        int dias = 0;
        float interes = 0;
    };

// Función para mostrar el menú principal
void menu_principal(Cuenta usuario)
{
    cout<<""<<endl;
    cout<<""<<endl;
    cout << endl;
    cout << "Bienvenido/a al sistema bancario, " << usuario.nombre << endl;
    cout << "Saldo en cuenta: $ " << usuario.saldo << endl;
    cout << "Dólares: USD$ " << usuario.dolares << endl;
    ifstream archivo;
    int num_plazos=0;
    archivo.open("../plazos.txt", ios::app);
    string linea;
    while(getline(archivo, linea)){
        stringstream ss(linea);  // Crear un stringstream para procesar la línea
        Leer_plazos plazo;
        string tipo, nombre, actividad;
        string fecha;

        getline(ss, plazo.nombre, ';');
        getline(ss, plazo.contrasenia, ';');
        ss >> plazo.monto;  // Leer saldo
        ss.ignore(); // Ignorar el ';'
        ss >> plazo.dias;  // Leer dolares
        ss.ignore();
        ss >> plazo.interes;
        // Saltar el carácter '!' al final de la línea (si existe)
        char check;
        ss >> check;
        if(plazo.nombre == usuario.nombre and plazo.contrasenia == usuario.contrasenia){
            num_plazos++;
        }
    }
    cout << "Plazos fijos armados: " << num_plazos << endl;
    archivo.close();
    cout << endl;
    cout << "1 - Ingresar efectivo." << endl;
    cout << "2 - Retirar efectivo." << endl;
    cout << "3 - Comprar y/o vender dólares." << endl;
    cout << "4 - Retirar dólares." << endl;
    cout << "5 - Armar plazo fijo." << endl;
    cout << "6 - Cancelar plazo fijo." << endl;
    cout << "7 - Pedir préstamo." << endl;
    cout << "8 - Ver movimientos." << endl;
    cout << "9 - Actualizar datos." << endl;
    cout << "10 - Salir del sistema." << endl;
    cout << endl;
    cout << "Elegir una opcion del menú: " << endl;
}

void crear_mov(string usuario, string contraseña,  string accion, int monto, string descripcion){
    ofstream archivoE;
    archivoE.open("../movimientos.txt", ios::app);
    archivoE << usuario << ";" << contraseña << ";" << accion << ";" << monto << ";" << descripcion << ";!" << endl;
    archivoE.close(); 
}

void iniciar_sesion(int &inic, Cuenta &usuario) {

    ifstream archivo;
    archivo.open("../cuentas.txt", ios::app);

    if(archivo.is_open()){
        string nombre, pass, nombreaux, passaux, saldo, dolares;
        string mov_tipo, mov_nombre, mov_actividad;
        string monto, interes, dias; 
        string linea;  // Línea para leer cada entrada en el archivo

        cout << "Ingrese el nombre que desea buscar: ";
        cin >> nombre;
        cout << "Contraseña: ";
        cin >> pass;

        bool encontrado = false;  // Bandera para saber si encontramos el usuario
        while (getline(archivo, linea)) {  // Leemos línea por línea
            string nombreaux, passaux;

            // Usamos un stringstream para separar el nombre y la contraseña
            stringstream ss(linea);
            getline(ss, nombreaux, ';');  // Obtener nombre hasta el ';'
            getline(ss, passaux, ';');  // Obtener contraseña hasta el '!'
            if (nombreaux == nombre) {
                encontrado=true;
                if (passaux == pass) {

                    cout << "¡Login correcto!" << endl;
                    getline(ss, saldo, ';');
                    getline(ss, dolares, ';');
                    usuario.nombre=nombreaux;
                    usuario.contrasenia=passaux;
                    usuario.saldo=stof(saldo);
                    usuario.dolares=stof(dolares);
                    
                    archivo.close();
                    archivo.open("../movimientos.txt", ios::app);
                    while (getline(archivo, linea)) {
                        getline(ss, nombreaux, ';');  // Obtener nombre hasta el ';'
                        getline(ss, passaux, ';');
                        if(nombreaux==nombre && passaux==pass){
                            while (getline(ss, mov_tipo, ';')) {
                                if (mov_tipo == "!") {
                                    break;
                                }
                                Movimiento mov;
                                mov.tipo = mov_tipo;
                                getline(ss, mov_nombre, ';');
                                getline(ss, mov_actividad, ';');
                                mov.nombre = mov_nombre;
                                mov.actividad = mov_actividad;
                                usuario.movimientos.push_back(mov);
                            } 
                        } 
                    }
                    archivo.close();
                    archivo.open("../plazos.txt", ios::app);
                    while (getline(archivo, linea)) {
                        getline(ss, nombreaux, ';');  // Obtener nombre hasta el ';'
                        getline(ss, passaux, ';');
                        if(nombreaux==nombre && passaux==pass){
                            while (getline(ss, monto, ';')) {
                                if (monto == "!") {
                                    break;
                                }
                                PlazoFijo plazo;
                                plazo.monto = stof(monto);
                                getline(ss, interes, ';');
                                getline(ss, dias, ';');
                                plazo.interes = stof(interes);
                                plazo.dias = stoi(dias);
                                usuario.plazos_fijos.push_back(plazo);
                                cout<<"monto del plazo ="<< plazo.monto<<endl;
                                cout<<"interés del plazo"<<plazo.interes<<endl;
                                cout<<"días del plazo "<<plazo.dias<<endl;
                            } 
                        } 
                    }
                    
                    
                    return;



                    //meter todos los getline en usuario
                    
                } else {
                    cout << "Contraseña incorrecta." << endl;
                    inic=0;
                    return;
                }
            }
        }

        if (!encontrado) {
            cout << "Nombre de usuario no encontrado." << endl;
            inic=0;
            return;
        }

        archivo.close();  // No olvides cerrar el archivo después de usarlo
    } else {
        cout << "No se pudo abrir el archivo." << endl;
        inic=0;
        return;
    }

    return;
}
void registro(int &inic) {
    ofstream archivoE;
    ifstream archivoL;
    archivoL.open("../cuentas.txt", ios::app);
    string nombre="";
    string pass="";
    string linea;
    if(archivoL.is_open()){
        cout<<"Registrate. Ingresa tu nombre de usuario"<<endl;
        cin>>nombre;
        cout<<"Ingrese la contraseña"<<endl;
        cin>>pass;
        while (getline(archivoL, linea)) {  // Leemos línea por línea
            string nombreaux, passaux;
            stringstream ss(linea);
            getline(ss, nombreaux, ';');  // Obtener nombre hasta el ';'
            getline(ss, passaux, ';');   // Obtener contraseña hasta el '!'
            if (nombreaux == nombre) {
                    cout << "ERROR: esa cuenta ya existe. Para iniciar sesión, vaya a la opción 'iniciar sesión' en el menú"<< endl;
                    inic=0;
                    return;
            }
        }
    archivoL.close();
    cout<<"Usuario creado correctamente"<<endl;
    archivoE.open("../cuentas.txt", ios::app);
        archivoE << nombre << ";" << pass << ";0;0;!" << "\n" <<endl;
    archivoE.close();
    archivoE.open("../plazos.txt", ios::app);
        archivoE << nombre << ";" << pass << ";!" << "\n" <<endl;
    archivoE.close();

        inic=0;
        return;
    }
    else{
        cout<<"Error al abrir el archivo"<<endl;
        inic=0;
        return;
    }
}
// 1
void ingresar_efectivo(Cuenta &usuario, int opcion) {
    ifstream archivo;
    archivo.open("../cuentas.txt", ios::app);
    float cant_efec=0;
    string linea;
    vector<Cuenta> cuentas;
    cout<<"Indique la cantidad de efectivo que quiera ingresar: "<<endl;
    cin>>cant_efec;

    while(getline(archivo, linea)){
        stringstream ss(linea);  // Crear un stringstream para procesar la línea
        Cuenta cuenta;
        string tipo, nombre, actividad;
        string fecha;

        // Leer los datos de la cuenta (nombre, contrasenia, saldo, dolares)
        getline(ss, cuenta.nombre, ';');
        getline(ss, cuenta.contrasenia, ';');
        ss >> cuenta.saldo;  // Leer saldo
        ss.ignore(); // Ignorar el ';'
        ss >> cuenta.dolares;  // Leer dolares

        // Saltar el carácter '!' al final de la línea (si existe)
        char check;
        ss >> check;
        cuentas.push_back(cuenta);
    }

    for(int i=0;i<cuentas.size();i++){
        if(usuario.nombre == cuentas[i].nombre){
            if(usuario.contrasenia == cuentas[i].contrasenia){
                usuario.saldo=usuario.saldo+cant_efec;
                cuentas[i].saldo=usuario.saldo;
                break;
            }
        }
    }

    archivo.close();

    ofstream archivoE;
    archivoE.open("../cuentas.txt", ios::out);

    for(Cuenta elemento : cuentas){
        archivoE << elemento.nombre << ";" << elemento.contrasenia << ";" << elemento.saldo << ";" << elemento.dolares << ";!" << endl;
    }
    archivoE.close();
    crear_mov(usuario.nombre, usuario.contrasenia, "Ingreso de efectivo",cant_efec, "Se ingresaron "+to_string(cant_efec));
}   

// 2
void retirar_efectivo(Cuenta &usuario, int opcion) {
    ifstream archivo;
    archivo.open("../cuentas.txt", ios::app);
    string linea;
    vector<Cuenta> cuentas;
    int cant_efec;
    while(true){
        cant_efec=0;
        cout<<"Indique la cantidad de efectivo que quiera retirar. (multiplo de 100): "<<endl;
        cin>>cant_efec;
        if(cant_efec % 100==0){
            break;
        }
        else{
            cout<<"Ingrese una cantidad de efectivo a retirar válida"<<endl;
        }
    }

    while(getline(archivo, linea)){
        stringstream ss(linea);  // Crear un stringstream para procesar la línea
        Cuenta cuenta;
        string tipo, nombre, actividad;
        string fecha;

        // Leer los datos de la cuenta (nombre, contrasenia, saldo, dolares)
        getline(ss, cuenta.nombre, ';');
        getline(ss, cuenta.contrasenia, ';');
        ss >> cuenta.saldo;  // Leer saldo
        ss.ignore(); // Ignorar el ';'
        ss >> cuenta.dolares;  // Leer dolares

        // Saltar el carácter '!' al final de la línea (si existe)
        char check;
        ss >> check;
        cuentas.push_back(cuenta);
    }

    for(int i=0;i<cuentas.size();i++){
        if(usuario.nombre == cuentas[i].nombre){
            if(usuario.contrasenia == cuentas[i].contrasenia){
                if(usuario.saldo<cant_efec){
                    cout<<"No se puede extraer esa cantidad de dinero"<<endl;
                    break;
                }
                else{
                    usuario.saldo=usuario.saldo-cant_efec;
                    cuentas[i].saldo=usuario.saldo;
                    int vuelto1000=0, vuelto500=0, vuelto200=0, vuelto100=0;
                    cout<<cant_efec<<endl;
                    if(cant_efec>=1000){
                        for(int i=0 ; cant_efec>=1000 ; i++){
                            cant_efec=cant_efec-1000;
                            vuelto1000=i+1;
                        }
                    }
                    if(cant_efec>=500){
                        for(int i=0 ; cant_efec>=500 ; i++){
                            cant_efec=cant_efec-500;
                            vuelto500=i+1;
                        }
                    }
                    if(cant_efec>=200){
                        for(int i=0 ; cant_efec>=200 ; i++){
                            cant_efec=cant_efec-200;
                            vuelto200=i+1;
                        }
                    }
                    if(cant_efec>=100){
                        for(int i=0 ; cant_efec>=100 ; i++){
                            cant_efec=cant_efec-100;
                            vuelto100=i+1;
                        }
                    }
                    cout<<"Cantidad de billetes:"<<endl;
                    cout<<"1000= "<< vuelto1000 <<endl;
                    cout<<"500= "<< vuelto500 <<endl;
                    cout<<"200= "<< vuelto200 <<endl;
                    cout<<"100= "<< vuelto100 <<endl;                  
            }
        }
    }

    archivo.close();

    ofstream archivoE;
    archivoE.open("../cuentas.txt", ios::out);
    for(Cuenta elemento : cuentas){
        archivoE << elemento.nombre << ";" << elemento.contrasenia << ";" << elemento.saldo << ";" << elemento.dolares << ";!" << endl;
    }

    archivoE.close();
}
crear_mov(usuario.nombre, usuario.contrasenia, "Retiro de efectivo",cant_efec, "Se retiraron "+to_string(cant_efec));
}

// 3
void manejar_dolares(Cuenta &usuario, int opcion){
    ifstream archivo;
    archivo.open("../cuentas.txt", ios::app);
    string linea;
    float cant_efec=0;
    vector<Cuenta> cuentas;

    while(getline(archivo, linea)){
        stringstream ss(linea);  // Crear un stringstream para procesar la línea
        Cuenta cuenta;
        string tipo, nombre, actividad;
        string fecha;

        // Leer los datos de la cuenta (nombre, contrasenia, saldo, dolares)
        getline(ss, cuenta.nombre, ';');
        getline(ss, cuenta.contrasenia, ';');
        ss >> cuenta.saldo;  // Leer saldo
        ss.ignore(); // Ignorar el ';'
        ss >> cuenta.dolares;  // Leer dolares

        // Saltar el carácter '!' al final de la línea (si existe)
        char check;
        ss >> check;
        cuentas.push_back(cuenta);
    }

    int eleccion;
    cout<<"1-Vender Dólares\n2-Comprar Dólares"<<endl;
    while(true){
        eleccion=0;
        cin>>eleccion;
        if(eleccion!=1 and eleccion!=2){
            cout<<"Elija una opción válida"<<endl;
        }
        else{
            break;
        }
    }

    for(int i=0;i<cuentas.size();i++){
        if(usuario.nombre == cuentas[i].nombre){
            if(usuario.contrasenia == cuentas[i].contrasenia){
                if(eleccion==1){
                    if(cuentas[i].dolares>10){
                        cout<<"Ingrese la cantidad de dólares que quiera vender: "<<endl;
                        while(true){
                            cin>>cant_efec;
                            if(cant_efec>cuentas[i].dolares){
                                cout<<"No se puede realizar la operación. Elija un valor dentro del rango"<<endl;
                            }
                            else{
                                break;
                            }
                        }
                        cout<<"La cantidad en pesos de su venta: "<<cant_efec*1000<<endl;
                        usuario.saldo=usuario.saldo+(cant_efec*1000);
                        cuentas[i].saldo=usuario.saldo;
                        usuario.dolares=usuario.dolares-cant_efec;
                        cuentas[i].dolares=usuario.dolares;
                    }
                    else{
                        cout<<"No puede realizar esta opción. No cumple con los requisitos mínimos."<<endl;
                    }
                } else{
                    if(usuario.saldo<1000){
                        cout<<"No puede comprar ningún dolar"<<endl;
                    }
                    else{
                        cout<<"Ingrese la cantidad de dólares que quiera comprar: "<<endl;
                        while(true){
                            cin>>cant_efec;
                            if(cant_efec*1000>cuentas[i].saldo){
                                cout<<"No se puede realizar la operación. Elija un valor dentro del rango"<<endl;
                            }
                            else{
                                break;
                            }
                    }
                    cout<<"La cantidad en pesos de su compra: "<<cant_efec*1000<<endl;
                    usuario.saldo=usuario.saldo-(cant_efec*1000);
                    cuentas[i].saldo=usuario.saldo;
                    usuario.dolares=usuario.dolares+cant_efec;
                    cuentas[i].dolares=usuario.dolares;
                    }
                }
            }
        }
    }
    
    archivo.close();

    ofstream archivoE;
    archivoE.open("../cuentas.txt", ios::out);
    for(Cuenta elemento : cuentas){
        archivoE << elemento.nombre << ";" << elemento.contrasenia << ";" << elemento.saldo << ";" << elemento.dolares << ";!" << endl;
    }

    archivoE.close();
    crear_mov(usuario.nombre, usuario.contrasenia, "Comprar",cant_efec, "Se compraron "+to_string(cant_efec)+"dólares");
}

// 4
void retirar_dolares(Cuenta &usuario, int opcion) {
    ifstream archivo;
    archivo.open("../cuentas.txt", ios::app);
    string linea;
    int cant_efec;
    vector<Cuenta> cuentas;

    while(true){
        cant_efec=0;
        cout<<"Indique la cantidad de efectivo que quiera retirar. (multiplo de 10): "<<endl;
        cin>>cant_efec;
        if(cant_efec%10==0){
            break;
        }
        else{
            cout<<"Ingrese una cantidad de efectivo a retirar válida"<<endl;
        }
    }

    while(getline(archivo, linea)){
        stringstream ss(linea);  // Crear un stringstream para procesar la línea
        Cuenta cuenta;
        string tipo, nombre, actividad;
        string fecha;

        // Leer los datos de la cuenta (nombre, contrasenia, saldo, dolares)
        getline(ss, cuenta.nombre, ';');
        getline(ss, cuenta.contrasenia, ';');
        ss >> cuenta.saldo;  // Leer saldo
        ss.ignore(); // Ignorar el ';'
        ss >> cuenta.dolares;  // Leer dolares

        // Saltar el carácter '!' al final de la línea (si existe)
        char check;
        ss >> check;
        cuentas.push_back(cuenta);
    }



    for(int i=0;i<cuentas.size();i++){
        if(usuario.nombre == cuentas[i].nombre){
            if(usuario.contrasenia == cuentas[i].contrasenia){
                if(usuario.dolares<cant_efec){
                    cout<<"No se puede extraer esa cantidad de dinero"<<endl;
                    break;
                }
                else{
                    usuario.dolares=usuario.dolares-cant_efec;
                    cuentas[i].dolares=usuario.dolares;
                    int vuelto100=0, vuelto50=0, vuelto20=0, vuelto10=0;
                    cout<<cant_efec<<endl;
                    if(cant_efec>=100){
                        for(int i=0 ; cant_efec>=100 ; i++){
                            cant_efec=cant_efec-100;
                            vuelto100=i+1;
                        }
                    }
                    if(cant_efec>=50){
                        for(int i=0 ; cant_efec>=50 ; i++){
                            cant_efec=cant_efec-50;
                            vuelto50=i+1;
                        }
                    }
                    if(cant_efec>=20){
                        for(int i=0 ; cant_efec>=20 ; i++){
                            cant_efec=cant_efec-20;
                            vuelto20=i+1;
                        }
                    }
                    if(cant_efec>=10){
                        for(int i=0 ; cant_efec>=10 ; i++){
                            cant_efec=cant_efec-10;
                            vuelto100=i+1;
                        }
                    }
                    cout<<"Cantidad de billetes:"<<endl;
                    cout<<"100= "<< vuelto100 <<endl;
                    cout<<"50= "<< vuelto50 <<endl;
                    cout<<"20= "<< vuelto20 <<endl;
                    cout<<"10= "<< vuelto10 <<endl;                  
            }
        }
    }

    archivo.close();

    ofstream archivoE;
    archivoE.open("../cuentas.txt", ios::out);
    for(Cuenta elemento : cuentas){
        archivoE << elemento.nombre << ";" << elemento.contrasenia << ";" << elemento.saldo << ";" << elemento.dolares << ";!" << endl;
    }

    archivoE.close();
}
crear_mov(usuario.nombre, usuario.contrasenia, "Venta de dólares",cant_efec, "Se vendieron "+to_string(cant_efec)+"dólares");
}

// 5
void armar_plazo_fijo(Cuenta &usuario, int opcion, PlazoFijo    &plazo_fijo) {
    ifstream archivo;
    archivo.open("../plazos.txt", ios::app);
    string linea;
    int dias=0;
    float cant_efec=0;
    float interes=0;
    vector<Leer_plazos> plazos;
    while(getline(archivo, linea)){
        stringstream ss(linea);  // Crear un stringstream para procesar la línea
        Leer_plazos plazo;
        string tipo, nombre, actividad;
        string fecha;

        getline(ss, plazo.nombre, ';');
        getline(ss, plazo.contrasenia, ';');
        ss >> plazo.monto;  // Leer saldo
        ss.ignore(); // Ignorar el ';'
        ss >> plazo.dias;  // Leer dolares
        ss.ignore();
        ss >> plazo.interes;
        // Saltar el carácter '!' al final de la línea (si existe)
        char check;
        ss >> check;
        plazos.push_back(plazo);
    }

    cout<<"\nIngrese la cantidad de dinero que quiera ingresar en el plazo fijo: ";
    while(true){
        cin>>cant_efec;
        if(cant_efec>usuario.saldo){
            cout<<"Elija un valor correcto: "<<endl;
        }
        else{
            break;
        }
    }
    cout<<"\nIngrese la cantidad de dias que quiera que este activo el plazo fijo (Mayor a 30 dias): ";
    while(true){
        cin>>dias;
        if(dias<30){
            cout<<"Elija un valor correcto: "<<endl;
        }
        else{
            break;
        }
    }
    cout<<"\n";

    interes=(cant_efec * dias * 70)/36500;
    Leer_plazos aux;
    aux.nombre=usuario.nombre;
    aux.contrasenia=usuario.contrasenia;
    aux.monto=cant_efec;
    aux.dias=dias;
    aux.interes=interes;
    plazos.push_back(aux);

    PlazoFijo aux1;
    aux1.monto=cant_efec;
    aux1.dias=dias;
    aux1.interes=interes;

    Cuenta aux2;
    aux2.nombre=usuario.nombre;
    aux2.contrasenia=usuario.contrasenia;
    aux2.saldo=usuario.saldo;
    aux2.dolares=usuario.dolares;
    aux2.movimientos=usuario.movimientos;
    aux2.plazos_fijos.push_back(aux1);
    usuario.saldo=usuario.saldo-cant_efec;

    cout<<"Plata Ingresada: "<<cant_efec<<endl;
    cout<<"Días Ingresados: "<<dias<<endl;
    cout<<"Interés que va a generar: "<<interes<<endl;

    archivo.close();

    ofstream archivoE;
    archivoE.open("../plazos.txt", ios::out);
    for(Leer_plazos elemento : plazos){
        archivoE << elemento.nombre << ";" << elemento.contrasenia << ";" << elemento.monto << ";" << elemento.dias << ";" << elemento.interes << ";!" << endl;
    }
    //Agregar varios plazos fijos.

    archivoE.close();

    ifstream archivoO;
    archivoO.open("../cuentas.txt", ios::app);
    string lineaO;
    vector<Cuenta> cuentas;
    while(getline(archivoO, lineaO)){
        stringstream ss(lineaO);  // Crear un stringstream para procesar la línea
        Cuenta cuenta;
        string tipo, nombre, actividad;
        string fecha;

        // Leer los datos de la cuenta (nombre, contrasenia, saldo, dolares)
        getline(ss, cuenta.nombre, ';');
        getline(ss, cuenta.contrasenia, ';');
        ss >> cuenta.saldo;  // Leer saldo
        ss.ignore(); // Ignorar el ';'
        ss >> cuenta.dolares;  // Leer dolares

        // Saltar el carácter '!' al final de la línea (si existe)
        char check;
        ss >> check;
        cuentas.push_back(cuenta);
    }

    for(int i=0;i<cuentas.size();i++){
            if(usuario.nombre == cuentas[i].nombre){
                if(usuario.contrasenia == cuentas[i].contrasenia){
                    cuentas[i].saldo=cuentas[i].saldo-cant_efec;
                    
                }
            }
        }

        archivoO.close();

        ofstream archivoES;
        archivoES.open("../cuentas.txt", ios::out);
        for(Cuenta elemento : cuentas){
            archivoES << elemento.nombre << ";" << elemento.contrasenia << ";" << elemento.saldo << ";" << elemento.dolares<< ";!" << endl;
        }

        archivoES.close();
        crear_mov(usuario.nombre, usuario.contrasenia, "Creación de plazo fijo",cant_efec, "Se creó un plazo fijo con una inversión inicial de "+to_string(cant_efec)+", por un tiempo de "+to_string(dias)+"días, que tiene un beneficio final de "+to_string(interes));
}

// 6
void cancelar_plazo_fijo(Cuenta &usuario, int opcion) {
    ifstream archivo;
    archivo.open("../plazos.txt", ios::app);
    string linea;
    vector<Leer_plazos> plazos_aux;
    vector<Leer_plazos> plazos;
    vector<Cuenta> cuentas;
    vector<int> j;
    while(getline(archivo, linea)){
        stringstream ss(linea);  // Crear un stringstream para procesar la línea
        Leer_plazos plazo;
        string tipo, nombre, actividad;
        string fecha;

        getline(ss, plazo.nombre, ';');
        getline(ss, plazo.contrasenia, ';');
        ss >> plazo.monto;  // Leer saldo
        ss.ignore(); // Ignorar el ';'
        ss >> plazo.dias;  // Leer dolares
        ss.ignore();
        ss >> plazo.interes;
        // Saltar el carácter '!' al final de la línea (si existe)
        char check;
        ss >> check;
        plazos.push_back(plazo);
    }
    int cual=0;
    int b=0;
    float vuelto;
    for(b;b<plazos.size();b++){
        if(usuario.nombre == plazos[b].nombre){
            if(usuario.contrasenia == plazos[b].contrasenia){
                    ifstream archivoO;
                    archivoO.open("../cuentas.txt", ios::app);
                    string lineaO;
                    while(getline(archivoO, lineaO)){
                        stringstream ss(lineaO);  // Crear un stringstream para procesar la línea
                        Cuenta cuenta;
                        string tipo, nombre, actividad;
                        string fecha;

                        // Leer los datos de la cuenta (nombre, contrasenia, saldo, dolares)
                        getline(ss, cuenta.nombre, ';');
                        getline(ss, cuenta.contrasenia, ';');
                        ss >> cuenta.saldo;  // Leer saldo
                        ss.ignore(); // Ignorar el ';'
                        ss >> cuenta.dolares;  // Leer dolares

                        // Saltar el carácter '!' al final de la línea (si existe)
                        char check;
                        ss >> check;
                        cuentas.push_back(cuenta);
                    }
                    bool check_sumar=true;
                    bool check=false;
                    for(int i=0;i<plazos.size();i++){
                        if(usuario.nombre == plazos[i].nombre){
                            if(usuario.contrasenia == plazos[i].contrasenia){
                                cout<<i<<"- Monto: "<<plazos[i].monto<<" Dias: "<<plazos[i].dias<<" Interes: "<<plazos[i].interes<<" Se devolverían: "<<plazos[i].monto<<endl;
                                j.push_back(i);
                                check=true;
                            }
                        }
                    }
                    if(check){
                        int eleccion=-1;
                        cout<<"Elija que plazo quiere cancelar: "<<endl;
                        bool entrar=true;
                        while(entrar){
                            cin>>eleccion;
                            for(int elemento : j){
                                if(eleccion!=elemento){
                                    cout<<"No se puede usar ese índice. Elija otro "<<elemento<<endl;
                                }
                                else{
                                    for(int h=0;h<plazos.size();h++){
                                        cout<<"h: "<<h<<" eleccion: "<<elemento<<endl;
                                        if(eleccion!=h){
                                            Leer_plazos aux;
                                            aux.nombre=plazos[h].nombre;
                                            aux.contrasenia=plazos[h].contrasenia;
                                            aux.monto=plazos[h].monto;
                                            aux.dias=plazos[h].dias;
                                            aux.interes=plazos[h].interes;
                                            plazos_aux.push_back(aux);
                                            entrar=false;
                                            cual=1;
                                        }
                                        else{
                                            for(int i=0;i<cuentas.size();i++){
                                                if(usuario.nombre == cuentas[i].nombre){
                                                    if(usuario.contrasenia == cuentas[i].contrasenia){
                                                        if(check_sumar){
                                                            usuario.saldo=usuario.saldo+plazos[h].monto;
                                                            vuelto=plazos[h].monto;
                                                            cuentas[i].saldo=usuario.saldo;
                                                            check_sumar=false;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                        
                                    }
                                    
                                    
                                }
                            }
                            
                        }
                        break;
                        
                    }
                archivoO.close();
                
            }
        }
        
    }
    if(b>=plazos.size()){
        cout<<"No tienes ningún plazo fijo"<<endl;
        cual=2;
    }
    
    archivo.close(); 
    
    ofstream archivoES;
    archivoES.open("../plazos.txt", ios::out);
    if(cual==1){
        for(Leer_plazos elemento : plazos_aux){
            archivoES << elemento.nombre << ";" << elemento.contrasenia << ";" << elemento.monto << ";" << elemento.dias<< ";" << elemento.interes << ";!" << endl;
        }
    }
    else if(cual==2){
        for(Leer_plazos elemento : plazos){
            archivoES << elemento.nombre << ";" << elemento.contrasenia << ";" << elemento.monto << ";" << elemento.dias<< ";" << elemento.interes << ";!" << endl;
        }
    }

    archivoES.close();

    ofstream archivoC;
    archivoC.open("../cuentas.txt", ios::out);

    for(Cuenta elemento : cuentas){
        archivoC << elemento.nombre << ";" << elemento.contrasenia << ";" << elemento.saldo << ";" << elemento.dolares << ";!" << endl;
    }

    archivoC.close();
    crear_mov(usuario.nombre, usuario.contrasenia, "Cancelación de plazo fijo",vuelto, "Se creó un plazo fijo con una inversión inicial de "+to_string(vuelto));
}

// 7
void pedir_prestamo(Cuenta &usuario, int opcion) {
    ifstream archivoC;
    archivoC.open("../cuentas.txt", ios::app);
    string lineaC;
    vector<Cuenta> cuentas;
    int porcentaje;
    float monto;

    while(getline(archivoC, lineaC)){
        stringstream ss(lineaC);  // Crear un stringstream para procesar la línea
        Cuenta cuenta;
        string tipo, nombre, actividad;

        // Leer los datos de la cuenta (nombre, contrasenia, saldo, dolares)
        getline(ss, cuenta.nombre, ';');
        getline(ss, cuenta.contrasenia, ';');
        ss >> cuenta.saldo;  // Leer saldo
        ss.ignore(); // Ignorar el ';'
        ss >> cuenta.dolares;  // Leer dolares

            // Saltar el carácter '!' al final de la línea (si existe)
        char check;
        ss >> check;
        cuentas.push_back(cuenta);
    }
    int eleccion=0;
    if(usuario.saldo>20000){
        cout<<"1-Prestamo del 5%"<<endl;
        if(usuario.saldo>50000){
            cout<<"2-Prestamo del 10%"<<endl;
            if(usuario.saldo>100000){
                cout<<"3-Prestamo del 15%"<<endl;
            }
        }
    }
    else{
        cout<<"No puedes pedir préstamos"<<endl;
        return;
    }
    cin>>eleccion;
    
    switch(eleccion){
        case 1:
            if(usuario.saldo>20000){
                for(int i=0;i<cuentas.size();i++){
                    if(usuario.nombre == cuentas[i].nombre){
                        if(usuario.contrasenia == cuentas[i].contrasenia){
                            usuario.saldo=usuario.saldo+usuario.saldo*0.05;
                            porcentaje=5;
                            monto=usuario.saldo+usuario.saldo*0.05;
                            cuentas[i].saldo=usuario.saldo;
                        }
                    }
                }
            }
            else{
                cout<<"No puedes elegir esta opción"<<endl;
            }
            break;
        
        case 2:
            if(usuario.saldo>50000){
                for(int i=0;i<cuentas.size();i++){
                    if(usuario.nombre == cuentas[i].nombre){
                        if(usuario.contrasenia == cuentas[i].contrasenia){
                            usuario.saldo=usuario.saldo+usuario.saldo*0.10;
                            porcentaje=10;
                            monto=usuario.saldo+usuario.saldo*0.1;
                            cuentas[i].saldo=usuario.saldo;
                        }
                    }
                }
            }
            else{
                cout<<"No puedes elegir esta opción"<<endl;
            }
            break;
        
        case 3:
            if(usuario.saldo>100000){
                for(int i=0;i<cuentas.size();i++){
                    if(usuario.nombre == cuentas[i].nombre){
                        if(usuario.contrasenia == cuentas[i].contrasenia){
                            usuario.saldo=usuario.saldo+usuario.saldo*0.15;
                            porcentaje=15;
                            monto=usuario.saldo+usuario.saldo*0.15;
                            cuentas[i].saldo=usuario.saldo;
                        }
                    }
                } 
            }
            else{
                cout<<"No puedes elegir esta opción"<<endl;
            }
            break;
    }

    archivoC.close();

    ofstream archivoCE;
    archivoCE.open("../cuentas.txt" , ios::out);
    for(Cuenta elemento : cuentas){
        archivoCE << elemento.nombre << ";" << elemento.contrasenia << ";" << elemento.saldo << ";" << elemento.dolares << ";!" << endl;
    }
    archivoCE.close();
    crear_mov(usuario.nombre, usuario.contrasenia, "Préstamo",monto, "Se pdió un préstamo en el que se brindó al usuario un "+to_string(porcentaje)+"porciento de su efectivo en el momento.");
}
// 8
void ver_movimientos(Cuenta usuario) {
    bool salir=false;
    ifstream archivo;
    archivo.open("../movimientos.txt", ios::app);
    string linea;
    vector<Movimiento> movs;
    while(getline(archivo, linea)){
        stringstream ss(linea);  // Crear un stringstream para procesar la línea
        Movimiento mov;

        getline(ss, mov.nombre, ';');
        getline(ss, mov.contrasenia, ';');
        getline(ss, mov.tipo, ';');
        getline(ss, mov.monto, ';');
        getline(ss, mov.actividad, ';');
        ss.ignore();
        movs.push_back(mov);
    }
    while(!salir){
    cout<<"Aquí se muestran los movimientos de su cuenta, si quiere ver más información sobre alguno, ingrese su índice. Si no, ingrese 0 para volver"<<endl;
    int opcion;
    for(int i=0 ; i<movs.size(); i++){
        if(movs[i].nombre==usuario.nombre){
            if(movs[i].contrasenia==usuario.contrasenia){
                cout<<i+1<<"- "<<movs[i].tipo<<" - "<<movs[i].monto<<endl;
            }
        }
    }
    cout<<""<<endl;
    cin>>opcion;
    cout<<""<<endl;
    if (opcion==0){
        salir=true;
    }
    else{
        for(int i=0 ; i<movs.size(); i++){
            if(movs[i].nombre==usuario.nombre){
                if(movs[i].contrasenia==usuario.contrasenia){
                    if(i==opcion-1){
                        cout<<movs[i].tipo<<endl;
                        cout<<movs[i].monto<<endl;
                        cout<<movs[i].actividad<<endl;
                        cout<<""<<endl;
                    }
                }
            }
        }
    }
}


}

// 9
void actualizar_datos(Cuenta &usuario) {

    ifstream archivo;
    ofstream escritura;
    archivo.open("../cuentas.txt", ios::app);
    string linea;
    vector<Cuenta> cuentas;
    vector<Leer_plazos> plazos;
    vector<Movimiento> movs;

    while(getline(archivo, linea)){
        stringstream ss(linea);  // Crear un stringstream para procesar la línea
        Cuenta cuenta;
        string tipo, nombre, actividad;

        // Leer los datos de la cuenta (nombre, contrasenia, saldo, dolares)
        getline(ss, cuenta.nombre, ';');
        getline(ss, cuenta.contrasenia, ';');
        ss >> cuenta.saldo;  // Leer saldo
        ss.ignore(); // Ignorar el ';'
        ss >> cuenta.dolares;  // Leer dolares

            // Saltar el carácter '!' al final de la línea (si existe)
        char check;
        ss >> check;
        cuentas.push_back(cuenta);
    }
    archivo.close();
    archivo.open("../plazos.txt", ios::app);
    while(getline(archivo, linea)){
        stringstream ss(linea);  // Crear un stringstream para procesar la línea
        Leer_plazos plazo;
        string tipo, nombre, actividad;
        string fecha;
        getline(ss, plazo.nombre, ';');
        getline(ss, plazo.contrasenia, ';');
        ss >> plazo.monto;  // Leer saldo
        ss.ignore(); // Ignorar el ';'
        ss >> plazo.dias;  // Leer dolares
        ss.ignore();
        ss >> plazo.interes;
        // Saltar el carácter '!' al final de la línea (si existe)
        char check;
        ss >> check;
        plazos.push_back(plazo);
    }
    archivo.close();
    archivo.open("../movimientos.txt", ios::app);
    while(getline(archivo, linea)){
        stringstream ss(linea);
        Movimiento mov;

        getline(ss, mov.nombre, ';');
        getline(ss, mov.contrasenia, ';');
        getline(ss, mov.tipo, ';');
        getline(ss, mov.monto, ';');
        getline(ss, mov.actividad, ';');
        ss.ignore();
        movs.push_back(mov);
    }
    archivo.close();
    int option=0;
    cout<<"Por favor, indique qué desea cambiar"<<endl;
    cout<<"1- Nombre"<<endl;
    cout<<"2- Contraseña"<<endl;
    cout<<"3- Cancelar"<<endl;
    cin>>option;
    string nuevo_nom="";
    string nueva_con="";
    bool check=false;
    switch (option){
        case 1:
                cout<<"Elija un nuevo nombre"<<endl;
                cin>>nuevo_nom;
                if (nuevo_nom==usuario.nombre){
                    cout<<"No puedes elegir tu propio nombre."<<endl;
                    break;
                }
                
                for (int i = 0; i < cuentas.size(); i++){
                    if(cuentas[i].nombre==nuevo_nom){
                        cout<<"ese nombre no está disponible"<<endl;
                        check=true;

                    }
                }
                if(!check){
                    for (int i = 0; i < cuentas.size(); i++){
                        if(cuentas[i].nombre==usuario.nombre){
                            cuentas[i].nombre=nuevo_nom;
                        }
                    }
                    for (int i = 0; i < plazos.size(); i++){
                        if(plazos[i].nombre==usuario.nombre){
                            plazos[i].nombre=nuevo_nom;
                        }
                    }
                    for (int i = 0; i < movs.size(); i++){
                        if(movs[i].nombre==usuario.nombre){
                            movs[i].nombre=nuevo_nom;
                        }
                    }
                    usuario.nombre=nuevo_nom;
                    escritura.open("../cuentas.txt" , ios::out);
                    for(Cuenta elemento : cuentas){
                        escritura << elemento.nombre << ";" << elemento.contrasenia << ";" << elemento.saldo << ";" << elemento.dolares << ";!" << endl;
                    }
                    escritura.close();
                    escritura.open("../plazos.txt", ios::out);
                    for(Leer_plazos elemento : plazos){
                        escritura << elemento.nombre << ";" << elemento.contrasenia << ";" << elemento.monto << ";" << elemento.dias<< ";" << elemento.interes << ";!" << endl;
                    }
                    escritura.close();
                    escritura.open("../movimientos.txt", ios::out);
                    for(Movimiento elemento : movs){
                        escritura << elemento.nombre << ";" << elemento.contrasenia << ";" << elemento.tipo << ";" << elemento.monto<< ";" << elemento.actividad << ";!" << endl;
                    }
                    escritura.close();
                    cout<<"Nombre actualizado con éxito"<<endl;             
                }
                check=false;
                
        break;
        case 2:
            cout<<"Elija una nueva contraseña"<<endl;
                cin>>nuevo_nom;
                if (nuevo_nom==usuario.contrasenia){
                    cout<<"No puedes usar tu misma contraseña."<<endl;
                }
                else{
                    cout<<"Escriba de nuevo su contraseña"<<endl;
                    cin>>nueva_con;
                    if (nueva_con==nuevo_nom)
                    {
                            for (int i = 0; i < cuentas.size(); i++){
                                if(cuentas[i].contrasenia==usuario.contrasenia){
                                    cuentas[i].contrasenia=nuevo_nom;
                                }
                            }
                            for (int i = 0; i < plazos.size(); i++){
                                if(plazos[i].contrasenia==usuario.contrasenia){
                                    plazos[i].contrasenia=nuevo_nom;
                                }
                            }
                            for (int i = 0; i < movs.size(); i++){
                                if(movs[i].contrasenia==usuario.contrasenia){
                                    movs[i].contrasenia=nuevo_nom;
                                }
                            }
                            usuario.contrasenia=nuevo_nom;
                            escritura.open("../cuentas.txt" , ios::out);
                            for(Cuenta elemento : cuentas){
                                escritura << elemento.nombre << ";" << elemento.contrasenia << ";" << elemento.saldo << ";" << elemento.dolares << ";!" << endl;
                            }
                            escritura.close();
                            escritura.open("../plazos.txt", ios::out);
                            for(Leer_plazos elemento : plazos){
                                escritura << elemento.nombre << ";" << elemento.contrasenia << ";" << elemento.monto << ";" << elemento.dias<< ";" << elemento.interes << ";!" << endl;
                            }
                            escritura.close();
                            escritura.open("../movimientos.txt", ios::out);
                            for(Movimiento elemento : movs){
                                escritura << elemento.nombre << ";" << elemento.contrasenia << ";" << elemento.tipo << ";" << elemento.monto<< ";" << elemento.actividad << ";!" << endl;
                            }
                            escritura.close();
                            cout<<"Contraseña actualizada con éxito"<<endl;                
                    }
                    else{
                        cout<<"Las contraseñas no coinciden"<<endl;
                    }
                }
            break;
        case 3:
            return;
            break;
        default:
            cout<<"elija una opción válida opción"<<endl;
            break;
    }

}

int main()
{
    PlazoFijo plazo_fijo;
    Cuenta usuario;
    int opcion = 0;
    int inic=0;
    while (inic==0)
    {
        cout<<"Bienvenido al banco Targus"<<endl;
        cout<<"1- Iniciar sesión"<<endl;
        cout<<"2- Registrarse"<<endl;
        cout<<"3- Salir"<<endl;
        cin>>inic;
        switch (inic)
        {
        case 1:
            iniciar_sesion(inic, usuario);
            break;
        case 2:
            registro(inic);
            break;
        case 3:
            cout << "Gracias por utilizar nuestro sistema, vuelva pronto." << endl;
            exit(0);
            break;        
        default:
            cout << "Opción no válida. Intente nuevamente." << endl;
            inic=0;
            break;
        }
    }
    
    


    while (opcion != 10)
    {
        menu_principal(usuario);
        cin >> opcion;

        switch (opcion)
        {
        case 1:
            ingresar_efectivo(usuario, opcion);
            break;
        case 2:
            retirar_efectivo(usuario, opcion);
            break;
        case 3:
            manejar_dolares(usuario, opcion);
            break;
        case 4:
            retirar_dolares(usuario, opcion);
            break;
        case 5:
            armar_plazo_fijo(usuario, opcion, plazo_fijo);
            break;
        case 6:
            cancelar_plazo_fijo(usuario, opcion);
            break;
        case 7:
            pedir_prestamo(usuario, opcion);
            break;
        case 8:
            ver_movimientos(usuario);
            break;
        case 9:
            actualizar_datos(usuario);
            break;
        case 10:
            cout << "Gracias por utilizar nuestro sistema " << usuario.nombre << ", vuelva pronto." << endl;
            exit(0);
            break;
        default:
            cout << "Opción no válida. Intente nuevamente." << endl;
            break;
        }
    }
}
