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
        struct sockaddr_in local, remota;
        unsigned char mensaje[]="Hola red como estas";
        udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
        if(udp_socket==-1)
        {
            perror("\nError al abrir el socket");
            exit(1);
        }
        else
        {
            perror("\Exito al abrir el socket");
            local.sin_family=AF_INET; 
            local.sin_port=htons(8081);   
            local.sin_addr.s_addr=INADDR_ANY;   
            lbind=bind(udp_socket, (struct sockaddr *)&local,sizeof(local));
            if(lbind==-1)
            {
                perror("\nError en bind");
            }
        else
        {
            printf("\Exito en bind");
            local.sin_family=AF_INET; 
            remota.sin_port=htons(8080);   
            inet_aton("10.0.2.15",&remota.sin_addr);
            tam=sendto(udp_socket,mensaje,strlen(mensaje)+1,0,(struct sockaddr *)&remota,sizeof(remota));
        if(tam==-1)
        {
            perror("\n Error al enviar");
            exit(1);            
        }
        else
        {
            printf("\Exito al enviar");
        }
        }
            
        }
        close(udp_socket);
        return 0;           
        }
