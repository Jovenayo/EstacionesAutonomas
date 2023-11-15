Proyecto dedicado al desarrollo de estaciones autónomas, conectadas a un servidor que atenderá las peticiones de los clientes, para administrar la información almacenada por las estaciones.


Ficheros:

#Servidor.c
  Codigo ejecutado por el servidor que crearà dos procesos para escuchar conexiones entrantes y en caso de aceptarlas crear un proceso (Estación o Cliente).

  ##void Estacion(): Codigo del proceso encargado de mantener la conexion de la estacione y almacenar información en el fichero de guardado.

  ##void Cliente(): Código del proceso encargado de mantener la conexión con el cliente y permitir realizar diferentes funciones de lectura de los datos del fichero de guardado.