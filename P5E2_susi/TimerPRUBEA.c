#include "cmsis_os2.h"                                                          // CMSIS RTOS header file
 
/*----------------------------------------------------------------------------
 *      Timer: Sample timer functions
 *---------------------------------------------------------------------------*/
 
/*----- One-Shoot Timer Example -----*/
osTimerId_t tim_id1;    // identificador del temporizador
static uint32_t exec1;  // es un argumento que se pasa a la función de callback del temporizador.
                        // es estático para que exista durante toda la ejecución del programa

// One-Shoot Timer Function = función que se ejecutará cuando el temporizador dispare (=termine)
static void Timer1_Callback (void const *arg) {
  // add user code here
}
 
/*----- Periodic Timer Example -----*/
osTimerId_t tim_id2;
static uint32_t exec2;
 
// Periodic Timer Function = función que se ejecutará repetidamente cada vez que el temporizador expire
static void Timer2_Callback (void const *arg) {
  // add user code here
}
 
// Función que crea y arranca los temporizadores (en este caso DOS)
int Init_Timers (void) {
  
  osStatus_t status;  //almacenará el estado devuelto por las funciones del RTOS
                      //(osOK si todo va bien, otros valores si hay errores)
 
  // Create one-shoot timer = creación del one shoot timer
  exec1 = 1U;   //asigna valor uno (este valor será pasado al callback = Timer1_Callback)
  
  //Crea un temporizador one-shot (dispara una sola vez)
  tim_id1 = osTimerNew((osTimerFunc_t)&Timer1_Callback, osTimerOnce, &exec1, NULL);
  /*PARÁMETROS : función que se ejecutará al expirar el temporizador.
   *             tipo de temporizador
   *             argumento que se pasará al callback
   *             atributos del temporizador (NULL=sin atributos especiales)
   *
   * Retorna un identificador del temporizador (tim_id1) si se creó correctamente */
  
  if (tim_id1 != NULL) {  // Si se ha creado el timer correctamente
    status = osTimerStart(tim_id1, 100U); // arranca el temporizador con 100 ms de retardo
    // si se ha iniciado el temporizador correctamente devuelve osOK, aquí status<=osOK
    if (status != osOK) { //si falla poner en marcha el temporizador
      return -1;  //para indicar error
    }
  }
 
  // Create periodic timer = creación del timer periódico
  exec2 = 2U;
  tim_id2 = osTimerNew((osTimerFunc_t)&Timer2_Callback, osTimerPeriodic, &exec2, NULL);
  if (tim_id2 != NULL) {  // Periodic timer created
    // start timer with periodic 1000ms interval
    status = osTimerStart(tim_id2, 1000U);            
    if (status != osOK) {
      return -1;
    }
  }
  return NULL; //Devuelve Null si todo se creó y arrancó correctamente
}

/* EXEC !!
 * exec1 y exec2 son los “datos de usuario” que cada timer puede pasar a su función callback
 * permite que una misma función callback sirva para varios timers, diferenciando los datos que reciben
 */