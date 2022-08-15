/*!! ATTENZIONE: se si chiude il server la mailbox rimane nel sistema !! */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <mqueue.h>
#define SERVER_QUEUE_NAME "/mq_server"
#define QUEUE_PERMISSIONS 0660
#define MAX_MESSAGES 10

struct message {
  int pid;
  char text[64];
}msg_send, msg_rcv;

void main(int argc, char *argv[]) {

  mqd_t qd_server, qd_client; // descrittori di code (mailbox)
  long serial_number = 1; // numero seriale da inviare al client

  char client_queue_name[16]; // nome coda(mailbox) del client
  struct mq_attr attr;  // attributi della coda
  
  attr.mq_flags = 0;  // bloccante
  attr.mq_maxmsg = MAX_MESSAGES;  // numero massimo di messaggi della coda
  attr.mq_msgsize = sizeof(msg_rcv);
  attr.mq_curmsgs = 0;

  printf("Server: Benvenuto!\n");
  qd_server = mq_open(SERVER_QUEUE_NAME, O_RDONLY | O_CREAT, QUEUE_PERMISSIONS, &attr);
  if (qd_server < 0) 
     printf("Errore creazione qd_server\n");

  while (1) {
    
    // preleva il messaggio più vecchio
    mq_receive (qd_server, (char*)&msg_rcv, sizeof(msg_rcv), NULL);

    printf("Server: messaggio ricevuto dal client %d, %s.\n", msg_rcv.pid, msg_rcv.text);
    sprintf(client_queue_name, "/mq_%d", msg_rcv.pid);

	printf("	%s	", client_queue_name);

    // invia il messaggio di risposta al client
    // il client deve creare in anticipo la coda
    qd_client = mq_open(client_queue_name, O_WRONLY);
    if (qd_client < 0)
	printf("Errore qd_client %d\n", qd_client);

    sprintf(msg_send.text, "Benvenuto client %d, il tuo numero e' %ld", msg_rcv.pid, serial_number);

    mq_send(qd_client, (const char*)&msg_send, sizeof(msg_send), 0);
    printf("Server : risposta inviata al client.\n");
    serial_number++;
  }
}

