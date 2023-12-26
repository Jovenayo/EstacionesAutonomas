#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>
#include <signal.h>

#define PORTE 8080
#define PORTC 8081
#define MAX_BUFFER_SIZE 1024
#define FICHERO_GUARDADO "fichero.txt" //Dieccion del fichero de guardado.

int fd; //Descriptor del fichero.
struct sockaddr_in serverEstaciones_address, clienteEstacion_address, serverClientes_address, clienteCliente_address; //Estructuras para almacenar las direcciones de conexion.
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
socklen_t addr_size;

sem_t *semaforo;

//Señal de interrupcion
void  interrupcion(int signo){
	sem_close(semaforo);
	sem_unlink("/semaforo");
	exit(EXIT_SUCCESS);
}

void sendDatosEstacion(int clienteCliente_socket){
	// Abrimos el archivo en modo lectura
	FILE *f = fopen(FICHERO_GUARDADO, "r");
	char idEstacion[1024];

	recv(clienteCliente_socket, idEstacion, sizeof(idEstacion), 0);
    printf("idEstacion: %s \n", idEstacion);


	// Si el archivo no existe, imprimimos un mensaje de error
	if (f == NULL) {
		printf("El archivo no existe.\n");
        return;
	}

    char *ultimaLinea = NULL;
    char copiaLinea[100] = "";

	// Leemos cada línea del archivo
	while (fgets(copiaLinea, sizeof(copiaLinea), f) != NULL) {
		// Obtenemos la línea del archivo
        copiaLinea[strcspn(copiaLinea, "\n")] = '\0';
        char *copiaAntesStrtok = (char *)malloc(strlen(copiaLinea) + 1);
		strcpy(copiaAntesStrtok, copiaLinea);


		// Obtenemos el primer campo de la línea
		char *campo_1 = strtok(copiaLinea, "#");

		// Si el primer campo es igual al número, enviamos la línea
		if (atoi(campo_1) == atoi(idEstacion)) {
            ultimaLinea = copiaAntesStrtok;
		}
	}

    printf("Ultimalinea: %s \n", ultimaLinea);
    if(ultimaLinea != NULL){
        ssize_t bytes_sent = send(clienteCliente_socket, ultimaLinea, strlen(ultimaLinea), 0);
        if (bytes_sent == -1) {
            // Error en el envío
            perror("Error al enviar datos");
        } else {
            // Éxito en el envío
            printf("Se han enviado %zd bytes\n", bytes_sent);
        }
        //send(clienteCliente_socket, "\n", 1, 0);
        free(ultimaLinea);
    }

	close(clienteCliente_socket);
	// Cerramos el archivo
	fclose(f);

}

void Cliente(int clienteCliente_socket){
	char buffer[1024];
	char opc[1024];
	//Recivir la opcion a realizar
	size_t bytes_recividos = recv(clienteCliente_socket, opc, sizeof(opc), 0);
	int opcion = atoi(opc);
	printf("%d\n", opcion);
	if(bytes_recividos > 0){
		//Crear el case, con las respectivas funciones
		switch (opcion) {
			case 1://: Obtener datos del fichero
				break;
			case 2://:Obtener datos de una estacion
				//Obtenemos Id Estacion del soket
				sendDatosEstacion(clienteCliente_socket);
				break;
			case 3:
				break;
			default:
			printf("Opción no valida: %i\n",opcion);
    	}

    } else {
        printf("Error al recibir datos del socket: %s\n",opc);
    }
	//Enviar resultado de la peticion

    close(clienteCliente_socket);
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
	serverClientes_address.sin_port = htons((int)PORTC);
	serverClientes_address.sin_addr.s_addr = INADDR_ANY;//Direccion del server (INADDR-ANY: Conexiones a cualquier interfaz del sistea.)
	memset(serverClientes_address.sin_zero, '\0', sizeof serverClientes_address.sin_zero);

	//Enlazar soket a la direcicon y puerto.
	if(bind(serverCliente_socket, (struct sockaddr*)&serverClientes_address, sizeof(serverClientes_address)) < 0){
	    perror("Error en el enlace Cliente");
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
			perror("Error al aceptar la conexion"); exit(EXIT_FAILURE);
		}

		if(fork() == 0){ //Proceso hijo Estacion para mantener la conexion----------------------------[HIJO]----------------
			close(serverCliente_socket);
            printf("Cliente conectado \n");
			Cliente(clienteCliente_socket);//Proceso hijo Cliente
			exit(EXIT_SUCCESS);
		} else {
		//Cerrar soket y termianr hijos
		   	close(clienteCliente_socket);
			waitpid(-1, NULL, WNOHANG);
		}
	}
}


void Estacion(int clienteEstacion_socket){ //Zona de riesgo para escribir el fichero
	while(1){
	char buffer[1024];
	printf("antes");
	//Recibir datos de la estacion
	size_t bytes_recividos = recv(clienteEstacion_socket, buffer, sizeof(buffer), 0);
	printf("Datos recividos_________");

	if(bytes_recividos > 0){

		//Abrir el archivo en modo escritura
		FILE *file = fopen(FICHERO_GUARDADO, "a");
		if(file == NULL){
			perror("Error al abrir el archivo");
			exit(EXIT_FAILURE);
		}

		//sem_wait(semaforo);
        	printf("Entro semaforo");
		//Escribir la linea al archivo
       		fprintf(file, "%.*s", (int)bytes_recividos, buffer);
        	//sem_post(semaforo);
		//sem_close(semaforo);
	        printf("Salgo semaforo");

        	//cerrar el ficehro
        	fclose(file);
       		printf("Línea escrita en el archivo: %d\n", (int)bytes_recividos);
    	} else {
       		 printf("Error al recibir datos del socket: %s\n",buffer );
        }
}
    close(clienteEstacion_socket);
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
	serverEstaciones_address.sin_port = htons((int)PORTE);
	serverEstaciones_address.sin_addr.s_addr = INADDR_ANY;//Direccion del server (INADDR-ANY: Conexiones a cualquier interfaz del sistea.)
	memset(serverEstaciones_address.sin_zero, '\0', sizeof serverEstaciones_address.sin_zero);

	//Enlazar soket a la direcicon y puerto.
	if(bind(serverEstacion_socket, (struct sockaddr*)&serverEstaciones_address, sizeof(serverEstaciones_address)) < 0){
		perror("Error en el enlace Estacion:");
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

		if(fork() == 0){//Proceso hijo Estacion para mantener la conexion---------------------------[HIJO]---------------------
            printf("SCK Entra");
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



int main(){
	signal(SIGINT, interrupcion);
	//Proceso Servidor
	//Crear el semaforo para asegurar el acceso al fichero, de forma exclusiva.
	sem_t *semaforo = sem_open("/semaforo", O_CREAT | O_EXCL, 0600, 1);
	if(semaforo == SEM_FAILED){
	perror("Error al crear el semaforo");
	exit(EXIT_FAILURE);
	}

	if(fork() == 0 ){
		//Proceso Gestor de estaciones
			//->Estacion
		GestorEstaciones();

	} else if(fork() == 0 ){
		//Proceso Gestor de clientes
			//->Cliente
		GestorClientes();
	}
	//Proceso servidor
	int status;
	wait(&status);
	sem_close(semaforo);
	sem_unlink("/semaforo");
	exit(0);
}
