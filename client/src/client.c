#include "client.h"


int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip = "127.0.0.1";
	char* puerto = "4444";
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"

	log_info(logger, "Hola, Soy un Log");


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();
	log_info(logger, "Config iniciada correctamente.");
	valor = config_get_string_value(config, "CLAVE");
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	// Loggeamos el valor de config
	logger = iniciar_logger();
	log_info(logger, valor);
	log_info(logger, ip);
	log_info(logger, puerto);


	/* ---------------- LEER DE CONSOLA ---------------- */

	log_info(logger, "Iniciando rutina de lectura de consola...");
	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	log_info(logger, "Intentando conexion con servidor...");
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje

	log_info(logger, "Enviando clave de config a servidor...");
	enviar_mensaje(valor, conexion);

	// Armamos y enviamos el paquete

	log_info(logger, "Iniciando rutina de empaquetamiento de mensajes...");
	paquete(conexion);

	log_info(logger, "Terminando programa...");
	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;
	nuevo_logger = log_create("tp0.log", "AM", true, LOG_LEVEL_INFO);
	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;
	nuevo_config = config_create("cliente.config");
	if (nuevo_config == NULL){
		t_log *nuevo_logger = iniciar_logger();
		log_error(nuevo_logger,"NO SE PUDO CREAR EL CONFIG");
		log_destroy(nuevo_logger);
		abort();
	}
	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	while(1){
		leido = readline(">");
		if(!leido){
			break;
		}
		if (strcmp(leido, "") == 0){
			free(leido);
			return 0;
		}
		log_info(logger,leido);
		free(leido);
	}

	// ¡No te olvides de liberar las lineas antes de regresar!

}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete

	while(1){
		leido = readline(">");
		if(!leido){
			break;
		}
		if (strcmp(leido, "") == 0){
			free(leido);
			enviar_paquete(paquete,conexion);
			eliminar_paquete(paquete);
			return 0;
		}
		agregar_a_paquete(paquete,leido,strlen(leido) + 1);
		free(leido);
	}

	
	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);
}
