#include "contiki.h"
#include "leds.h"
#include "node-id.h"
#include <stdio.h> /* For printf() */

#define PERIOD CLOCK_SECOND
#define LEDS_PERIOD CLOCK_SECOND/10 //not in ms!!

static struct ctimer timer;  // Timer object
static struct ctimer leds_timer;
/*---------------------------------------------------------------------------*/
// Timer callback function declaration
static void timer_cb(void* ptr);
static void leds_timer_cb(void* ptr);

// Timer callback function (called by the system when the time comes)
static void timer_cb(void* ptr) {
  printf("Leds on\n");
  leds_on(LEDS_RED|LEDS_GREEN);
  ctimer_set(&timer, PERIOD, timer_cb, ptr); // set the same timer again
  ctimer_set(&leds_timer, LEDS_PERIOD, leds_timer_cb, ptr);//when the "second" begins, start timer to turn off the leds
}

static void leds_timer_cb(void* ptr) {
  printf("Leds off\n");
  leds_off(LEDS_RED|LEDS_GREEN);
}

/*---------------------------------------------------------------------------*/
// Declare a process
PROCESS(hello_world_process, "Hello world process");
PROCESS(set_board, "Set board process");
// List processes to start at boot
AUTOSTART_PROCESSES(&set_board, &hello_world_process);
/*---------------------------------------------------------------------------*/
// Implement the process thread function

PROCESS_THREAD(set_board, ev, data){
  PROCESS_BEGIN(); 

  leds_off(LEDS_RED|LEDS_GREEN);
  printf("Finished board setup\n");

  PROCESS_END(); 
}

PROCESS_THREAD(hello_world_process, ev, data)
{
  PROCESS_BEGIN();
  
  // Set the callback timer
  ctimer_set(&timer, PERIOD, timer_cb, NULL);
  //ctimer_set(&leds_timer, LEDS_PERIOD, leds_timer_cb, NULL);

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
