#include "client.h"
#include <readline/readline.h>

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	log_info(logger,"Hola! Soy un log");

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	// Loggeamos el valor de config
	ip =config_get_string_value(config,"IP");
	puerto = config_get_string_value(config,"PUERTO");
	/* ---------------- LEER DE CONSOLA ---------------- */

	//leer_consola(logger, registrarLog);
	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	char valorEnviar = config_get_string_value(config,"CLAVE");
	enviar_mensaje(valorEnviar,conexion);
	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);



	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("tp0.log","TP0",1,LOG_LEVEL_INFO);
	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("./cliente.config");
	if (nuevo_config == NULL) {
		printf("no se pudo crear el config");
		exit(1);
	}
	return nuevo_config;
}

void leer_consola(t_log* logger, void (*funcion)(t_log*, char*))
{
	char* leido;

	// La primera te la dejo de yapa

	leido = readline("> ");
	// El resto, las vamos leyendo y logueando hasta recibir un string vacío

    while (leido && leido[0] != '\0')
    {

        funcion(logger, leido);
        free(leido);
    	leido = readline("> ");
    }

    // Liberar la última línea antes de regresar
    free(leido);
	// ¡No te olvides de liberar las lineas antes de regresar!
}

void registrarLog(t_log* logger,char *valor){
	log_info(logger,valor);
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete = crear_paquete();
	// Leemos y esta vez agregamos las lineas al paquete
	leido = readline(">");

	while(leido && leido[0] != '\0'){
		agregar_a_paquete(paquete, leido, conexion);
		free(leido);
		leido = readline(">");
	}
	free(leido);

	enviar_paquete(paquete,conexion);
	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
}
