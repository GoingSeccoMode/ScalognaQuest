#include "gamelib.h"

void logo(void){
    printVERDE("███████╗ ██████╗ █████╗ ██╗      ██████╗  ██████╗ ███╗   ██╗ █████╗        ██████╗ ██╗   ██╗███████╗███████╗████████╗\n");
    printVERDE("██╔════╝██╔════╝██╔══██╗██║     ██╔═══██╗██╔════╝ ████╗  ██║██╔══██╗      ██╔═══██╗██║   ██║██╔════╝██╔════╝╚══██╔══╝\n");
    printVERDE("███████╗██║     ███████║██║     ██║   ██║██║  ███╗██╔██╗ ██║███████║█████╗██║   ██║██║   ██║█████╗  ███████╗   ██║   \n");
    printVERDE("╚════██║██║     ██╔══██║██║     ██║   ██║██║   ██║██║╚██╗██║██╔══██║╚════╝██║▄▄ ██║██║   ██║██╔══╝  ╚════██║   ██║   \n");
    printVERDE("███████║╚██████╗██║  ██║███████╗╚██████╔╝╚██████╔╝██║ ╚████║██║  ██║      ╚██████╔╝╚██████╔╝███████╗███████║   ██║   \n");
    printVERDE("╚══════╝ ╚═════╝╚═╝  ╚═╝╚══════╝ ╚═════╝  ╚═════╝ ╚═╝  ╚═══╝╚═╝  ╚═╝       ╚══▀▀═╝  ╚═════╝ ╚══════╝╚══════╝   ╚═╝   \n");
    printVERDE("                                                       Un gioco in C                                                 \n");
}

static int nGiocatori = 0;
static struct Giocatore* arraygiocatori[4];
/*static struct Giocatore* GiocatorePtr = NULL;*/ //Allora il nome del ptr e' GiocatorePtr, e' di tipo static e punta alla struct Giocatore!
static struct Abitante* AbitantePtr = NULL;
static int Sacrificio = 0;
static int StanzeDellaPauraEDellaFameCreate = 0;
static bool combattimento = false;
static int Deceduti = 0;

//Dichiarazioni dei puntatori per la lista doppiamente linkata della mappa
static struct Zona_segrete *pFirst = NULL;
static struct Zona_segrete *pLast = NULL;

static int ACaso(int alvolo);
static void RandOrdine(int*, int);
static void Scambio(int*, int*);
static void AMonsterAppears(void);


//IMPOSTA GIOCO
//static void ControlloSettato(void);
static void GenGiocatori(void);

//IMPOSTA DUNGEON
static void ImpostazioneMappa(void);
static void CreaDungeon(void);
static void InserisciSegreta();
static void SradicaSegreta();
static void ControllaDungeon(void);
static void FinisciCreazione(void);

//GIOCA
static void Avanza(struct Giocatore*);
static void Indietreggia(struct Giocatore*);
static void StampaGiocatore(struct Giocatore*);
static void StampaStanza(struct Giocatore*);
static void ApriPorta(struct Giocatore*);
static void PrendiTesoro(struct Giocatore*);
static void PassaTurno(void);
static void Scappa(struct Giocatore*, struct Abitante*);
static void Combatti(struct Giocatore*, struct Abitante*);
static void GiocaPotere(struct Giocatore*, struct Abitante*);



static int ACaso(int alvolo) {
    int r = (rand()% alvolo);
    return r;
}

static void RandOrdine (int* arr, int n) {
    for (int i = (n -1); i > 0; i--) {
        Scambio(&arr[ACaso(i)], &arr[i]);
    }    
}

static void Scambio (int* a, int* b) {
    int n = *a;
    *a = *b;
    *b = n;
}

static int AcasoMostro(int min, int max){
    return min + rand() %(max - min +1);
}

static void AMonsterAppears(void){
    int coinFlip = ACaso(2)+1;
    
    AbitantePtr = (struct Abitante*)malloc(sizeof(struct Abitante));
    struct Abitante* temp = AbitantePtr;
    
    //Volevo mettere due tipi di nemici che creo appena entro nell'incontro
    switch (coinFlip)   
    {
    case 1:
        temp->dadi_attacco = AcasoMostro(2,4);
        temp->dadi_difesa = AcasoMostro(1,3);
        temp->PV = AcasoMostro(4,5);
        temp->nome_mostro = Golem;
        break;
    case 2:
        temp->dadi_attacco = AcasoMostro(3,4);
        temp->dadi_difesa = AcasoMostro(1,3);
        temp->PV = AcasoMostro(2,5);
        temp->nome_mostro = Scheletro;
        break;
    default:
    break;
    }
}

//static void ControlloSettato(void){}

static void GenGiocatori(void){

    //per come hai fatto devi separare in due sta cosa

    do //Chiedere se ha senso metterla direttamente sotto la scelta di imposta gioco come Flow.
    {
        printVERDE("Prima di tutto in quanti si vogliono avventurare in questo Dungeon?\n");
        scanf("%d", &nGiocatori);
        if (nGiocatori > 4 || nGiocatori < 1)
        {
            printROSSO("NUMERO GIOCATORI NON VALIDO\n\n");
            system("clear");
        }
    } while (nGiocatori > 4 || nGiocatori < 1);
    
    //GiocatorePtr = (struct Giocatore*) malloc(sizeof(struct Giocatore));
    
    
    int sceltaClasse = 0;

    for (int i = 0; i < nGiocatori; i++)
    {
        arraygiocatori[i] = (struct Giocatore*)malloc(sizeof(struct Giocatore));
        struct Giocatore* temp = arraygiocatori[i];

        printf("Inserisci il nome del tuo eroe:");
        scanf("%s", arraygiocatori[i]->nome_giocatore);

        printf("\nQuale classe vuoi giocare?\n");
        printf("| 1 - Barbaro | 2 - Nano | 3 - Elfo | 4 - Mago |\n");
        printf("|Stas Barbaro: 3 dadi attacco, 2 dadi difesa, 8 PV, 1 o 2 mente, 0 cariche potere|\n");
        printf("|Stas Nano: 2 dadi attacco, 2 dadi difesa, 7 PV, 2 o 3 mente, 1 cariche potere|\n");
        printf("|Stas Elfo: 2 dadi attacco, 2 dadi difesa, 6 PV, 3 o 4 mente, 1 cariche potere|\n");
        printf("|Stas Mago: 1 dadi attacco, 2 dadi difesa, 4 PV, 4 o 5 mente, 3 cariche potere|\n");
        scanf("%d", &sceltaClasse);

        switch (sceltaClasse)
        {
        case 1:
            printf("Hai scelto il Barbaro!\n");
            temp ->classe = barbaro;
            temp ->dadi_attacco = 3;
            temp ->dadi_difesa = 2;
            temp ->PV = 8;
            temp ->mente = ACaso(2) +1;
            temp ->potere_speciale = 0;
            printf("Vuoi sacrificare un punto mente per ottenere un punto vita?\nVuoi sacrificare un punto vita per punto mente?\n\n");
            printf("Premi 1 per ottenere un punto vita\nPremi 2 per ottenere un punto mente\n");
            scanf("%d", &Sacrificio);
                if (Sacrificio == 1)
                {
                    printf("Hai scelto di ottenere vita extra\n");
                    temp->mente -= 1;
                    temp->PV += 1;
                } else {
                    printf("Hai scelto di ottenere mente extra\n");
                    temp->PV -= 1;
                    temp->mente += 1;
                }
            break;

        case 2:
            printf("Hai scelto il nano!\n");
            temp ->classe = nano;
            temp ->dadi_attacco = 2;
            temp ->dadi_difesa = 2;
            temp ->PV = 7;
            temp ->mente = ACaso(2) +2;
            temp ->potere_speciale = 1;
            printf("Vuoi sacrificare un punto mente per ottenere un punto vita?\nVuoi sacrificare un punto vita per punto mente?\n\n");
            printf("Premi 1 per ottenere un punto vita\nPremi 2 per ottenere un punto mente\n");
            scanf("%d", &Sacrificio);
                if (Sacrificio == 1)
                {
                    printf("Hai scelto di ottenere vita extra\n");
                    temp->mente -= 1;
                    temp->PV += 1;
                } else {
                    printf("Hai scelto di ottenere mente extra\n");
                    temp->PV -= 1;
                    temp->mente += 1;
                }
            break;

        case 3:
            printf("Hai scelto l'elfo!\n");
            temp ->classe = elfo;
            temp ->dadi_attacco = 2;
            temp ->dadi_difesa = 2;
            temp ->PV = 6;
            temp ->mente = ACaso(2) + 3;
            temp ->potere_speciale = 1;
            printf("Vuoi sacrificare un punto mente per ottenere un punto vita?\nVuoi sacrificare un punto vita per punto mente?\n\n");
            printf("Premi 1 per ottenere un punto vita\nPremi 2 per ottenere un punto mente\n");
            scanf("%d", &Sacrificio);
                if (Sacrificio == 1)
                {
                    printf("Hai scelto di ottenere vita extra\n");
                    temp->mente -= 1;
                    temp->PV += 1;
                } else {
                    printf("Hai scelto di ottenere mente extra\n");
                    temp->PV -= 1;
                    temp->mente += 1;
                }
        break;

        case 4:
            printf("Hai scelto il mago!\n");
            temp->classe = mago;
            temp->dadi_attacco = 1;
            temp->dadi_difesa = 2;
            temp->PV = 4;
            temp->mente = ACaso(2) +4;
            temp->potere_speciale = 3;
            printf("Vuoi sacrificare un punto mente per ottenere un punto vita?\nVuoi sacrificare un punto vita per punto mente?\n\n");
            printf("Premi 1 per ottenere un punto vita\nPremi 2 per ottenere un punto mente\n");
            scanf("%d", &Sacrificio);
                if (Sacrificio == 1)
                {
                    printf("Hai scelto di ottenere vita extra\n");
                    temp->mente -= 1;
                    temp->PV += 1;
                } else {
                    printf("Hai scelto di ottenere mente extra\n");
                    temp->PV -= 1;
                    temp->mente += 1;
                }
        break;
        
        default:
        system("clear");
        printf("La classe scelta da te non e' ancora stata pensata. Conttatare lo sviluppatore per avere una mod!");
        break;
        }
    }
    for (int i = nGiocatori; i < 4; i++)
    {
        arraygiocatori[i] = NULL;
    }

}

static void CreaDungeon(void){

    //SE NON CI SONO DELLE STANZE NE VENGONO CREATE 15, ALTRIMENTI DEALLOCO QUELLE ESISTENTI E NE FACCIO 15 NUOVE
    if (StanzeDellaPauraEDellaFameCreate == 0)  {
        printVERDE("\n\nIL DUNGEON STA VENENDO CREATO\n\n");
        
        for (int i = 0; i < 15; i++) {
        StanzeDellaPauraEDellaFameCreate++;
        struct Zona_segrete *New = (struct Zona_segrete*)malloc(sizeof(struct Zona_segrete));
        
        if (pFirst == NULL && pLast == NULL)
        {
            pFirst = New;
        } else
        {
            pLast->zona_successiva = New;
            New->zona_precedente = pLast;
        }
        pLast = New;
        New->zona = ACaso(10);
        New->tesoro = ACaso(4); 
        New->porta = ACaso(3);
        }   
    } else {
        printROSSO("\n\nIL PRECENDENTE DUNGEON STA VENENDO RIMPIAZZATO\n\n");

        //HO RIFATTO QUESTO PEZZO UN NUMERO ASSURDO DI VOLTE, NON VOGLIO PIU' VEDERLO
        //AGGIORNAMENTO 21:15, PERCHE' QUI SBAGLIA E SOPRA NO
        while (pFirst != NULL)
        {
            struct Zona_segrete* t = pFirst;
            pFirst = pFirst->zona_successiva;
            free(t);
        }
        
        StanzeDellaPauraEDellaFameCreate = 0;
        pFirst = NULL;
        pLast = NULL;

        for (int i = 0; i < 15; i++)
        {
            struct Zona_segrete* New = (struct Zona_segrete*)malloc(sizeof(struct Zona_segrete));
            StanzeDellaPauraEDellaFameCreate++;
            if (pFirst == NULL && pLast == NULL)
            {
                pFirst = New;
                pFirst->zona_precedente = NULL;
            } else {
                pLast->zona_successiva = New;
                New->zona_precedente = pLast;
            }
            pLast = New;
            pLast->zona_successiva = NULL;  //INIZIO A SOSPETTARE DI AVER SEMPRE SBAGLIATO A GESTIRE I PFIRST E PLAST SU QUESTO CICLO

            New->zona = ACaso(10);
            New->tesoro = ACaso(4);
            New->porta = ACaso(3);
            
        }
        

        /*for (int i = 0; i < 15; i++) {
            struct Zona_segrete *New = (struct Zona_segrete*) malloc (sizeof(struct Zona_segrete));
        
            StanzeDellaPauraEDellaFameCreate++;
            if (pFirst == NULL && pLast == NULL)
            {
                pFirst = New;
            } else
            {
                pLast->zona_successiva = New;
                New->zona_precedente = pLast;
            }
            pLast = New;
            New->zona = ACaso(10);
            New->tesoro = ACaso(4); 
            New->porta = ACaso(3);
        }*/
    }
}

static void InserisciSegreta(){
    int posizione = 0;
    printf("Inserisci la posizione in cui creare una nuova cella del dungeon (I valori accettati vanno da 1 fino all'indice dell'ultima stanza creata!)\n\n");
    scanf("%d", &posizione);

    if (StanzeDellaPauraEDellaFameCreate < 1 || StanzeDellaPauraEDellaFameCreate < posizione)
    {
        printROSSO("LA POSIZIONE DATA NON E' VALIDA\n\n");
        return;
    }

    if (posizione == 1) //se lo vuole mettere all'inizio
    {
       printVERDE("\tIl Dungeon sta cambiando\n");
       struct Zona_segrete *New = (struct Zona_segrete*)malloc(sizeof(struct Zona_segrete));

       New->zona = ACaso(10);
       New->tesoro = ACaso(4);
       New->porta = ACaso(3);
       New->zona_successiva = pFirst;
       New->zona_precedente = NULL;

       if (pFirst != NULL) {
        (pFirst)->zona_precedente = New;
        pFirst = New;
       }

        StanzeDellaPauraEDellaFameCreate++;
    }
    else
    {
            struct Zona_segrete *Nodo_attuale = pFirst;  //APPROFONDIRE E VEDERE SE E- GIUSTO

            for (int i = 1; i < posizione - 1 && Nodo_attuale != NULL; i++)
            {
                Nodo_attuale = Nodo_attuale->zona_successiva;
            }

            struct Zona_segrete *New = (struct Zona_segrete*)malloc(sizeof(struct Zona_segrete));
            New->zona_precedente = Nodo_attuale;
            New->zona_successiva = Nodo_attuale->zona_successiva;
            Nodo_attuale->zona_successiva = New;
            New->porta = ACaso(3);
            New->tesoro = ACaso(4);
            New->zona = ACaso(10);
            
            StanzeDellaPauraEDellaFameCreate++;
    }
}

static void SradicaSegreta(){

    int posizione = 0;
    printf("Inserisci la posizione in cui creare una nuova cella del dungeon (I valori accettati vanno da 0 fino all'indice dell'ultima stanza creata!)");
    scanf("%d", &posizione);

    if (pFirst == NULL){    //CONTROLLO STANDARD PER VEDERE SE LA LISTA E' VUOTA
        printROSSO("NON CI SONO STANZE DA ELIMINARE\n\n");
        return;
    }

    struct Zona_segrete *Nodo_Attuale = pFirst;

    for (int i = 1; Nodo_Attuale != NULL && i < posizione - 1; i++){
        Nodo_Attuale = Nodo_Attuale->zona_successiva;
    }

    if (Nodo_Attuale == NULL){      //CONTROLLO SE LA POSIZIONE DATA ESISTE
        return;
    }
    //CORREZIONI DEI PUNTATORI DEL SUCCESSIVO E PRECEDENTE DEL NODO APPENA ELIMINATO

    if (Nodo_Attuale->zona_precedente != NULL){
        Nodo_Attuale->zona_precedente->zona_successiva = Nodo_Attuale->zona_successiva;
    }
    if (Nodo_Attuale->zona_successiva != NULL){
        Nodo_Attuale->zona_successiva->zona_precedente = Nodo_Attuale->zona_precedente;
    }
    //SE IL NODO DATO E' LA TESTA DELLA LISTA
    if (pFirst == Nodo_Attuale){
        pFirst = Nodo_Attuale->zona_successiva;    
    }
    
    free(Nodo_Attuale); //ELIMINAZIONE DEL NODO INTERESSATO
    StanzeDellaPauraEDellaFameCreate--;
    printVERDE("\tIl Dungeon sta cambiando\n");
} 

//Controlla i campi di tutte le stanze del Dungeon
static void ControllaDungeon(void){
    //FUNZIONE USA UN WHILE CHE SCORRE LUNGO TUTTA LA LISTA DELLE STANZE FINO A QUANDO NON INCAPPA NELL'ULTIMA
    struct Zona_segrete *current = pFirst;

    //CONTROLLO SE LE STANZE CI SONO O NO
    if (current == NULL)
    {
        printROSSO("NON CI SONO STANZE NEL DUNGEON!\n\n");
        return;
    }
    
    int SegreteScorse = 1;  //ID DELLE STANZE
    while (current != NULL)
    {
        //char *zonenames[all_zones] = {"corridoio", "scala", "sala_banchetto", "magazzino", "giardino", "posto_guardia", "prigione", "cucina", "armeria", "tempio"};
        printf("Segreta num %d\n", SegreteScorse);
        printf("Tipo zona %d\n", current->zona);
        printf("Tipo della porta %d\n", current->porta);
        printf("Tipo di tesoro %d\n\n\n", current->tesoro);

        SegreteScorse++;
        current = current->zona_successiva;
         //PURE RAGIONI ESTETICHE
    }
    if (SegreteScorse < 15)
    {
        printROSSO("ATTENZIONE! CI SONO MENO DI QUINDICI STANZE!");
    }
    
}

static void FinisciCreazione(void){

    if (StanzeDellaPauraEDellaFameCreate < 15 || nGiocatori == 0)
    {
        printROSSO("\tATTENZIONE IL GIOCO NON E' STATO IMPOSTATO CORRETTAMENTE. SI PREGA DI IMPOSTARTLO IN MODO CORRETTO\n");
    } else {
        printVERDE("\tIL GIOCO E' STATO IMPOSTATO IN MANIERA CORRETTA!\n");
        settato = true;
    }
}



static void ImpostazioneMappa(void){
    int DecisioneGM = 0;
    do
    {   
        printf("╔═══════════════════════════════════╗\n");
        printf("║Cosa vuole fare il Dungeon-Master? ║\n");
        printf("║ --------------------------------- ║\n");
        printf("║1 - Generare il Dungeon            ║\n");
        printf("║2 - Inserire una segreta           ║\n");
        printf("║3 - Sradicare una segreta          ║\n");
        printf("║4 - Controllare il Dungeon         ║\n");
        printf("║5 - Chiudere la creazione          ║\n");
        printf("╚═══════════════════════════════════╝\n");
        scanf("%d", &DecisioneGM);

        //Per generare il DUngeon posso fare un loop di Inserisci zona per 15 volte, dopo vedere se aggiungere altre 15 o sovrascrivere le altre

        switch (DecisioneGM)
        {
        case 1:
            printVERDE("\tI muri si stanno spostando\n");
            CreaDungeon();
            break;
        case 2:
            InserisciSegreta();
            break;
        case 3:
            SradicaSegreta();
            break;
        case 4:
            ControllaDungeon();
            break;
        case 5:
        
        if (StanzeDellaPauraEDellaFameCreate < 15)
        {
            printROSSO("\t\n\nIL DUNGEON NON E' STATO CREATO CORRETTAMENTE\n");
        } else {
            printVERDE("\t\n\nIL DUNGEON E' STATO CREATO CORRETTAMENTE\n");
            FinisciCreazione();
        }
            break;
        
        default:
            break;
        }
    } while (DecisioneGM != 5);
    

}

void imposta_gioco(){
    int SceltaImposta = 0;

    printVERDE("Eroe scegli cosa vuoi impostare!\n");

    //idea sarebbe di chiedere e dopo fare una cosa simile al menu principale quindi un do while a scelta
    //il menu stampera Imposta mappa - Stampa mappa - Ritorna al menu di scelta di Imposta - Gioca - Termina

    do
    {
        /*Menu e Art sono fatte con REXPaint v1.70 https://www.gridsagegames.com/rexpaint/*/
        printf("╔══════════════════════════╗\n");
        printf("║1-Imposta Giocatori       ║\n");
        printf("║2-Imposta Mappa           ║\n");
        printf("║3-Stampa mappa            ║\n");
        printf("║4-Torna la menu principale║\n");
        printf("╚══════════════════════════╝\n");

        scanf("%d", &SceltaImposta);
        switch (SceltaImposta)
        {
        case 1:
            if (nGiocatori != 0)
            {
             printROSSO("I GIOCATORI SONO GIA' STATI IMPOSTATI!\n\n");   
            } else {
                GenGiocatori();
            }
            break;
        case 2:
            ImpostazioneMappa();
            break;
        case 3:
            ControllaDungeon();
            break;
        case 4:
            system("clear");
            logo();
            FinisciCreazione();
            break;
        
        default:
            break;
        }  
    } while (!settato);
}

static void Avanza(struct Giocatore* temp){

    if (temp->posizione->porta == 2)
    {
        printROSSO("LA PORTA E' CHIUSA! DOVRAI SCASSINARLA\n");
        //temp->posizione = temp->posizione->zona_successiva;   Usata in fase di debug
        
    } else if (ACaso(100) <= 33){
        printROSSO("A MONSTER APPEARS!\n\n");
        combattimento = true;
        AMonsterAppears();
        temp->movimento--;
    } else {
        temp->posizione = temp->posizione->zona_successiva;
        temp->movimento--;
        printVERDE("Sei andato nella stanza successiva\n");
        if (temp->posizione == pLast) {
            printf("Il giocatore %s e' arrivato nell'ultima stanza.\n", temp->nome_giocatore);
            printROSSO("Dal buio della segreta emerge l'ultimo nemico ...\n");
            combattimento = true;
            AMonsterAppears();
        }
    }
}

static void Indietreggia(struct Giocatore* temp){
    
    if (temp->posizione == pFirst){    //controllo se mi trovo nella prima zona
        printROSSO("Non puoi uscire ne' dal Dungeon ne' dai confini della realta'! (memoria di gioco)\n");
    } else {
        temp->posizione = temp->posizione->zona_precedente;
        temp->posizione->tesoro = ACaso(4)+1;
    }

    if (ACaso(100) <= 33) {
        combattimento = true;
        AMonsterAppears();
    }
}

static void ApriPorta(struct Giocatore* g){
    int d6 = ACaso(6)+1;
    if (g->posizione->porta == 2) {
        
        if (d6 <= g->mente) {
            printVERDE("Hai scassinato la porta e sei avanzato!\n");
            g->posizione = g->posizione->zona_successiva;
            g->movimento--;
        } 
        else 
        /*Inizia la cascata per vedere l'outcome. Avevo provato a farlo con degli switch, ma non riuscivo*/
        if (ACaso(100) < 11) {
            printVERDE("Mentre cercavi di scassinare la porta hai attivato il sigillo di sicurezza sulla porta e sei statao teletrasportato all'inizio del Dungeon!\n");
            g->posizione = pFirst;
       } else if (ACaso(100) < 51) {
            printVERDE("Ti sei ferito mentre tentavi di scassinare la porta\nPerdi un punto vita\n");
            g->PV--;
            if (g->PV == 0)
            {
                Deceduti++;
                printROSSO("Sei morto mentre ... cercavi di scassinare un porta? Skill issue\n");
                printROSSO("Gli spazzini stanno portando via il tuo corpo\n");
                free(g);
                g = NULL;
            }
            
       }else if (ACaso(100) < 41) {
            printVERDE("Hai fatto troppo rumore ed hai attirato l'attenzione sul gruppo!\nPreparati a difenderti!\n");
            combattimento = true;
            AMonsterAppears();
       }
    }else {
    printVERDE("La porta e' gia' aperta\n");
    }
}
    /* FUNZIONI PER LA STAMPA DI INFO */

static void StampaGiocatore(struct Giocatore* g){
    char *classnames[all_class] = {"barbaro", "nano", "elfo", "mago"};
    printf("Nome dell'eroe: %s\n", g->nome_giocatore);
    printf("Classe: %s\n", classnames[g->classe]);
    printf("Punti vita: %d\n", g->PV);
    printf("Potere d'attacco: %d\n", g->dadi_attacco);
    printf("CA: %d\n", g->dadi_difesa);
    printf("Mente: %d\n", g->mente);
    printf("Potere speciale: %d\n", g->potere_speciale);
}

static void CheckTesoro(struct Giocatore* p){
    if (p->posizione->tesoro != nessun_tesoro){
        printf("C'e' un tesoro in questa stanza\n");
        return;
    }
    printf("Non c'e' nulla in questa stanza!\n");
    return;
    
}

static void CheckPorta(struct Giocatore* p){
    if (p->posizione->porta != nessuna_porta){
        printf("C'e' una porta\n");
        return;
    }
    printf("La stanza non ha porte\n");
    return;
}

static void StampaStanza(struct Giocatore* temp){
    char *zonenames[all_zones] = {"corridoio", "scala", "sala_banchetto", "magazzino", "giardino", "posto_guardia", "prigione", "cucina", "armeria", "tempio"};
    printf("Stanza attuale: " "%s\n", zonenames[temp->posizione->zona]);
    CheckTesoro(temp);
    CheckPorta(temp);
}

static void PrendiTesoro(struct Giocatore* g){
    struct Zona_segrete* diComodo = g->posizione;
    
    if (diComodo->tesoro == nessun_tesoro) {
        printf("Un mostro ha gia' portato via tutto quello c'era qua dentro\n");
    } else if (diComodo->tesoro == veleno) {
        printROSSO("Il tesoro che hai raccolto era intriso di veleno che brucia al contatto\n");
        printf("Perdi 2 PV\n");
        g-> PV -= 2;
    } else if (diComodo->tesoro == guarigione) {
        printVERDE("Hai trovato delle scorte mediche di uno sfortunato avventuriero\n");
        printf("Guadagni 1 PV\n");
        g-> PV += 1;
    } else if (diComodo->tesoro == doppia_guarigione) {
        printVERDE("Hai trovato delle scorte mediche di ottima qualita'\n");
        printf("Guadagni 2 PV\n");
        g-> PV += 2;
    }
}

static void PassaTurno(void) {

}

static void Scappa(struct Giocatore* g, struct Abitante* a){
    int RollScappa = 0;
    int ccColpoAlleSpalle = 0;
    int ccParataSCappa = 0;
    int hpPersiScappa = 0;

    int d6 = ACaso(6)+1;

    if (g->posizione == pLast)
    {
        printROSSO("NON SI SCAPPA!\n");
    }
    
    if (d6 <= g->mente) {
        printf("Hai rollato un %d", d6);
        printVERDE("Sei scappato!\n");
        g->posizione = g->posizione->zona_precedente;
        combattimento = false;
        free(a);
        a = NULL;
    }
    else {
        printROSSO("Non sei riuscito a scappare! L'abitante attacca!\n");
        
        for (int i = 0; i < a->dadi_attacco; i++)
           {
                RollScappa = ACaso(6)+1;
                if (RollScappa <= 3)
                    ccColpoAlleSpalle++;
            }

            if (ccColpoAlleSpalle == 0)
            {
                printROSSO("\nMancato miracolosamente!\n");
                return;
            } else {
                for (int i = 0; i < g->dadi_difesa/2; i++)
                {
                    RollScappa = ACaso(6)+1;
                    if (RollScappa <= 2)
                        ccParataSCappa++;
                }
                hpPersiScappa = ccColpoAlleSpalle - ccParataSCappa;
                if (hpPersiScappa <= 0)
                {
                    printVERDE("Mancato miracolosamente!\n");
                    return;
                }
                printf("\nPerdi %d HP", hpPersiScappa);
                for (int i = 0; i < hpPersiScappa; i++)
                {
                    g->PV--;
                    if (g->PV == 0)
                    {
                        Deceduti++;
                        printROSSO("SEI MORTO MENTRE CERCAVI DI SCAPPARE\n");
                        printROSSO("Gli spazzini stanno portando via il tuo corpo\n");
                        combattimento = false;
                        free(g);
                        g = NULL;
                        break;
                    }
                    
                }
            }
    }
}

static void GiocaPotere(struct Giocatore* g, struct Abitante* a) {
    /*controllo se uno dei due puntatori passati e' NULL*/
    if (a==NULL)
    {
        printROSSO("ERRORE!");
        return;
    }
    
    if (g->potere_speciale == 0)
    {
     printf("\nNon hai piu' cariche disponibili.\n Il mostro ride del tuo fallimento\n");
     return;
    }
    else
    {
        g->potere_speciale -= 1;
        free(a);
        a = NULL;
        printROSSO("\nL'Abitante e' stato cancellato dall'esistenza!\n");
        combattimento = false;
        return;
    }
    
}


static void Combatti(struct Giocatore* g, struct Abitante* a){


    //int sceltaPotere = 0;
    int rollata = 0;
    int ccTeschi = 0;
    int ccScudoNero = 0;
    int ccScudoBianco = 0;
    int hpPersi = 0;

    /*printf("\n Usare potere speciale per uccidere il mostro?\n Premi un numero qualsiasi per usarlo\n Altrimenti premi 0 per non farlo\n");
    scanf("%d", &sceltaPotere);
    if (sceltaPotere == 1)
    {
        GiocaPotere(g,a);
    }*/
        
    while (g != NULL && a != NULL)
    {
        int initPlayer = ACaso(6)+1;
        int initAbitante = ACaso(6)+1;

        printf("╔════════════════════════════════╗\n");
        printf("║ Hai rollato %d per l'iniziativa ║\n", initPlayer);
        printf("║ L'abitante ha rollato %d        ║\n", initAbitante);
        printf("╚════════════════════════════════╝\n");

        if (initPlayer >= initAbitante)
        {
           for (int i = 0; i < g->dadi_attacco; i++)
           {
                rollata = ACaso(6)+1;
                if (rollata <= 3){
                    printf("Hai rollato");
                    ccTeschi++;
                }
            }

            if (ccTeschi == 0)
            {
                printROSSO("\nNon sei riuscito a mettere a segno nemmeno un colpo!\n");
                return;
            } else {
                for (int i = 0; i < a->dadi_difesa; i++)
                {
                    rollata = ACaso(6)+1;
                    if (rollata == 1){
                        //printf("Qui segmenta");
                        ccScudoNero++;
                    }
                }
                hpPersi = ccTeschi - ccScudoNero;
                if (hpPersi <= 0)
                {
                    printROSSO("L'abitante è riuscito a parare tutti i colpi inferti\n");
                    return;
                }
                
                printf("\nL'abitante perde %d HP\n", hpPersi);
                for (int i = 0; i < hpPersi; i++)
                {
                    a->PV--;
                    if (a->PV == 0 && g->posizione == pLast) 
                    {
                        printVERDE("Hai superato l'ultimo ostacolo che ti separava dalla vittoria!\nHAI VINTO\n\n");
                        combattimento = false;
                        termina();
                        break;    
                    } else if (a->PV == 0)
                    {
                        printVERDE("Hai ucciso l'abitante\n");
                        combattimento = false;
                        free(a);
                        a = NULL;
                        break;
                    }
                }
                
                //a->PV -= hpPersi;

                /*if (a->PV == 0 && g->posizione == pLast)
                {
                    printVERDE("Sei riuscito ad uccidere l'ultimo ostacolo che ti separava dalla vittoria!\n\tHAI VINTO, SEI TE LA SCALOGNAQUEST!\n");
                    //system("clear");
                    termina();
                }
                else if(a->PV == 0)
                {
                    printVERDE("Hai ucciso l'abitante!\n");
                    combattimento = false;
                    free(a);
                    a = NULL;
                    //printf("Caso mio");
                    //fflush(stdout);
                    //system("clear");
                }*/
                return;
                
            }
        } else {
           for (int i = 0; i < a->dadi_attacco; i++)
           {
                rollata = ACaso(6)+1;
                if (rollata <= 3){
                    //printf("Qui segmenta");
                    ccTeschi++;
                }
            }

            if (ccTeschi == 0)
            {
                printVERDE("\nMancato miracolosamente!\n");
                return;
            } else {
                for (int i = 0; i < g->dadi_difesa; i++)
                {
                    rollata = ACaso(6)+1;
                    if (rollata <= 2) {
                        //printf("Qui segmenta");
                        ccScudoBianco++;
                    }
                }

                hpPersi = ccTeschi - ccScudoBianco;
                if (hpPersi <= 0)
                {
                    printVERDE("Sei riuscito a parare tutti i colpi dell'abitante\n");
                    return;
                }
                printf("\nPerdi %d HP\n", hpPersi);
                for (int i = 0; i < hpPersi; i++)
                {
                    g->PV--;
                    if (g->PV == 0)
                    {
                        Deceduti++;
                        printROSSO("Sei morto!\n");
                        printROSSO("In attessa che gli spazzini portino via il tuo corpo\n");
                        SiVaALetto(2);
                        //system("clear");
                        combattimento = false;
                        free(g);
                        g = NULL;
                        break;
                    }
                    
                }
                return;
                
                
                /*g->PV -= hpPersi;
                if (g->PV <= 0)
                {
                    Deceduti++;
                    printROSSO("Sei morto!\n");
                    printROSSO("In attessa che gli spazzini portino via il tuo corpo\n");
                    SiVaALetto(2);
                    //system("clear");
                    combattimento = false;
                    free(g);
                    g = NULL;
                    //printf("Print fatto");
                    //fflush(stdout);
                }*/
                
            }
        }
    }
}



//Aggiungere funzione in più di gioco che dice che sulle zone rimanenti della mappa da dove si trova il giocatore che la chiama
//c'è come tesoro un veleno oppure no

static void SpoilerTes(struct Giocatore* g){
    struct Zona_segrete* pScan =  g->posizione; //è quella attuale
    int tes = 0;

    while (pScan != NULL && pScan->zona_successiva != NULL)
    {
        if (pScan->tesoro == 1)
        {
            tes +=1;
        }
        
        pScan = pScan->zona_successiva;
            
    }

    if (tes)
    {
        printf("Almeno un veleno te lo becchi\n");
    } else {
        printf("Non ci sono veleni nelle zone successive. Ottimo seed\n");
    }
    
    
}

void gioca(){ //SECONDA VERSIONE

    if(!settato)
    {
      printROSSO("IL GIOCO NON E' STATO IMPOSTATO IN MANIERA CORRETTA!\n\nCONTROLLARE MAPPA O GIOCATORI!\n");
      return;  
    }
    
    int esplorazione = 0;
    //TUTTO IL NECESSARIO PER STABILIRE UN ORDINE INIZIALE DEI GIOCATORI E RANDOMIZZARLO
    int OrdineIniziale [(nGiocatori)];
    

    //printf("AAAAAA");
    //fflush(stdout);
    for (int i = 0; i < nGiocatori; i++) //SCORRO E VADO AVANTI COI PUNTATORI
    {
        OrdineIniziale[i] = i;
        arraygiocatori[i] ->posizione = pFirst;
    }
    do
    {
       RandOrdine(OrdineIniziale, nGiocatori); 
       for (int i = 0; i < nGiocatori; i++) {
        
        // ES OrdineIiniziale -> turnazione giocatori (3, 1, 2, 0)
        //STEP 0 il 3
        //STEP 1 il 1
        //STEP 2 il 2
        //STEP 3 il 0
        struct Giocatore* playeratt = arraygiocatori[OrdineIniziale[i]];

            if (playeratt-> PV !=0) {   //CONTROLLO SE IL GIOCATORE ATTUALE E' VIVO, FORSE E' RIDONDANTE MA MI SENTIVO PIU' SICURO IN QUESTO MODO
                //DOPO UN PAIO DI PARTITE VEDEVO CHE NON SI STAVA RIAGGIORNANDO IL VALORE DI MOVIMENTO
                playeratt->movimento = 1;
                
                do
                {
                printf("Cosa fara' %s?\n", playeratt->nome_giocatore);   //STAMPO IL NOME, CAPIRE DI CHI E' IL TURNO
                printVERDE("╔═══════════════════════╗\n");
                printVERDE("║ 1-Avanza nel dungeon  ║\n");
                printVERDE("║ 2-Torna indietro      ║\n");
                printVERDE("║ 3-Controllati         ║\n");
                printVERDE("║ 4-Controlla la stanza ║\n");
                printVERDE("║ 5-Apri la porta       ║\n");
                printVERDE("║ 6-Prendere il bottino ║\n");
                printVERDE("║ 7-Controlla veleno    ║\n");
                printVERDE("║ 8-Passa/Combatti      ║\n");
                printVERDE("╚═══════════════════════╝");
                scanf("%d", &esplorazione);
                    switch (esplorazione)
                    {
                    case 1:
                        system("clear");
                        if (playeratt->movimento > 0 && playeratt->posizione->zona_successiva != NULL) {
                            Avanza(playeratt);
                        } else
                        printROSSO("Ti sei gia' spostato in questo turno!\n");
                        break;
                    case 2:
                        system("clear");
                        Indietreggia(playeratt);
                        break;
                    case 3:
                        system("clear");
                        StampaGiocatore(playeratt);
                        break;
                    case 4:
                        system("clear");
                        StampaStanza(playeratt);
                        break;
                    case 5:
                        system("clear");
                        ApriPorta(playeratt);
                        break;
                    case 6:
                        system("clear");
                        PrendiTesoro(playeratt);
                       break;
                    case 7:
                        system("clear");
                        SpoilerTes(playeratt);
                        break;
                    case 8:
                        PassaTurno();
                        break;
                    
                    default:
                        printROSSO("Comando immesso non esistente\n");
                        break;
                    }   
                } while (esplorazione != 7);    //PASSATURNO E' UNA FUNZIONE VUOTA. FACENDO TUTTO CON DO WHILE MI BASTA PIGIARE IL 7 PER USCIRE DAL CICLO
                
                //VOLENDO SEPARARE I MENU DI COMBATTIMENTO ED ESPLORAZIONE COME IN WIZARDRY CONTROLLO SE MI TROVO IN COMBATTIMENTO
                while (combattimento) {    
                    struct Abitante* a = AbitantePtr;

                    printVERDE("╔═══════════════════════╗\n");
                    printVERDE("║ 1-Fuggi!              ║\n");
                    printVERDE("║ 2-Combatti!           ║\n");
                    printVERDE("║ 3-Attiva il potere    ║\n");
                    printVERDE("║ 4-Status Giocatore    ║\n");
                    printVERDE("╚═══════════════════════╝\n");
                    scanf("%d", &esplorazione);
                        switch (esplorazione) {
                        case 1:
                            system("clear");
                            Scappa(playeratt, a);
                            break;
                        case 2:
                            system("clear");
                            Combatti(playeratt,a);
                            //printf("QUI");
                            //fflush(stdout);
                            break;
                        case 3:
                            system("clear");
                            GiocaPotere(playeratt, a);
                            break;
                        case 4:
                            system("clear");
                            StampaGiocatore(playeratt);
                            break;
                        default:
                            break;
                        }
                }
            }
            
       }
       
    } while (nGiocatori > Deceduti);
    if (Deceduti == nGiocatori) {   
        printROSSO("GAME OVER!\n\n");
        termina();
    }
    
}

void termina() {

    logo();
    printVERDE("Grazie per aver giocato a ScalognaQuest (un gioco fatto non in Java)\nSpero che vi siate divertiti!\n");
    printROSSO("Sto liberando memoria, ci vorra' solo un attimo\n");
    //free(arraygiocatori);
    nGiocatori = 0;
    Deceduti = 0;
    free(AbitantePtr);
    AbitantePtr = NULL;
    //LOOP PER DEALLOCARE LA MEMORIA DI TUTTE LE STANZE
    while (pFirst != NULL)
        {
            struct Zona_segrete* t = pFirst;
            pFirst = pFirst->zona_successiva;
            free(t);
        }
    StanzeDellaPauraEDellaFameCreate = 0;
    pFirst = NULL;
    pLast = NULL;
    settato = false;

    SiVaALetto(3);
    printROSSO("Finito!\n");
    SiVaALetto(2);
    system("clear");
}
