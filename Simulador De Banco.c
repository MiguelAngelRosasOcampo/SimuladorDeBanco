#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define T 30

typedef struct //Estructura para almacenar la fecha.
{
	short int dia;
	short int mes;
	short int anio;
	
}fechaT;

typedef struct //Estructura para almacenar los datos del cliente.
{
	unsigned int cuenta;	 //4 bytes
	char nombre[T];			//30 bytes
	char apellido[T]; 		//30 bytes
	char correo[T+10];		//40 bytes
	char telefono[T-10];	//20 bytes
	double fondos;					//8 bytes
	short int nip;					//2 bytes
	fechaT fRe; //Fecha de registro //6 bytes
	fechaT fMo; //Fecha movimiento. //6 bytes
	
}clienteT;

void menu(void);

void administrador(void);
void agregarCliente(void);
void mostrarClientes(void);
void buscarCliente(void);
void ordenarClientesName(void);
void editarDatos(void);
void eliminarDatos(void);

unsigned int iniciarSesion(void);
void cliente(unsigned int);
void mostrarInformacion(unsigned int);
void realizarDeposito(unsigned int);
void realizarRetiro(unsigned int);
void transferir(unsigned int);

int main(void)
{	
	srand(time(NULL));
	system("title BANCO C");	
	
	menu();
	
	return 0;
}

void menu()
{
	short int i, modo = 1; //Se inicializa en 1 para que entre en el while una vez
	unsigned int sesion;
	clienteT usuario;
	
	while(modo)
	{	
		system("cls"); //Limpiar la pantalla
		setbuf(stdin,NULL); //Limpiar el buffer
		
		printf("\n\t\t\t\t\t\t   M E N %c ",233);
		printf("\n\t\t"); //Salto de línea y tabulador para mostrar la línea más centrada.
		
		for(i=0; i<80; i++) //Ciclo para ir mostrando una línea de separación.
			printf("%c",220);
			
		printf("\n\n\t\t\t[1] Administrador\t\t\t\t[2] Cliente");
		printf("\n\n\n\n\t\t\t\t\t\t   [0] Salir");
		
		printf("\n\n\n\n\t\t\t\t\t\tModo de acceso: ");
		scanf("%hd",&modo);
		
		switch(modo)
		{			
			case 1: administrador();	break;
			
			case 2: sesion = iniciarSesion();
					if(sesion) //Si "iniciarSesion" retorna 0, entonces la cuenta o el nip son incorrectos.
						cliente(sesion);		
		}		
	}	
}

void administrador()
{
	short int i, opcion = 1; //Se iniciliza "opcion" en 1 (true) para que entre por primera vez al while.
			
	while(opcion)
	{
		system("cls");
		system("color 30");
		setbuf(stdin,NULL);
		
		printf("\n\t\t\t\t\t A D M I N I S T R A D O R");
		printf("\n\t\t"); //Salto de línea y tabulador para mostrar la línea más centrada.
		
		for(i=0; i<80; i++) //Ciclo para ir mostrando una línea de separación.
			printf("%c",220);
			
		printf("\n\n\t\t\t\t\t[1] Agregar nuevo cliente.");
		printf("\n\n\t\t\t\t\t[2] Mostrar todos los clientes.");
		printf("\n\n\t\t\t\t\t[3] Buscar Cliente.");
		printf("\n\n\t\t\t\t\t[4] Ordenar clientes por nombre.");
		printf("\n\n\t\t\t\t\t[5] Editar datos de cuenta.");
		printf("\n\n\t\t\t\t\t[6] Eliminar cuenta.");
		printf("\n\n\t\t\t\t\t[0] Salir.");
		
		printf("\n\n\n\t\t\t\t\tIngrese opci%cn: ",162);
		scanf("%hd",&opcion);

		system("cls");
		switch(opcion)
		{
			case 1: agregarCliente();		break;
			
			case 2: mostrarClientes();		break;
			
			case 3: buscarCliente();		break;
			
			case 4: ordenarClientesName();	break;
			
			case 5: editarDatos();			break;
			
			case 6: eliminarDatos();		
		}
	}
	
	system("color 07");
}

void agregarCliente(void) //Modulo para agregar cliente.
{
	FILE *f;
	clienteT usuario;
	time_t fechaActual;
	struct tm fecha; 
	short int cuentaExistente;
	unsigned int numeroCuenta;
		
	system("color 02");				  
	f = fopen("clientes.dat","ab+"); //Abre el archivo en modo escritura/lectura (si no existe lo crea)
	                             
	if(!f)
	{
		printf("\n\n\t\t\t\t\t\aNo se ha podido abrir el archivo en modo \"ab+\".");
		system("pause>nul");
		exit(-1); //Salir del programa para evitar que se hagan operaciones con el archivo.
	}
	
	do //Bucle para comprobar que el número de cuenta NO se repita.
	{
		cuentaExistente = 0; //Se asume que la cuenta NO va ser repetida.
		numeroCuenta = 100000 + rand() % (1000000 - 100000); //Li + rand() % ( (Ls+1) - Li).
		
		rewind(f); //Regresa el puntero del archivo al inicio, (el puntero es como el cursor).
		
		while(!feof(f) && !cuentaExistente)
		{
			fread(&usuario, sizeof(clienteT), 1, f); //Lee los registros 1 por 1.
			if(usuario.cuenta == numeroCuenta) //Verifica si ese número de cuenta ya existe.
				cuentaExistente = 1; //En caso de existir, entonces variable cambia a 1 (verdadero).
		}
	
	}while(cuentaExistente); //Si variable cambió a verdadero, bucle se vuelve a repetir.
	
	usuario.cuenta = numeroCuenta; //Si sale del bucle, número de cuenta generado NO existé y se almacena.
	
	setbuf(stdin,NULL);
	printf("\n\t\tIngresa el nombre del cliente (25 car%ccteres): ",160);
	fgets(usuario.nombre, T, stdin);
		
	printf("\n\t\tIngresa el apellido del cliente (25 car%ccteres): ",160);
	fgets(usuario.apellido, T, stdin);
		
	printf("\n\t\tIngresa el correo electr%cnico del cliente (35 car%ccteres): ",162,160);	
	fgets(usuario.correo, T+10, stdin);

	printf("\n\t\tIngresa el n%cmero de tel%cfono del cliente: ",163,130);
	fgets(usuario.telefono, T-10, stdin);
		
	printf("\n\t\tIngresa la cantidad de fondos: ");
	scanf("%lf",&usuario.fondos);
	
	printf("\n\t\tIngresa el NIP (4 d%cgitos): ",161);
	scanf("%4hd",&usuario.nip); 
	setbuf(stdin,NULL);
	
	fechaActual = time(NULL);
	fecha = *localtime(&fechaActual);
	
	//Se guarda la fecha de registro.
	usuario.fRe.dia = fecha.tm_mday;
	usuario.fRe.mes = fecha.tm_mon + 1;
	usuario.fRe.anio = fecha.tm_year;
	usuario.fRe.anio += 1900; //Se le suma 1900 porque "fecha.tm_year" empieza a contar desde 1900.
	
	//Se guarda la fecha en que se creo como último movimiento.
	usuario.fMo.dia = fecha.tm_mday;
	usuario.fMo.mes = fecha.tm_mon + 1;
	usuario.fMo.anio = fecha.tm_year;
	usuario.fMo.anio += 1900;
		
	fwrite(&usuario, sizeof(clienteT), 1, f); //Se escribe en el fichero los datos del usuario.

	printf("\n\n\t\t\t\t\tSe ha guardado el cliente con %cxito.",130);
	
	fclose(f);
	system("pause>nul");
}

void mostrarClientes()
{
	FILE *f;
	clienteT usuario;
	short int i;
	unsigned long int registros;
	
	system("color E0");
	
	f = fopen("clientes.dat","rb"); //Se abre el archivo en modo lectura.
	
	if(!f)
	{
		printf("\n\n\n\n\t\t\t\t\a   No se ha podido abrir el archivo \"clientes.dat\"");
		printf("\n\n\t\t\t1. Es posible que a%cn NO se haya registrado ninguna cuenta de banco.",163);
		printf("\n\n\t\t2. En caso de haberla creado, verifique tener el archivo en la misma carpeta del programa.");
		system("pause>nul");
		return; //Se sale de la función para evitar hacer operaciones con el archivo YA que no se abrió.
	}
	
	fseek(f, 0, SEEK_END); //Se va hasta el final del archivo.
	registros = ftell(f) / sizeof(clienteT); //Ftell regresa la cantidad de bytes leidos se divide entre
	//el tamaño de "clientesT" para saber cuántos registros hay.
	rewind(f); //Regresar el puntero del archivo al inicio.
	
	for(i=0; i<registros; i++)
	{
		printf("\n\t\t\t\t\t\t   Cliente %hd",i+1);
		fread(&usuario, sizeof(clienteT), 1, f); //Lee un registro.
		printf("\n\n\t\t\t\t\tN%cmero de cuenta....... %u",163,usuario.cuenta);
		printf("\n\t\t\t\t\tNombre................. %s",usuario.nombre);		
		printf("\t\t\t\t\tApellidos.............. %s",usuario.apellido);
		printf("\t\t\t\t\tFecha de registro...... %hd / %hd / %hd",usuario.fRe.dia,usuario.fRe.mes,usuario.fRe.anio);		
		printf("\n\t\t\t\t\tN%cmero de tel%cfono..... %s",163,130,usuario.telefono);
		printf("\t\t\t\t\tCorreo................. %s",usuario.correo);
		printf("\t\t\t\t\tFondos................. $ %G",usuario.fondos);
		printf("\n\t\t\t\t\tNIP.................... %hd",usuario.nip);		
		printf("\n\t\t\t\t\t%cltimo movimiento...... %hd / %hd / %hd\n\n",233,usuario.fMo.dia,usuario.fRe.mes,usuario.fRe.anio);
	}
	
	fclose(f);
	system("pause>nul");
}

void buscarCliente()
{
	FILE *f;
	clienteT usuario;
	short int i, modoBusqueda = 1; //Se inicializa en 1 (true) para entrar en el while por primera vez.
	unsigned int cuenta;
	unsigned long int posicion;
	char cadenaAux[T+10];
	
	system("color 5F");
	
	f = fopen("clientes.dat","rb");	//Se abre archivo en modo lectura.

	if(!f) //Verifica que se abra correctamente.
	{
		printf("\n\n\n\n\t\t\t\t\a   No se ha podido abrir el archivo \"clientes.dat\"");
		printf("\n\n\t\t\t1. Es posible que a%cn NO se haya registrado ninguna cuenta de banco.",163);
		printf("\n\n\t\t2. En caso de haberla creado, verifique tener el archivo en la misma carpeta del programa.");
		system("pause>nul");
		return; //Salir del módulo para evitar que se hagan operaciones con el archivo (no se abrió).
	}
		
	while(modoBusqueda) //Bucle para que no se salga del menú hasta que usuario elija.
	{
		posicion = 0; //Se inicializa a 0 (falso) porque se asume que NO se encontrará el dato a buscar.
		
		do
		{	
			setbuf(stdin,NULL);
			system("cls");
			
			printf("\n\t\t\t\t\tM O D O    D E    B %c S Q U E D A",233);
			printf("\n\t\t"); //Salto de línea y tabulador para mostrar la línea más centrada.
			
			for(i=0; i<80; i++) //Ciclo para ir mostrando una línea de separación.
				printf("%c",220);
			
			printf("\n\n\t\t\t\t\t  1) N%cmero de cuenta.",163);
			printf("\n\n\t\t\t\t\t  2) N%cmero telef%cnico.",163,162);
			printf("\n\n\t\t\t\t\t  3) Nombre del cliente.");
			printf("\n\n\t\t\t\t\t  4) Correo electr%cnico.",162);
			printf("\n\n\t\t\t\t\t  0) Salir.");
			
			printf("\n\n\n\t\t\t\t\tIngresa el modo de b%csqueda: ",163);
			scanf("%hd",&modoBusqueda);
			
		}while(modoBusqueda < 0 || modoBusqueda > 4);
	
		setbuf(stdin,NULL); //Limpiar el buffer.
		
		if(!modoBusqueda)	
		{	
			fclose(f);
			return; //Si usuario presiona 0 (falso) la función termina.
		}
		
		else if(modoBusqueda == 1) //Si usuario escogió modo de búsqueda 1, le pregunta el número de cuenta.
		{
			printf("\n\n\t\t\t\t\tIngresa el n%cmero de cuenta: ",163);
			scanf("%u",&cuenta);
		}
		
		else if(modoBusqueda == 2) //Su número de teléfono.
		{
			printf("\n\n\t\t\t\t\tIngresa el tel%cfono del cliente: ",130);
			fgets(cadenaAux, T, stdin);
		}
	
		else if(modoBusqueda == 3)
		{
			printf("\n\n\t\t\t\t\tIngresa el nombre del cliente: ");
			fgets(cadenaAux, T, stdin);

		}
		
		else if(modoBusqueda == 4)
		{
			printf("\n\n\t\t\t\t\tIngresa el correo del cliente: ");
			fgets(cadenaAux, T+10, stdin);
		}	
		
		rewind(f); //El puntero regresa al inicio del archivo.
		
		while(!feof(f) && !posicion) //Termina hasta llegar al fin del archivo, o hasta encontrar el dato.
		{
			
			fread(&usuario, sizeof(clienteT), 1, f); //Lee registro.
			
			if(modoBusqueda == 1)
			{
				if(usuario.cuenta == cuenta)
					posicion = ftell(f); //Se guarda la posición en la que haya terminado de leer.
			}
					
			else if(modoBusqueda == 2)
			{
				if( !strcmp(usuario.telefono,cadenaAux) ) //Si son iguales, regresa 0, y con "!" será true.
					posicion = ftell(f);
			}

			else if(modoBusqueda == 3)
			{
				if( !strcmp(usuario.nombre,cadenaAux) )
					posicion = ftell(f);
			}
					
			else if(modoBusqueda == 4)
			{
				if( !strcmp(usuario.correo,cadenaAux) )
					posicion = ftell(f);
			}
					
		}
			
		if(!posicion) //Siposición es igual a 0, entonces NO se encontró el dato.
			printf("\n\n\t\t\t\t\tNo se ha encontrado ning%cn cliente.",163);
			
		else
		{
			system("cls");
			fseek(f, posicion-sizeof(clienteT), SEEK_SET); //Se regresa puntero del archivo al inicio
			//y se desplaza hasta la diferencia en bytes del segundo parámetro.
			
			printf("\n\n\t\t\t\t\tN%cmero de cuenta....... %u",163,usuario.cuenta);
			printf("\n\t\t\t\t\tNombre................. %s",usuario.nombre);		
			printf("\t\t\t\t\tApellidos.............. %s",usuario.apellido);
			printf("\t\t\t\t\tFecha de registro...... %hd / %hd / %hd",usuario.fRe.dia,usuario.fRe.mes,usuario.fRe.anio);		
			printf("\n\t\t\t\t\tN%cmero de tel%cfono..... %s",163,130,usuario.telefono);
			printf("\t\t\t\t\tCorreo................. %s",usuario.correo);
			printf("\t\t\t\t\tFondos................. $ %G",usuario.fondos);
			printf("\n\t\t\t\t\tNIP.................... %hd",usuario.nip);		
			printf("\n\t\t\t\t\t%cltimo movimiento...... %hd / %hd / %hd",233,usuario.fMo.dia,usuario.fRe.mes,usuario.fRe.anio);	
		}
		
		system("pause>nul");
	}
	
	fclose(f); //Cierra el archivo.
}

void ordenarClientesName()
{
	FILE *f;
	clienteT usuario, *usuarioAux = NULL;
	char nombre[T], aux[T];
	short int i, j; 
	unsigned long int registros;
	
	system("color 71");
	
	f = fopen("clientes.dat","rb"); //Abrir el archivo en modo lectura.
			
	if(!f)
	{
		printf("\n\n\n\n\t\t\t\t\a   No se ha podido abrir el archivo \"clientes.dat\"");
		printf("\n\n\t\t\t1. Es posible que a%cn NO se haya registrado ninguna cuenta de banco.",163);
		printf("\n\n\t\t2. En caso de haberla creado, verifique tener el archivo en la misma carpeta del programa.");
		system("pause>nul");
		return; //Salir del módulo para evitar que se hagan operaciones con el archivo (no se abrió).
	}
	
	fseek(f, 0, SEEK_END); //El puntero del archivo se va hasta el final.
	registros = ftell(f) / sizeof(clienteT); // guardar la cantidad de registros en el fichero.
	rewind(f); //Regresar el puntero del archivo al inicio.
	
	usuarioAux = (clienteT*) malloc( registros * sizeof(clienteT) ); //Se reserva memoria del heap.

	if(!usuarioAux) //Verifica que se haya podido reservar el espacio pedido.
	{
		printf("\n\t\t\tNo se ha podido reservar suficiente memoria.");
		system("pause>nul");
		exit(-1);
	}
	
	for(i=0; i<registros; i++) //Guarda los registros en la estructura dinámica.
		fread(usuarioAux, sizeof(clienteT), registros, f);

	rewind(f); //Regresar al principio del archivo.
	
	for(i=0; i<registros; i++) //Bucle para ordenar los nombres alfabéticamente.
		for(j=i; j<registros; j++) //Bucle para que el registro número "i" se vaya comparando con los demás.
			if( strcmp( usuarioAux[i].nombre, usuarioAux[j].nombre) > 0 ) //Verifica el orden alfabético.
			{//Si entra en el condicional, entonces el elemento "i" es MAYOR alfabéticamente.
				strcpy(aux, usuarioAux[i].nombre); //Se copia el elemento número "i" en aux.
				strcpy(usuarioAux[i].nombre, usuarioAux[j].nombre);//Se copia el elemento número "j" a "i".
				strcpy(usuarioAux[j].nombre, aux);//Ahora en "aux" se copia el elemento "i" en número "J".
			}
	
	for(i=0; i<registros; i++)
	{
		rewind(f); //Regresar el puntero del archivo al inicio para volver a leer el fichero completo.
		for(j=0; j<registros; j++) 
		{
			fread(&usuario, sizeof(clienteT), 1, f);//Va leyendo cada registro del archivo.
			if( !strcmp(usuario.nombre, usuarioAux[i].nombre) ) //Verifica cual es el nombre que coincide.
			{ 
				printf("\n\t\t\t\t\tN%cmero de cuenta....... %u",163,usuario.cuenta);
				printf("\n\t\t\t\t\tNombre................. %s",usuario.nombre);		
				printf("\t\t\t\t\tApellidos.............. %s",usuario.apellido);
				printf("\t\t\t\t\tFecha de registro...... %hd / %hd / %hd",usuario.fRe.dia,usuario.fRe.mes,usuario.fRe.anio);		
				printf("\n\t\t\t\t\tN%cmero de tel%cfono..... %s",163,130,usuario.telefono);
				printf("\t\t\t\t\tCorreo................. %s",usuario.correo);
				printf("\t\t\t\t\tFondos................. $ %G",usuario.fondos);
				printf("\n\t\t\t\t\tNIP.................... %hd",usuario.nip);		
				printf("\n\t\t\t\t\t%cltimo movimiento...... %hd / %hd / %hd\n\n",233,usuario.fMo.dia,usuario.fRe.mes,usuario.fRe.anio);			
			}			
		}
	}
		
	free(usuarioAux); //Liberar la memoria que se reservó.	
	fclose(f);//Cerrar el fichero.
	system("pause>nul");	
}

void editarDatos()
{
	FILE *f;
	clienteT usuario;
	short int encontrado = 0;
	unsigned int cuenta;
	unsigned long int posicion;
	
	system("color 87");
	f = fopen("clientes.dat","r+b"); //se abre en modo lectura y escritura.
	
	if(!f)
	{
		printf("\n\n\n\n\t\t\t\t\a   No se ha podido abrir el archivo \"clientes.dat\"");
		printf("\n\n\t\t\t1. Es posible que a%cn NO se haya registrado ninguna cuenta de banco.",163);
		printf("\n\n\t\t2. En caso de haberla creado, verifique tener el archivo en la misma carpeta del programa.");
		system("pause>nul");
		return; //Salir del módulo para evitar que se hagan operaciones con el archivo (no se abrió).
	}
	
	printf("\n\n\t\t\t\tIngrese el n%cmero de cuenta: ",163);
	scanf("%u",&cuenta);
	setbuf(stdin,NULL);
	
	rewind(f);
	while(!feof(f) && !encontrado ) //Termina hasta leer todo el archivo, o hasta encontrar el num. cuenta.
	{
		fread(&usuario, sizeof(clienteT), 1, f);
		if(usuario.cuenta == cuenta) //Verifica si el número de cuenta, es el mismo del registro.
			encontrado = 1; //Si coincide, entonces "encontrado" toma valor de 1 (verdadero).
	}
	
	if(!encontrado) //Si encontrado vale 0 (false), entonces ese número de cuenta NO existe.
	{
		printf("\n\n\t\t\tEse n%cmero de cuenta no se encuentra asociado a ning%cn cliente.",163,163);
		fclose(f); //Se cierra el archivo.
		system("pause>nul");
		return; 
	}
	
	posicion = ftell(f); //Se guarda la posición en donde encontró el registro del número de cuenta.

	while(encontrado) //Se usa la variable "encontrado" como auxliar.
	{
		system("cls");
		
		fseek(f, posicion-sizeof(clienteT), SEEK_SET); //Se regresa el puntero a la posición 
		fread(&usuario, sizeof(clienteT), 1, f); //Guarda el nuevo dato, el puntero del archivo se desplaza.
	
		printf("\n\n\t\t\t\t\tN%cmero de cuenta....... %u",163,usuario.cuenta);
		printf("\n\t\t\t\t\tNombre................. %s",usuario.nombre);		
		printf("\t\t\t\t\tApellidos.............. %s",usuario.apellido);
		printf("\t\t\t\t\tFecha de registro...... %hd / %hd / %hd",usuario.fRe.dia,usuario.fRe.mes,usuario.fRe.anio);		
		printf("\n\t\t\t\t\tN%cmero de tel%cfono..... %s",163,130,usuario.telefono);
		printf("\t\t\t\t\tCorreo................. %s",usuario.correo);
		printf("\t\t\t\t\tFondos................. $ %G",usuario.fondos);
		printf("\n\t\t\t\t\tNIP.................... %hd",usuario.nip);		
		printf("\n\t\t\t\t\t%cltimo movimiento...... %hd / %hd / %hd",233,usuario.fMo.dia,usuario.fRe.mes,usuario.fRe.anio);	
		
		printf("\n\n\n\t\t\t\t\t E D I T A R    D A T O S\n\t\t");
		for(encontrado=0; encontrado<80; encontrado++)//se usa "encontrado" como auxiliar para el for.
			printf("%c",220); //Imprime línea divisora.
			
		printf("\n\n\t\t\t\t\t\t1. Nombre.");
		printf("\n\t\t\t\t\t\t2. Apellidos.");
		printf("\n\t\t\t\t\t\t3. N%cmero de tel%cfono.",163,130);
		printf("\n\t\t\t\t\t\t4. Correo electr%cnico.",162);
		printf("\n\t\t\t\t\t\t5. Fondos.");
		printf("\n\t\t\t\t\t\t6. NIP.");
		printf("\n\t\t\t\t\t\t7. %cltimo movimiento.",233);
		printf("\n\t\t\t\t\t\t0. Salir.");
		
		printf("\n\n\n\t\t\t\t\tIngresa el n%cmero de dato a editar: ",163);
		scanf("%hd",&encontrado); //Se usa "encontrado" como variable auxiliar para almacenar la opción.
	
		setbuf(stdin,NULL);
		if(encontrado>0 && encontrado<7)
			printf("\n\t\t\t\tIngresa el nuevo dato: ");
			
		switch(encontrado)
		{
			case 1: fgets(usuario.nombre, T, stdin); 		break;
			
			case 2: fgets(usuario.apellido, T, stdin);		break;
			
			case 3: fgets(usuario.telefono, T-10, stdin);	break;
			
			case 4: fgets(usuario.correo, T+10, stdin);		break;
			
			case 5: scanf("%lf",&usuario.fondos);			break;
			
			case 6:	scanf("%4hd",&usuario.nip);	setbuf(stdin,NULL);	break;
			
			case 7: printf("\n\n\t\tIngrese la nueva fecha en formato d%ca/mes/a%co ejemplo (5/4/2020): ",161,164);
					scanf("%hd/%hd/%hd",&usuario.fMo.dia,&usuario.fMo.mes,&usuario.fMo.anio); 
		}
		
		if(encontrado>0 && encontrado<8) //Entra al condicional sólo si presiono el rango de esos números.
		{
			fseek(f, posicion-sizeof(clienteT), SEEK_SET); //Se regresa el puntero a la posición anterior.
			fwrite(&usuario, sizeof(clienteT), 1, f); //Guarda el nuevo dato, el puntero del archivo se desplaza.
		}
	}
				
	fclose(f); //Se cierra el fichero.
}

void eliminarDatos()
{
	FILE *f, *fAux;
	clienteT usuario, usuarioAux;
	short int encontrado = 0;
	unsigned int cuenta;
	unsigned long int registros, posicion;
	
	system("color 46");
	
	f = fopen("clientes.dat", "rb"); //Se abre en modo lectura
	
	if(!f)
	{
		printf("\n\n\n\n\t\t\t\t\a   No se ha podido abrir el archivo \"clientes.dat\"");
		printf("\n\n\t\t\t1. Es posible que a%cn NO se haya registrado ninguna cuenta de banco.",163);
		printf("\n\n\t\t2. En caso de haberla creado, verifique tener el archivo en la misma carpeta del programa.");
		system("pause>nul");
		return; //Salir del módulo para evitar que se hagan operaciones con el archivo (no se abrió).
	}
	
	printf("\n\n\t\t\t\t     Ingrese el n%cmero de cuenta: ",163);
	scanf("%u",&cuenta);
	setbuf(stdin,NULL);
	
	rewind(f); //Regresar el puntero del archivo al inicio.
	while(!feof(f) && !encontrado ) //Termina hasta leer todo el archivo, o hasta encontrar el num. cuenta.
	{
		fread(&usuario, sizeof(clienteT), 1, f);
		if(usuario.cuenta == cuenta) //Verifica si el número de cuenta, es el mismo del registro.
			encontrado = 1; //Si coincide, entonces "encontrado" toma valor de 1 (verdadero).
	}
	
	if(!encontrado) //Si encontrado vale 0 (false), entonces ese número de cuenta NO existe.
	{
		printf("\n\n\t\t\tEse n%cmero de cuenta no se encuentra asociado a ning%cn cliente.",163,163);
		fclose(f); //Se cierra el archivo.
		system("pause>nul");
		return; 
	}
	
	printf("\n\n\t\t\t\t\tN%cmero de cuenta....... %u",163,usuario.cuenta);
	printf("\n\t\t\t\t\tNombre................. %s",usuario.nombre);		
	printf("\t\t\t\t\tApellidos.............. %s",usuario.apellido);
	printf("\t\t\t\t\tFecha de registro...... %hd / %hd / %hd",usuario.fRe.dia,usuario.fRe.mes,usuario.fRe.anio);		
	printf("\n\t\t\t\t\tN%cmero de tel%cfono..... %s",163,130,usuario.telefono);
	printf("\t\t\t\t\tCorreo................. %s",usuario.correo);
	printf("\t\t\t\t\tFondos................. $ %G",usuario.fondos);
	printf("\n\t\t\t\t\tNIP.................... %hd",usuario.nip);		
	printf("\n\t\t\t\t\t%cltimo movimiento...... %hd / %hd / %hd",233,usuario.fMo.dia,usuario.fRe.mes,usuario.fRe.anio);	
	
	printf("\n\n\n\t\t\t\t%cEst%cs seguro que deseas eliminar esta cuenta?",168,160);
	printf("\n\n\t\t\t\t\t\t   1. S%c",161);
	printf("\n\n\t\t\t\t\t\t   2. No");
	
	printf("\n\n\n\t\t\t\t     Ingresa la opci%cn correspondiente: ",162);
	scanf("%hd",&encontrado); //Se usa variable "encontrado" como auxiliar.
	
	if(encontrado != 1)
	{
		fclose(f); //Se cierra el archivo.
		return; 		
	}

	fAux = fopen("clientesAux.dat","wb"); //Abre el archivo en modo escritura (si no existe, lo crea).
	
	if(!fAux)
	{
		printf("\n\n\n\n\t\t\t\t\a   No se ha podido crear el archivo \"aux.dat\"");
		fclose(f); //Cerrar el archivo "clientes.dat"
		system("pause>nul");
		return; //Salir del módulo para evitar que se hagan operaciones con el archivo (no se abrió).
	}
	
	rewind(f); //Regresar el puntero del archivo al inicio.
	fread(&usuarioAux, sizeof(clienteT), 1, f); //Lee el primer registros.
	while( !feof(f) ) //Termina hasta llegara al final del archivo.
	{
		if(usuarioAux.cuenta != cuenta) //Verifica que la cuenta NO coincida con el registro leido.
			fwrite(&usuarioAux, sizeof(clienteT), 1, fAux); //Si NO coincide, se copia al fichero auxiliar.
		
		fread(&usuarioAux, sizeof(clienteT), 1, f); //Lee los registros.
	}
	
	printf("\n\n\n\t\t\t\t\t    La cuenta ha sido eliminada.");

	fclose(f); //Cerrar ficheros.
	fclose(fAux);
	remove("clientes.dat"); //Eliminar el ficher "clientes.dat" (contiene TODA la información).
	rename("clientesAux.dat","clientes.dat"); //Renombrar al nuevo fichero. (toda la info. menos la eliminada).
	system("pause>nul");
}

unsigned int iniciarSesion()
{
	FILE *f;
	clienteT usuario;
	short int nip, encontrado = 0;
	unsigned int cuenta;

	f = fopen("clientes.dat","rb"); //Se abre archivo en modo lectura.
	
	if(!f) //Verifica que se haya abierto correctamente.
	{
		printf("\n\n\t\t\t\t\t\aNo se ha creado ninguna cuenta.");
		system("pause>nul");
		return 0; //Regresa 0 (falso), NO entrará en el condicional y NO iniciará sesión.
	}
	
	system("cls");
	printf("\n\n\t\t\t\t\tIngrese el n%cmero de cuenta: ",163);
	scanf("%u",&cuenta);
	
	printf("\n\n\t\t\t\t\tIngrese NIP: ");
	scanf("%hd",&nip);
	
	while(!feof(f) && !encontrado) //Bucle para buscar la cuenta y el nip.
	{
		fread(&usuario, sizeof(clienteT), 1, f);
		if( usuario.cuenta == cuenta && usuario.nip == nip)
			encontrado = 1;
	}

	if(!encontrado) //Si encontrado sigue valiendo 0 (falso) entonces NO encunetro la cuenta.
	{
		printf("\n\n\t\t\t\t\tLos datos ingresados son incorrectos.");
		fclose(f);
		system("pause>nul");
		return 0; //Regresa 0 (falso)
	}
	
	fclose(f); //Se cierra el archivo.
	return usuario.cuenta; //Regresa el número de cuenta.
}

void cliente(unsigned int cuenta)
{
	short int i, opcion = 1;
	
	while(opcion)
	{
		system("cls");
		system("color B0");
		setbuf(stdin,NULL);
		
		printf("\n\t\t\t\t\t     C L I E N T E");
		printf("\n\t\t"); //Salto de línea y tabulador para mostrar la línea más centrada.
		
		for(i=0; i<80; i++) //Ciclo para ir mostrando una línea de separación.
			printf("%c",220);
			
		printf("\n\n\t\t\t\t\t[1] Consular informaci%cn.",162);
		printf("\n\n\t\t\t\t\t[2] Realizar dep%csito.",162);
		printf("\n\n\t\t\t\t\t[3] Realizar retiro.");
		printf("\n\n\t\t\t\t\t[4] Realizar transferencia.");
		printf("\n\n\t\t\t\t\t[0] Salir.");
		
		printf("\n\n\n\t\t\t\t\tIngrese opci%cn: ",162);
		scanf("%hd",&opcion);

		system("cls");
		switch(opcion)
		{
			case 1:	mostrarInformacion(cuenta);	break; 
			
			case 2: realizarDeposito(cuenta);	break;
			
			case 3: realizarRetiro(cuenta);		break;
			
			case 4: transferir(cuenta);			break;
		}
	}
	
	system("color 07");
}

void mostrarInformacion(unsigned int cuenta)
{
	FILE *f;
	clienteT usuario;
	short int encontrado = 0;
	
	f = fopen("clientes.dat","rb"); //Se abre en modo lectura.
	
	if(!f)
	{
		printf("\n\n\n\n\t\t\t\t\a   No se ha podido abrir el archivo \"clientes.dat\"");
		fclose(f);
		system("pause>nul");
		exit(-1); //Salir del módulo para evitar que se hagan operaciones con el archivo (no se abrió).
	}
	
	while(!feof(f) && !encontrado)
	{
		fread(&usuario, sizeof(clienteT), 1, f);
		if( usuario.cuenta == cuenta ) //Busca el registro asoaciado a la cuenta.
			encontrado = 1;	
	}
	
	printf("\n\n\t\t\t\t\tN%cmero de cuenta....... %u",163,usuario.cuenta);
	printf("\n\t\t\t\t\tNombre................. %s",usuario.nombre);		
	printf("\t\t\t\t\tApellidos.............. %s",usuario.apellido);
	printf("\t\t\t\t\tFecha de registro...... %hd / %hd / %hd",usuario.fRe.dia,usuario.fRe.mes,usuario.fRe.anio);		
	printf("\n\t\t\t\t\tN%cmero de tel%cfono..... %s",163,130,usuario.telefono);
	printf("\t\t\t\t\tCorreo................. %s",usuario.correo);
	printf("\t\t\t\t\tFondos................. $ %G",usuario.fondos);
	printf("\n\t\t\t\t\tNIP.................... %hd",usuario.nip);		
	printf("\n\t\t\t\t\t%cltimo movimiento...... %hd / %hd / %hd\n\n",233,usuario.fMo.dia,usuario.fRe.mes,usuario.fRe.anio);

	fclose(f);
	system("pause>nul");
}

void realizarDeposito(unsigned int cuenta)
{
	FILE *f;
	clienteT usuario;
	short int encontrado = 0;
	double deposito;
	unsigned long int posicion;
	
	f = fopen("clientes.dat", "rb+"); //Se abre en modo lectura y escritura.
	
	if(!f)
	{
		printf("\n\n\n\n\t\t\t\t\a   No se ha podido abrir el archivo \"clientes.dat\"");
		fclose(f);
		system("pause>nul");
		exit(-1); //Salir del módulo para evitar que se hagan operaciones con el archivo (no se abrió).
	}	
	
	printf("\n\n\t\t\t\t\tIngrese el monto dep%csito: ",162);
	scanf("%lf",&deposito);
	
	if(deposito <= 0) //Condicional para verificar que la cantidad a depositar sea mayor a 0.
	{
		printf("\n\n\t\t\t\tDebes de ingresar una cantidad mayor a 0.");
		fclose(f); //Se cierra el archivo.
		system("pause>nul");
		return; 
	}
	
	while(!feof(f) && !encontrado) //Bucle para llegar al registro de la cuenta selecionada.
	{
		fread(&usuario, sizeof(clienteT), 1, f);
		if( usuario.cuenta == cuenta)
		{	
			encontrado = 1;
			posicion = ftell(f); //Se guarda la posición.
		}
	}

	usuario.fondos += deposito; //Se suma lo depositado a la cuenta.
	fseek(f, posicion-sizeof(clienteT), SEEK_SET); //Se desplaza hacia el registro de la cuenta.
	fwrite(&usuario, sizeof(clienteT), 1, f); //Se guarda el el registro.
	
	printf("\n\n\t\t\t\t\tSe ha hecho el dep%csito a tu cuenta.",162);	
		
	fclose(f);
	system("pause>nul");
}

void realizarRetiro(unsigned int cuenta)
{
	FILE *f;
	clienteT usuario;
	short int encontrado = 0;
	double retiro;
	unsigned long int posicion;
	
	f = fopen("clientes.dat","rb+");
	
	if(!f)
	{
		printf("\n\n\n\n\t\t\t\t\a   No se ha podido abrir el archivo \"clientes.dat\"");
		fclose(f);
		system("pause>nul");
		exit(-1); //Salir del módulo para evitar que se hagan operaciones con el archivo (no se abrió).
	}

	printf("\n\n\t\t\t\t\tIngresa el monto del retiro: ");
	scanf("%lf",&retiro);
	
	if(retiro <= 0) //Condicional para verificar que la cantidad a retirar sea mayor a 0.
	{
		printf("\n\n\t\t\t\t\tDebes de ingresar una cantidad mayor a 0.");
		fclose(f); //Se cierra el archivo.
		system("pause>nul");
		return; 
	}

	while(!feof(f) && !encontrado) //Bucle para llegar al registro de la cuenta selecionada.
	{
		fread(&usuario, sizeof(clienteT), 1, f);
		if( usuario.cuenta == cuenta)
		{	
			encontrado = 1;
			posicion = ftell(f); //Se guarda la posición.
		}
	}

	if( usuario.fondos < retiro )
	{
		printf("\n\n\t\t\t\tNo dispones con los fondos suficientes para retirar.",162);
		fclose(f);
		system("pause>nul");
		return;
	}
	
	usuario.fondos -= retiro;
	fseek(f, posicion-sizeof(clienteT), SEEK_SET); //Se desplaza hacia el registro de la cuenta.
	fwrite(&usuario, sizeof(clienteT), 1, f); //Se guarda el el registro.
	
	printf("\n\n\t\t\t\t\tSe ha hecho el retiro de tu cuenta.",162);
	
	fclose(f);
	system("pause>nul");
	
}

void transferir(unsigned int cuenta)
{
	FILE *f;
	clienteT usuario;
	short int i, encontrado = 0, salir, intentar;
	unsigned int cuentaTrans;
	unsigned long int posicion, posicionAux;
	double monto;
	char nombre[T];
	
	f = fopen("clientes.dat","rb+");
	
	if(!f)
	{
		printf("\n\n\n\n\t\t\t\t\a   No se ha podido abrir el archivo \"clientes.dat\"");
		fclose(f); //Cerrar el archivo
		system("pause>nul");
		exit(-1); //Salir del módulo para evitar que se hagan operaciones con el archivo (no se abrió).	
	}
	
	printf("\n\n\t\t\t\t\tIngrese el monto a transferir: ");
	scanf("%lf",&monto);
	
	if(monto <= 0) //Condicional para verificar que la cantidad a retirar sea mayor a 0.
	{
		printf("\n\n\t\t\t\t\tDebes de ingresar una cantidad mayor a 0.");
		fclose(f); //Se cierra el archivo.
		system("pause>nul");
		return; 
	}

	while(!feof(f) && !encontrado) //Bucle para llegar al registro de la cuenta selecionada.
	{
		fread(&usuario, sizeof(clienteT), 1, f);
		if( usuario.cuenta == cuenta)
		{	
			encontrado = 1;
			posicion = ftell(f); //Se guarda la posición.
		}
	}

	if( usuario.fondos < monto )
	{
		printf("\n\n\t\t\t\tNo dispones con los fondos suficientes para retirar.",162);
		fclose(f);
		system("pause>nul");
		return;
	}
	
	do
	{
		setbuf(stdin,NULL);
		system("cls");
		
		printf("\n\n\t\t\t\tIngrese el n%cmero de cuenta a transferir: ",163);
		scanf("%u",&cuentaTrans);
		
		encontrado = 0; //0 NO se encontro, 1 SÍ se encontró, 2 es la misma cuenta del enviador.
		salir = 1; //1 se repite, 0 sale.
		intentar = 1; 
		rewind(f); //Regresa el puntero del archivo al incio.
			
		while(!feof(f) && !encontrado) //Bucle para buscar la cuenta a transferir.
		{
			fread(&usuario, sizeof(clienteT), 1, f);
			if(usuario.cuenta == cuentaTrans)
			{	
				if(cuenta == cuentaTrans)
					encontrado = 2;		
								
				else 
				{
					posicionAux = ftell(f);
					encontrado = 1;
					salir = 0;
				}
			}
		}
		
		if(!encontrado) //Si no se encuentra la cuenta, entra en el condicional.
		{
			printf("\n\n\t\t\t\t     No se ha encontrado ese n%cmero de cuenta.",163);
			printf("\n\n\n\t\t\t\t\t\t1. Intentar de nuevo.");
			printf("\n\n\t\t\t\t\t\t0. Salir.");
			
			printf("\n\n\n\n\t\t\t\t\t\tIngrese opci%cn: ",162);
			scanf("%hd",&intentar);
			
			if(!intentar)
				salir = 0;
		}
		
		else if(encontrado == 2)
		{
			printf("\n\n\t\t\t\tNo puedes hacer la transferencia a tu propia cuenta.");
			printf("\n\n\n\t\t\t\t\t\t1. Intentar de nuevo.");
			printf("\n\n\t\t\t\t\t\t0. Salir.");
			
			printf("\n\n\n\n\t\t\t\t\t\tIngrese opci%cn: ",162);
			scanf("%hd",&intentar);
			
			if(!intentar)
				salir = 0;
		}
		
	}while(salir);
	
	if(!intentar) //Si usuario dio en salir (0), entonces entra en el condicional.
	{
		fclose(f); //Se cierra el archivo.
		return; 
	}
	
	for(i=0; usuario.nombre[i] != '\n'; i++) //Ciclo para quitarle el '\n' de "usuario.nombre".
		nombre[i] = usuario.nombre[i];
		
	nombre[i] = '\0'; //Al final pasarle el carácter nulo.
	
	printf("\n\n\t\t\t\t\tN%cmero de cuenta....... %u",163,usuario.cuenta);
	printf("\n\t\t\t\t\tNombre................. %s %s",nombre,usuario.apellido);		
	printf("\t\t\t\t\tN%cmero de tel%cfono..... %s",163,130,usuario.telefono);
	printf("\t\t\t\t\tCorreo................. %s",usuario.correo);
	printf("\t\t\t\t\tMonto a transferir..... %G",monto);
	
	printf("\n\n\n\t\t\t\t\t\t0. Cancelar.");
	printf("\n\n\t\t\t\t\t\t1. Transferir.");
	
	printf("\n\n\n\t\t\t\t\tIngrese la opci%cn correspondiente: ",162);
	scanf("%hd",&intentar); //Se toma variable "intentar" como auxiliar.
	
	if(!intentar)
	{
		fclose(f); //Se cierra el archivo.
		return; 
	}

	fseek(f, posicion-sizeof(clienteT), SEEK_SET); //Puntero del archivo se desplaza al usuario que
	fread(&usuario, sizeof(clienteT), 1, f); //realizará la transferencia.
	
	usuario.fondos -= monto; //Se le resta el dinero transferido.
	
	fseek(f, posicion-sizeof(clienteT), SEEK_SET); //Se vuelve a desplazar el puntero del archivo a la
	fwrite(&usuario, sizeof(clienteT), 1, f); //posición del usuario y se actualiza el registro.
	
	
	fseek(f, posicionAux-sizeof(clienteT), SEEK_SET); //Puntero del archivo se desplaza al usuario 
	fread(&usuario, sizeof(clienteT), 1, f); //a quien le van a transferir.
	
	usuario.fondos += monto; //Se le suma el dinero transferido.
	
	fseek(f, posicionAux-sizeof(clienteT), SEEK_SET); //Se vuelve a desplazar el puntero del archivo
	fwrite(&usuario, sizeof(clienteT), 1, f); //para actualizar el registro.
	
	printf("\n\n\n\n\t\t\t\t\t    Se ha hecho la trasnferencia.");

	fclose(f);
	system("pause>nul");
}
