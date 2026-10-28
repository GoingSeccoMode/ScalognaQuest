#include "gamelib.h"

int main(void){
   time_t t; 
   srand(time(&t));
    unsigned short scelta = 0;

    /*Voglio che il menu' stampi ogni volta il titolo del gioco, ma ovviamente prima chiamo la funzione clear per pulire lo schermo dal precendete*/
    system("clear");
    logo(); 
    do
    {
     if (settato == false) /*Mi devo accertare che il gioco sia stato impostato almeno una singola volta, quindi mi interessa solo il caso negativo*/
        printf("Prima di iniziare a giocare devi impostare il gioco almeno una volta!\n");
        /*funzioni di stampaggio*/
    printf("\t1 - Imposta il Dungeon\n");
    printf("\t2 - Avventurati nel Dungeon\n");
    printf("\t3 - Fuggi dal Dungeon\n");
    scanf("%hu", &scelta);

        switch (scelta)
        {
        case 1:
            system("clear");
            imposta_gioco();
            break;

        case 2:
        if (settato){
            system("clear");
            gioca();
        } else {
            system("clear");
            logo();
            printROSSO("ATTENZIONE IL GIOCO NON E' STATO IMPOSTATO!\n");
        }
       gioca();
        break;
        
        case 3:
            system("clear");
            termina();
            break;

        default:
        system("clear");
        printf("Il comando inserito non e' stato nemmeno pensato\n");
        break;
        }
    }
    while (scelta != 3 );
    
    return 0;
}
