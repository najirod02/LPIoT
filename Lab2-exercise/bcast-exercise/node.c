/*---------------------------------------------------------------------------*/
#include "contiki.h"
#include "dev/radio.h"
#include "net/netstack.h"
#include "net/rime/rime.h"
#include "random.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
/*---------------------------------------------------------------------------*/
PROCESS(broadcast_process, "Broadcast Process");
AUTOSTART_PROCESSES(&broadcast_process);
/*---------------------------------------------------------------------------*/
static void broadcast_recv(struct broadcast_conn *c, const linkaddr_t *from)
{
  /* TODO:
   * - Get the string from the received message
   * - Print the sender address and the received message
   *   Expected print: Recv from XX:XX - Message: 'Hello Word!"
   */
  size_t len = packetbuf_datalen();
  char msg[len + 1];
  memcpy(msg, (char *)packetbuf_dataptr(), len);
  msg[len] = '\0';
  printf("Recv from %02X:%02X - Message: %s", from->u8[0], from->u8[1], msg);

}

/* TODO - STEP 2: (To be implemented *ONLY when* all other TODOs work as expected!)
 * - Implement the sent callback function (broadcast_sent): print status and number of TX
*/
static void broadcast_sent(struct broadcast_conn *conn, int status, int num_tx){
  printf("Status: %d - Number of TX: %d\n", status, num_tx);
}

static const struct broadcast_callbacks broadcast_cb = { // TODO: Assign pointers to your callback functions
  .recv = broadcast_recv, 
  .sent = broadcast_sent,
};
static struct broadcast_conn broadcast;
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(broadcast_process, ev, data)
{
  static struct etimer et;

  static bool toggleMsg = false;
  static char msg[] = "Hello from Luca!\n";

  PROCESS_BEGIN();

  SENSORS_ACTIVATE(button_sensor);

  /* TODO:
   * - Initialize the Rime broadcast primitive
   */
  broadcast_open(&broadcast, 123, &broadcast_cb);

  /* Print node link layer address */
  printf("Node Link Layer Address: %02X:%02X\n",
    linkaddr_node_addr.u8[0], linkaddr_node_addr.u8[1]);

  while(1) {
    /* Delay 5-8 seconds */
    etimer_set(&et, CLOCK_SECOND * 5 + random_rand() % (CLOCK_SECOND * 3));

    /* Wait for the timer to expire */
    //PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    PROCESS_WAIT_EVENT();
    
    if(ev == PROCESS_EVENT_TIMER  && etimer_expired(&et)){
      /* TODO:
       * - Create a text message
       * - Fill the message in the packet buffer
       * - Send!
       */
      //char msg[] = "Hello World!\n";
      packetbuf_clear();
      packetbuf_copyfrom((void *)msg, strlen(msg));
      broadcast_send(&broadcast);
    } else if (ev == sensors_event && data == &button_sensor){
      //you can comment this line, useful for debug
      printf("Button pressed in: %02X:%02X\n",
        linkaddr_node_addr.u8[0], linkaddr_node_addr.u8[1]);
        
      toggleMsg = !toggleMsg;

      if(toggleMsg){
        //send Matteo
        char new_msg[] = "Hello from Matteo!\n";
        memcpy(msg, new_msg, strlen(new_msg));
      } else {
        //send Luca
        char new_msg[] = "Hello from Luca!\n";
        memcpy(msg, new_msg, strlen(new_msg));
      }
      
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
