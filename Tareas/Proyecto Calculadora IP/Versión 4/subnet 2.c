// Subnet Calculator 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdint.h>

int ipVerify(char* ipAddress, unsigned char* octetArray) {
	// La función verifica que se ha ingresado una IP válida, y luego actualiza el array de octetos con los octetos validados.

	char* token;
	int i = 0;
	int j = 0;
	int periodCount = 0;

	// Continuaremos obteniendo tokens mientras no sea nulo
	token = strtok(ipAddress, ".");
	while (token != NULL) {

		// Recorre cada carácter y verifica si es un dígito
		// Si no lo es, interrumpe el ciclo. Usamos j para ver si se iteró la cantidad correcta de veces
		for (j=0; j<strlen(token); j++) {
			if (isdigit(token[j]) == 0) {
				break;
			}
		}

		// Si se ha ingresado la cantidad correcta de dígitos, confirma el octeto como validado y lo agrega al array
		if (strlen(token) > 0 && strlen(token) < 4 && j == strlen(token)
				&& atoi(token) < 256 && atoi(token) >= 0) {
			periodCount++;
			octetArray[i] = atoi(token);
		} else {
			// No tiene sentido continuar si incluso un octeto falla la prueba
			break;
		}

		i++;

		token = strtok(NULL, ".");
	}

	if (periodCount != 4) {
		return 0;
	} else {

		return 1;
	}
}

void printSubnetInfo(uint32_t* addressOctets, int* CIDR, int* subnetBits) {
// Imprime información sobre la subred dada.
// Toma punteros a los datos requeridos, sin embargo, no cambia nada dentro de ellos.
// Cualquier manipulación requerida se hace con variables locales

	uint32_t netAddress;
	uint32_t netMask;
	
	netMask = (0xFFFFFFFF << (32 - (*CIDR + *subnetBits)) & 0xFFFFFFFF);
	netAddress = *addressOctets & netMask;

	// Desempaqueta y muestra la dirección de red
	printf("\nDireccion de red: %d.%d.%d.%d/%d\n", (netAddress >> 24) & 0xFF, (netAddress >> 16) & 0xFF,
						    (netAddress >> 8) & 0xFF, (netAddress) & 0xFF, *CIDR + *subnetBits);

	// Resta la dirección de red de la dirección de broadcast y resta uno del resultado para el total de hosts
	printf("Total hosts: %d\n", ((netAddress | ~netMask) - netAddress) - 1);

	// Muestra la primera dirección de host sumando a cada uno de nuestros octetos desempaquetados
	printf("Primera direccion de host: %d.%d.%d.%d\n", ((netAddress + 1) >> 24) & 0xFF, ((netAddress + 1) >> 16) & 0xFF,
						    ((netAddress + 1) >> 8) & 0xFF, (netAddress + 1) & 0xFF);
	
	//Realiza un OR bit a bit con la dirección int y la máscara negada para obtener la dirección de broadcast en la variable
	netAddress = netAddress | ~netMask;

	// Reste de la dirección de broadcast para obtener la dirección del host final
	printf("Ultima direccion de host: %d.%d.%d.%d\n", ((netAddress - 1) >> 24) & 0xFF, ((netAddress - 1) >> 16) & 0xFF,
						   ((netAddress - 1) >> 8) & 0xFF, (netAddress - 1) & 0xFF);
	
	// Desempaqueta y muestra la dirección de broadcast
	printf("Broadcast address: %d.%d.%d.%d\n", (netAddress >> 24) & 0xFF, (netAddress >> 16) & 0xFF,
						   (netAddress >> 8) & 0xFF, (netAddress) & 0xFF);
}

int main() {

	char ipAddress[18];
	char buffer[4];
	int CIDR;
	unsigned char* octetArray;
	octetArray = calloc(4, sizeof(char));
	uint32_t addressOctets;
	
	int subnetNumber;
	int subnetBits = 0;
	int totalSubnets = 0;
	uint32_t currentSubnet;
	int i;

	// conseguir la dirección IP
	while (1) {
		printf("Ingrese la direccion IPv4 ahora: ");
		fgets(ipAddress, 17, stdin);
		ipAddress[strlen(ipAddress)-1] = '\0';

		printf("Verificando: %s... ", ipAddress);

		// Verifícador de IP
		if (ipVerify(ipAddress, octetArray) == 0) {
			printf("IP no valida ingresada.\n");
		} else {
			printf("Direccion verificada!\n");
			break;
		}
	}

	//Obtén el número CIDR
	while (1) {
		printf("Ingresa la mascara de subred en notacion CIDR ahora: ");
		fgets(buffer, 4, stdin);

		CIDR = atoi(buffer);

		if (CIDR > 0 && CIDR < 32) {
			break;
		} else {
			printf("Se ha introducido un CIDR invalido. Intenta otra vez.\n");
		}
	}

	printf("\n%d.%d.%d.%d/%d ", octetArray[0], octetArray[1], octetArray[2], octetArray[3], CIDR);

	if (octetArray[0] > 239) {
		printf("(Clase E)\n");
	} else if (octetArray[0] > 223) {
		printf("(Clase D)\n");
	} else  if (octetArray[0] > 191) {
		printf("(Clase C)\n");
	} else if (octetArray[0] > 127) {
		printf("(Clase B)\n");
	} else {
		printf("(Clase A)\n");
	}

	// Empaqueta los bits de la dirección IP en un entero
	addressOctets = (octetArray[0] << 24) | (octetArray[1] << 16) | (octetArray[2] << 8) | (octetArray[3]);

	// Llama a la función subnetinfo para la red
	printSubnetInfo(&addressOctets, &CIDR, &subnetBits);

	do {
		printf("Ingresa el numero de redes requeridas, o q para salir: ");
		fgets(buffer, 4, stdin);
		subnetNumber = atoi(buffer);

		if (subnetNumber == 0) {
			printf("Saliendo...\n");
			exit(0);
		}

		// Determina la cantidad de bits necesarios para contener las redes requeridas
		while (subnetNumber > totalSubnets) {
			subnetBits++;
			totalSubnets = pow(2, subnetBits) -2;
		}

		// Verifica que tenemos la cantidad necesaria de bits para subnetear exitosamente
		if ((CIDR + subnetBits) > 31) {
			printf("La cantidad de redes es demasiado grande para ser acomodada.\n");
		}
	} while ((CIDR + subnetBits) > 31);

	printf("\nTotal de subredes a ser creadas: %d\n-------------------------------", totalSubnets); 
	// Construye los bits de red de la subred, luego imprime la información
	for (i=0; i<totalSubnets; i++) {
		currentSubnet = (addressOctets & ((0xFFFFFFFF << (32 - CIDR)) & 0xFFFFFFFF))
				| i << (32 - (CIDR + subnetBits));
		printSubnetInfo(&currentSubnet, &CIDR, &subnetBits);
	}

	free(octetArray);
	
	return 0;
}

