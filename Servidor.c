#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <pthread.h>

#define PORT 8080;
#define MAX_BUFFER_SIZE 1024;
#define FICEHRO_GUARDADO "fichero.txt"; //Dieccion del fichero de guardado.
int fd; //Descriptor del fichero.
struct sockaddr_in serverEstaciones_address, clienteEstacion_address, serverClientes_address, clienteCliente_address; //Estructuras para almacenar las direcciones de conexion.
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
socklen_t addr_size;

int main(){
	//Proceso Servidor
	if(fork() == 0 ){
		//Proceso Gestor de estaciones
			//->Estacion
		GestorEstaciones();
			
	} else if(fork() == 0 ){
		//Proceso Gestor de clientes
			//->Cliente
		GestorClienes();
	}
	//Proceso servidor
}


void GestorClientes(){
	
	//Crear Soket
	int serverCliente_socket, clienteCliente_socket;
	serverCliente_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(serverCliente_socket == -1){
	    perror("Erroral crear el serverEstacion_socket");
	    exit(EXIT_FAILURE);
	}
	
	//Configurar direccion del servidor
	serverClientes_address.sin_family = AF_INET;
	serverClientes_address.sin_port = htons(PORT);
	serverClientes_address.sin_addr.s_addr = INADDR_ANY;//Direccion del server (INADDR-ANY: Conexiones a cualquier interfaz del sistea.)
	
	//Enlazar soket a la direcicon y puerto.
	if(bind(serverCliente_socket, (struct soketaddr*)&serverClientes_address, sizeof(serverClientes_address)) < 0){
	    perror("Error en el enlace");
	    exit(EXIT_FAILURE);
	}
	
	//Activar el serverEstaciones_soket con bufer para 10 peticiones
	if(listen(serverCliente_socket, 10) == 0) {
	    printf("Esperando conexiones...\n");
	} else {
	    perror("Error al escuchar");
	    exit(EXIT_FAILURE);
	}
	
	//Bucle infinito para atender peticiones de estaciones.
	while(1){
		addr_size = sizeof(clienteCliente_socket);
		
		//Aceptar la conexion entrante
		clienteCliente_socket = accept(serverCliente_socket, (struct sockaddr*)&clienteEstacion_address,  &addr_size);
		if(clienteCliente_socket < 0){
			perror("Error al aceptar la conexion");
			exit(EXIT_FAILURE);
		}
		
		if(forck() == 0){
			//Proceso hijo Estacion para mantener la conexion-------------------------------------------------
			close(serverCliente_socket);
			Cliente(clienteCliente_socket);//Proceso hijo Cliente
			exit(EXIT_SUCCESS);
		} else {
		//Cerrar soket y termianr hijos
		    close (clienteCliente_socket);
			waitpid(-1, NULL, WNOHANG);
		}
	}
}

void Cliente(int clienteCliente_socket){
	char buffer[1024];
	char opc[1024];
	//Recivir la opcion a realizar
	size_t bytes_recividos = recv(clienteCliente_socket, buffer, sizeof(buffer), 0);
	recv(clienteCliente_socket, opc, sizeof(opt), 0)
	int opcion = atoi(opc)


	if(bytes_recividos > 0){
		//Crear el case, con las respectivas funciones
		switch (opcion) {
			case 1://: Obtener datos del fichero

				break;
			case 2://: Obtener datos de una estacion
				//Obtenemos Id Estacion del soket
				getDatos(clienteCliente_socket);
				break;
			case 3:
			
				break;
			default:
			printf("Opción no valida.\n");
  		}
			
		
    	} else { 
        printf("Error al recibir datos del socket");
        }
	//Enviar resultado de la peticion
	
	
    close(clienteCliente_socket);
}

void GestorEstaciones(){
	
	//Crear Soket
	int serverEstacion_socket, clienteEstacion_socket;
	serverEstacion_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(serverEstacion_socket == -1){
		perror("Erroral crear el serverEstacion_socket");
		exit(EXIT_FAILURE);
	}
	
	//Configurar direccion del servidor
	serverEstaciones_address.sin_family = AF_INET;
	serverEstaciones_address.sin_port = htons(PORT);
	serverEstaciones_address.sin_addr.s_addr = INADDR_ANY;//Direccion del server (INADDR-ANY: Conexiones a cualquier interfaz del sistea.)
	
	//Enlazar soket a la direcicon y puerto.
	if(bind(serverEstacion_socket, (struct soketaddr*)&serverEstaciones_address, sizeof(serverEstaciones_address)) < 0){
		perror("Error en el enlace");
		exit(EXIT_FAILURE);
	}
	
	//Activar el serverEstaciones_soket con bufer para 10 peticiones
	if(listen(serverEstacion_socket, 10) == 0) {
		printf("Esperando conexiones...\n");
	} else {
		perror("Error al escuchar");
		exit(EXIT_FAILURE);
	}
	
	//Bucle infinito para atender peticiones de estaciones.
	while(1){
		addr_size = sizeof(clienteEstacion_socket);
		
		//Aceptar la conexion entrante
		clienteEstacion_socket = accept(serverEstacion_socket, (struct sockaddr*)&clienteEstacion_address,  &addr_size);
		
		if(clienteEstacion_socket < 0){
			perror("Error al aceptar la conexion");
			exit(EXIT_FAILURE);
		}
		
		if(forck() == 0){
			//Proceso hijo Estacion para mantener la conexion-------------------------------------------------
			close(serverEstacion_socket);
			Estacion(clienteEstacion_socket);//Proceso hijo Estacion
			exit(EXIT_SUCCESS);
		} else {
			//Cerrar soket y termianr hijos
			close (clienteEstacion_socket);
			waitpid(-1, NULL, WNOHANG);
		}
	}
}

void Estacion(int clienteEstacion_socket){ //Zona de riesgo para escribir el fichero
	char buffer[1024];

	//Recibir datos de la estacion
	size_t bytes_recividos = recv(clienteEstacion_socket, buffer, sizeof(buffer), 0);
	
	if(bytes_recividos > 0){
	
		//Abrir el archivo en modo escritura
		FILE *file = fopen(ficehroGuardado, "a");
		if(file == NULL){
			perror("Error al abrir el archivo");
			exit(EXIT_FAILURE);
		}
		
		//Escribir la linea al archivo
       		fprintf(file, "%.*s", (int)bytes_recividos, buffer);
        
        	//cerrar el ficehro
        	fclose(file);
       		printf("Línea escrita en el archivo: %.*s\n", (int)bytes_recividos);
    	} else { 
        printf("Error al recibir datos del socket");
        }
    close(clienteEstacion_socket);
}


//Funciones de gestion del fichero
void getDato(int clienteCliente_soket){
	// Abrimos el archivo en modo lectura
	FILE *f = fopen(FICEHRO_GUARDADO, "r");
	char idEstacion[1024];
	
	recv(clienteCliente_socket, idEstacion, sizeof(idEstacion), 0)
	
	// Si el archivo no existe, imprimimos un mensaje de error
	if (f == NULL) {
		printf("El archivo no existe.\n");
	return 1;
	}
	
	// Leemos cada línea del archivo
	while (!feof(f)) {
		// Obtenemos la línea del archivo
		char linea[1024];
		fgets(linea, sizeof(linea), f);
	
		// Obtenemos el primer campo de la línea
		char *campo_1 = strtok(linea, "#");
	
		// Si el primer campo es igual al número, enviamos la línea
		if (atoi(campo_1) == atoi(idEstacion)) {
			send(clienteCliente_socket, linea, sizeof(linea), 0);
		}
	}
	close(clienteCliente_socket)
	// Cerramos el archivo
	fclose(f);
	
	return 0;
	}
}
