 #include <sys/socket.h>
       #include <netinet/in.h>
       #include <netinet/ip.h> /* superset of previous */   
       #include <stdlib.h>
       #include <unistd.h>
       #include <stdio.h>
       #include <arpa/inet.h>
         #include <string.h>
       
        int main ()
        {
        int udp_socket, lbind, tam;
        struct sockaddr_in servidor, cliente;
        unsigned char mensaje[]="Hola red dns";
        unsigned char paqrec[512];
        udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
        if(udp_socket==-1)
        {
            perror("\nError al abrir el socket");
            exit(1);
        }
        else
        {
            perror("\Exito al abrir el socket");
            servidor.sin_family=AF_INET; 
            servidor.sin_port=htons(8080);   
            servidor.sin_addr.s_addr=INADDR_ANY;   
            lbind=bind(udp_socket, (struct sockaddr *)&servidor,sizeof(servidor));
            if(lbind==-1)
            {
                perror("\nError en bind");
            }
        else
        {
            printf("\Exito en bind");
            //servidor.sin_family=AF_INET; 
            //cliente.sin_port=htons(53);   
            //inet_aton("8.8.8.8",&cliente.sin_addr);
            socklen_t lrecv = sizeof(cliente);
            tam=recvfrom(udp_socket,paqrec,512,0,(struct sockaddr *)&cliente, &lrecv);

        if(tam==-1)
        {
            perror("\n Error al enviar");
            exit(1);            
        }
        else
        {
            printf("\Exito al enviar");
            printf("\n%s\n",paqrec);
        }
        }
            
        }
        close(udp_socket);
        return 0;           
        }