#include <studio.h>
#include <stblib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define MAX_BUFFER_SIZE 1024;
const char *ficehroGuardado = "fichero.txt"; //Dieccion del fichero de guardado.
int fd; //Descriptor del fichero.
struct sockaddr_in serverEstaciones_address, clienteEstacion_address, clienteCliente_addres; 
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(){
	//Proceso Servidor
	//Obtener descriptor del fichero de guardado
	fd = open(nombreArchivo, O_RDONLY);



	if(fork() == 0 ){
	//Proceso GestorEstaciones.------------------------------------------------------------------

	    //Crear Soket
	    int serverEstacion_soket, clienteEstacion_soket;
	    serverEstacion_socket = socket(AF_INET, SOCK_STEAM, 0);
	    if(serverEstacion_socket == -1){
		    perror("Erroral crear el server_socket");
		    exit(EXIT_FAILURE);
	    }

	    //Configurar direccion del servidor
	    serverEstacnes_address.sin_family = AF_INET;
	    serverEstaciones_address.sin_port = htons(PORT);
	    serverEstaciones_address.sin_addr.s_addr = INADDR_ANY;

	    //Enlazar soket a la direcicon y puerto.
	    if(bind(serverEstacion_soket, (struct soketaddr*)&serverEstacion_address, sizeof(serverEstacion_address)) < 0){
		    perror("Error en el enlace")
		    exit(EXIT_FAILURE);
	    }

        //Activar el serverEstaciones_soket con bufer para 10 peticiones
	    if(listen(serverEstacion_socket, 10) == 0) {
		    printf("Esperando conexiones...\n")
	    } else {
		    perror("Error al escuchar");
		    exit(EXIT_FAILURE);
	    }

        //Bucle infinito para atender peticiones de estaciones.
	    while(1){
		    addr_size = sizeof(client_address);

		    //Aceptar la conexion entrante
		    clinteEstacion_socket = accept(serveEstacion_soket, (struct sockaddr*)&clienteEstacion_address,  &addr_size);
		    if(clienteEstacion_scket < 0){
			    perror("Error al aceptar la conexion")
			    exit(EXIT_FAILURE);
		    }

		    if(forck() == 0){
            //Proceso hijo Estacion para mantener la conexion-------------------------------------------------
			    close(serverCliente_soket);
			    Estacion(clienteEstacion_soket);
			    exit(EXIT_SOURCCESS);
		    } else {
			//Limpiar
			    close (clienteEstacion_soket);

		    }
	    }
        //Cerrar sockets y esperar a los hijos
	    close(serveEstacion_socket);
	    waitpid(-1, NULL, WNOHANG)
	    return 0;
        //FIN clase GestorEstaciones--------------------------------------------------------------


    }else{
	if(fork() == 0 ){
	//Proceso GestorClientes.-----------------------------------------------------------------


	}else{
	//Proceso Servidor.-----------------------------------------------------------------------


	}
}

void Estacion(int lienteEstacion_soket){ //Zona de riesgo para escribir el fichero
	char buffer[MAX_BUFFER_SIZE];
	size_t bytes_recividos = recv(clienteEstacion_soket, buffer, sizeof(buffer), 0);
	
	if(bytes_recividos > 0){
		//C
		//Abrir el archivo en modo escritura
		FILE *file = fopen(fichero, "a");
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
