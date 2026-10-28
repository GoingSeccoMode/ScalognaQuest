#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

/*Prendendo anche spunto, qui ho definito due macro per i colori con corrispettivo ANSI e sotto il comando per stampare con il colore selezionato e dopo riportare
al colore base*/
#define VERDE "\x1b[32m" 
#define ROSSO "\x1b[31m"
#define DEAFULT "\x1b[0m"
#define printVERDE(X) printf("%s%s%s",VERDE,X,DEAFULT)
#define printROSSO(X) printf("%s%s%s",ROSSO,X,DEAFULT)
#define SiVaALetto(time) sleep(time)

/*Servono sempre*/
static bool settato = false;

enum Classe_giocatore {barbaro, nano, elfo, mago, all_class};
enum Tipo_zona {corridoio, scala, sala_banchetto, magazzino, giardino, posto_guardia, prigione, cucina, armeria, tempio, all_zones};
enum Tipo_tesoro {nessun_tesoro, veleno, guarigione, doppia_guarigione};
enum Tipo_porta {nessuna_porta, porta_normale, porta_da_scassinare};
enum Tipo_mostro {Golem, Scheletro};

typedef struct Giocatore
{
    char nome_giocatore [64];
    enum Classe_giocatore classe;
    struct Zona_segrete* posizione;
    unsigned char PV;
    unsigned char dadi_attacco;
    unsigned char dadi_difesa;
    unsigned char mente;
    unsigned char potere_speciale;
    unsigned char movimento;
} Giocatore;

typedef struct Zona_segrete
{
    enum Tipo_zona zona;
    enum Tipo_tesoro tesoro;
    enum Tipo_porta porta;
    struct Zona_segrete* zona_successiva;
    struct Zona_segrete* zona_precedente;
} Zona_segrete;

typedef struct Abitante
{
    unsigned char nome_mostro;
    unsigned char PV;
    unsigned char dadi_attacco;
    unsigned char dadi_difesa;
} Abitante;




/*funzioni base del programma*/
void logo();
void imposta_gioco();
void gioca();
void termina();
