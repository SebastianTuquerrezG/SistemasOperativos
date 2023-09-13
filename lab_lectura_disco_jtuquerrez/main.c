/**
 * @file main.c
 * @author Joan Sebastian Tuquerrez Gomez (jtuquerrez@unicauca.edu.co)
 * @brief Gestion de particiones mbr
 * @version 1.0.0
 * @date 2023-06-29
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief PartitionEntry
 * 	EStructura de datos asignada a cada particion encontrada en el archivo mbr
 */
typedef struct
{
	unsigned char bootable;
	unsigned char start_head;
	unsigned char start_sector;
	unsigned char start_cylinder;
	unsigned char partition_type;
	unsigned char end_head;
	unsigned char end_sector;
	unsigned char end_cylinder;
	unsigned int start_lba;
	unsigned int size;
} PartitionEntry;

/**
 * @brief Get the Partition Type devolver el tipo de particion segun un hexadecimal
 * 
 * @param partitionType caracter hexadecimal para reconocer el tipo de particion
 * @return char* 
 */
char *getPartitionType(unsigned char partitionType)
{
	switch (partitionType)
	{
	case 0x00:
		return "Sin partición";
	case 0x01:
		return "FAT12";
	case 0x04:
		return "FAT16 (partición pequeña)";
	case 0x05:
		return "Extendida";
	case 0x06:
		return "FAT16";
	case 0x07:
		return "NTFS";
	case 0x0B:
		return "FAT32";
	case 0x0C:
		return "FAT32 (partición grande)";
	case 0x0E:
		return "FAT16 (partición extendida)";
	case 0x0F:
		return "Partición extendida (LBA)";
	case 0x83:
		return "Linux";
	default:
		return "Desconocido";
	}
}

int main(int argc, char *argv[])
{
	if(argc != 2){
		printf("Error, debe ingresar el nombre del archivo mbr del disco\n");
		exit(EXIT_FAILURE);
	}
	char *fileMBR = argv[1];
	FILE *file = fopen(fileMBR, "rb");
	if (!file)
	{
		printf("No se pudo abrir el archivo %s\n",&fileMBR);
		return 1;
	}

	unsigned char buffer[512];
	fread(buffer, sizeof(unsigned char), 512, file);

	PartitionEntry *partitions = (PartitionEntry *)(buffer + 446);
	for (int i = 0; i < 4; i++)
	{
		PartitionEntry partition = partitions[i];

		printf("Partición %d:\n", i + 1);
		printf("Tipo de partición: %s\n", getPartitionType(partition.partition_type));
		printf("Comienzo de la particion: %u\n", partition.start_lba);
		printf("Tamaño de la partición: %.1f GB\n", partition.size / 1024.0 / 1024.0 /2);

		if (partition.bootable == 0x80)
		{
			printf("Particion de arranque, BOOT \n");
		}
		printf("\n");
	}

	fclose(file);
	exit(EXIT_SUCCESS);
}
