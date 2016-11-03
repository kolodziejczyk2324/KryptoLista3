#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include "unistd.h"

void init_plik_konfiguracyjy(char** argv);
void usunNL(char* tekst);
void koduj_plik_konfiguracyjny();
void odkoduj_plik_konfiguracyjny();
void czytaj_plik_konfiguracyjny(char* data_for_music_decrypt[3]);
void odkoduj_piosenke(char* sciezka_do_piosenki, char* data_for_music_decrypt[3]);
int wlacz_piosenke();
void instalacja(int argc, char *argv[]);
void stworz_PIN();
int sprawdz_PIN();
void czytaj_prawdziwy_pin();

const char* key = "somepassword";

int main( int argc, char *argv[] ){

	if( access( "plik_konfiguracyjny.txt.enc", F_OK ) == -1)
		instalacja(argc, argv);
	else{
		char* real_pin = NULL;
        	char polecenie_odkodowania[128] = "./my_decrypt K pin.txt.enc pin.txt ";
        	strcat(polecenie_odkodowania, key);
        	system(polecenie_odkodowania);
        	FILE *fp;
        	size_t len = 0;
       	 	fp = fopen("pin.txt", "r");
        	getline(&real_pin, &len, fp);
        	system("rm pin.txt");
		while(sprawdz_PIN(real_pin));
	}

	odkoduj_plik_konfiguracyjny();

	char* data_for_music_decrypt[3] = { NULL, NULL, NULL };
	czytaj_plik_konfiguracyjny(data_for_music_decrypt);

	while(wlacz_piosenke(data_for_music_decrypt));

}

void instalacja(int argc, char *argv[]){
	if(argc != 4){
   		printf("Za malo paramterow\n");
      	        exit(0);
	}
	stworz_PIN();
	init_plik_konfiguracyjy(argv);
	koduj_plik_konfiguracyjny();
}

void stworz_PIN(){
	char pin[32];
	printf("Podaj nowy PIN: ");
	scanf("%s", pin);
	FILE *fp;
        fp = fopen("pin.txt", "w+");
        fprintf(fp, "%s", pin);
        fclose(fp);
	char polecenie_kodowania[128] = "./my_encrypt W pin.txt ";
        strcat(polecenie_kodowania, key);
        system(polecenie_kodowania);
}

int sprawdz_PIN(char* real_pin){
	char pin[32];
	printf("Prosze podac PIN: ");
	scanf("%s", pin);
	if(strcmp("EXIT", pin) == 0)
               	exit(0);
	else if(strcmp(pin, real_pin) == 0)
              	return 0;
	else
		return 1;
}

void czytaj_prawdziwy_pin(char* real_pin){
        char polecenie_odkodowania[128] = "./my_decrypt K pin.txt.enc pin.txt ";
        strcat(polecenie_odkodowania, key);
        system(polecenie_odkodowania);
	FILE *fp;
	size_t len = 0;
 	fp = fopen("pin.txt", "r");
	getline(&real_pin, &len, fp);
	system("rm pin.txt");
}

int wlacz_piosenke(char* data_for_music_decrypt[3]){
	char sciezka_do_piosenki[80];
        printf("Podaj sciezke do zakodowanej piosenki: ");
     	scanf("%s", sciezka_do_piosenki);
        if(strcmp("EXIT", sciezka_do_piosenki) == 0)
 	       return 0;
	odkoduj_piosenke(sciezka_do_piosenki, data_for_music_decrypt);
	system("play music.mp3");
	system("rm music.mp3");
	return 1;
}

void init_plik_konfiguracyjy(char** argv){
	char str[128];
        strcpy(str, argv[1]);strcat(str, "\n");
        strcat(str, argv[2]);strcat(str, "\n");
        strcat(str, argv[3]);strcat(str, "\n");

        FILE *fp;
        fp = fopen("plik_konfiguracyjny.txt", "w+");
        fprintf(fp, "%s", str);
        fclose(fp);
}

void koduj_plik_konfiguracyjny(){
        char polecenie_kodowania[128] = "./my_encrypt W plik_konfiguracyjny.txt ";
        strcat(polecenie_kodowania, key);
        system(polecenie_kodowania);
}

void odkoduj_plik_konfiguracyjny(){
        char polecenie_odkodowania[128] = "./my_decrypt K plik_konfiguracyjny.txt.enc plik_konfiguracyjny.txt ";
        strcat(polecenie_odkodowania, key);
        system(polecenie_odkodowania);
}

void czytaj_plik_konfiguracyjny(char* data_for_music_decrypt[3]){
       	size_t len = 0;
	FILE *fp;
        fp = fopen("plik_konfiguracyjny.txt", "r");
	for(int i=0 ; i<3 ; i++){
        	getline(&data_for_music_decrypt[i], &len, fp);
		usunNL(data_for_music_decrypt[i]);
	}
        fclose(fp);
	system("rm plik_konfiguracyjny.txt");
}

void odkoduj_piosenke(char* sciezka_do_piosenki, char* data_for_music_decrypt[3]){
        char polecenie_odkodowania_piosenki[256] = "./my_decrypt H ";
        strcat(polecenie_odkodowania_piosenki, sciezka_do_piosenki);
        strcat(polecenie_odkodowania_piosenki, " ");
        strcat(polecenie_odkodowania_piosenki, "music.mp3 ");
        strcat(polecenie_odkodowania_piosenki, data_for_music_decrypt[0]);
        strcat(polecenie_odkodowania_piosenki, " ");
        strcat(polecenie_odkodowania_piosenki, data_for_music_decrypt[1]);
        strcat(polecenie_odkodowania_piosenki, " ");
        strcat(polecenie_odkodowania_piosenki, data_for_music_decrypt[2]);

        system(polecenie_odkodowania_piosenki);
}

void usunNL(char* tekst){
	tekst--;
	while(*(++tekst) != '\n');
	*tekst = '\0';
}
