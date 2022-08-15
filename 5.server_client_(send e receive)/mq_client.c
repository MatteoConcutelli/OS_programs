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
  char client_queue_name[16];
  mqd_t qd_server, qd_client; // descrittori code

  sprintf(client_queue_name, "/mq_%d", getpid());
  printf("client_queue_name : %s\n", client_queue_name);

  struct mq_attr attr;

  attr.mq_flags = 0;
  attr.mq_maxmsg = MAX_MESSAGES;
  attr.mq_msgsize = sizeof(msg_send);
  attr.mq_curmsgs = 0;

  
  qd_client = mq_open (client_queue_name, O_RDONLY | O_CREAT , QUEUE_PERMISSIONS, &attr);
  if (qd_client < 0) 
      printf("Errore creazione qd_client\n");
  
  /* apre la coda del server per inviare il proprio identificatore (pid) */
  qd_server = mq_open (SERVER_QUEUE_NAME, O_WRONLY);
  if (qd_server < 0)
  	printf("%s, apertura mailbox server non riuscita\n", argv[1]);
  	
  printf("Richiedi un numero (Premi <INVIO>):");
  char in_buf[10];

  msg_send.pid = getpid();
  if (argc > 1) 
    strcpy(msg_send.text, argv[1]); // nikname
  else 
    strcpy(msg_send.text, "Pietro");  // default nickname

  // fgets legge dal flusso d'input tastiera (stdin) una str di 2 caratteri e la salva in in_buf
  // ritorna al puntatore alla stringa
  while (fgets(in_buf, 2, stdin)) {
    
    //invia il msg al server
    mq_send (qd_server, (const char*)&msg_send, sizeof(msg_send), 0);

    // riceve risposta dal server
    mq_receive (qd_client, (char*)&msg_rcv, sizeof(msg_rcv), NULL);

    // visualizza il messaggio ricevuto dal server
    printf("Client: messaggio dal server: %s\n\n", msg_rcv.text);
    printf("Richiedi un numero (Premi <INVIO>): ");
  }
  
  mq_close (qd_client);
  mq_unlink(client_queue_name); // elimina coda
  printf("Client: ciao\n");

}
