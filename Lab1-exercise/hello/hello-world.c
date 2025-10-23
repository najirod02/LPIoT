#include "contiki.h"
#include "leds.h"
#include "node-id.h"
#include <stdio.h> /* For printf() */

#define PERIOD CLOCK_SECOND
#define LEDS_PERIOD CLOCK_SECOND/10 //not in ms!!

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
  printf("Leds turned off. Finished board setup\n");

  PROCESS_END(); 
}

PROCESS_THREAD(hello_world_process, ev, data)
{
  // Timer object
  static struct etimer timer; // ALWAYS use static variables in processes!
  static struct etimer leds_timer;

  PROCESS_BEGIN();            // All processes should start with PROCESS_BEGIN()

  while (1) {
    // Set a timer
    etimer_set(&timer, PERIOD);
    
    //set leds on and then wait for timer to expire
    leds_on(LEDS_RED|LEDS_GREEN);
    etimer_set(&leds_timer, LEDS_PERIOD);
    printf("Leds turned on\n");
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&leds_timer));
    leds_off(LEDS_RED|LEDS_GREEN);
    printf("Leds turned off\n");

    // Wait for it to expire
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    // Print the node ID
    //printf("Hello, world! I am 0x%04x\n", node_id);
    // Toggle red and green LEDs
    //leds_toggle(LEDS_RED|LEDS_GREEN); // See also leds_on(), leds_off()
  }
  
  PROCESS_END();              // All processes should end with PROCESS_END()
}
/*---------------------------------------------------------------------------*/
