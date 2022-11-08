#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VSTUP "startovni_listina_kombinace_zeny.txt"
#define VYSTUP "vyssledkova_listina_kombinace_zeny.txt"
#define VELIKOST 250
#define CASTSTRUKTURY 99
#define ODDELOVACE " \r\n"
#define ODDELOVACE_CAS ":,"

typedef struct{
    int cislo;
    char jmeno[CASTSTRUKTURY];
    char prijmeni[CASTSTRUKTURY];
    char narodnost[CASTSTRUKTURY];
    char sjezd[CASTSTRUKTURY];
    char slalom[CASTSTRUKTURY];
}ZAVODNICE;

int casNaSetiny(const char *cas, int typ){
    char pomoc[VELIKOST];
    strcpy(pomoc, cas);
    int celkem = 0;
    int castCyklu = 1;
    char *pch = NULL;
    pch = strtok(pomoc, ODDELOVACE_CAS);
    if(typ == 1) {
        while (pch != NULL) {
            switch (castCyklu) {
                case 1:
                    celkem += atoi(pch) * 6000;
                    break;
                case 2:
                    celkem += atoi(pch) * 100;
                    break;
                case 3:
                    celkem += atoi(pch);
            }
            castCyklu++;
            pch = strtok(NULL, ODDELOVACE_CAS);

        }
    } else if (typ == 0) {
        while (pch != NULL) {
            switch (castCyklu) {
                case 1:
                    celkem += atoi(pch) * 100;
                    break;
                case 2:
                    celkem += atoi(pch);
            }
            castCyklu++;
            pch = strtok(NULL, ODDELOVACE_CAS);
        }
    }
    return celkem;
}

void seradZavodnice(ZAVODNICE *zavodnici, int pocet){
    ZAVODNICE pomoc;
    int i;
    int j;
    for(i = 0; i < pocet; i++){
        for(j = 0; j < pocet-i-1; j++){
            if((casNaSetiny(zavodnici[j].sjezd, 1) + casNaSetiny(zavodnici[j].slalom, 0)) > (casNaSetiny(zavodnici[j+1].sjezd, 1) + casNaSetiny(zavodnici[j+1].slalom, 0))){
                pomoc = zavodnici[j];
                zavodnici[j] = zavodnici[j+1];
                zavodnici[j+1] = pomoc;
            }
        }
    }
}

char *setinyNaCas(int setiny){
    char *cas = (char*) malloc(20 * sizeof(char));
    int m = setiny / 6000;
    int sek = (setiny % 6000) / 100;
    int set = setiny % 100;
    sprintf(cas,"%d:%02d,%02d", m, sek, set);
    return cas;
}

int main(int argc, char** argv) {
    ZAVODNICE *zavodnici = NULL;
    char radek[VELIKOST];
    FILE *fp;
    int pocetRadku = 0;
    int castCyklu;
    char *pch = NULL;
int i;
    fp = fopen(VSTUP,"r");
    if(fp == NULL){
        printf("Soubor %s nebyl otevren.\n",VSTUP);
        return 0;
    }

    while((fgets(radek, VELIKOST, fp))!=NULL){
        pocetRadku++;
            zavodnici = (ZAVODNICE*) realloc(zavodnici, pocetRadku * sizeof(ZAVODNICE));
            if(zavodnici != NULL){
                castCyklu = 1;
                pch = strtok(radek, ODDELOVACE);
                while(pch != NULL){
                    switch (castCyklu) {
                        case 1:
                            zavodnici[pocetRadku-1].cislo = atoi(pch);
                            break;
                        case 2:
                            strcpy(zavodnici[pocetRadku-1].jmeno, pch);
                            break;
                        case 3:
                            strcpy(zavodnici[pocetRadku-1].prijmeni, pch);
                            break;
                        case 4:
                            strcpy(zavodnici[pocetRadku-1].narodnost, pch);
                            break;
                        case 5:
                            strcpy(zavodnici[pocetRadku-1].sjezd, pch);
                            break;
                        case 6:
                            strcpy(zavodnici[pocetRadku-1].slalom, pch);
                            break;

                    }
                    pch = strtok(NULL, ODDELOVACE);
                    castCyklu++;
                }
            } else {
                printf("Chyba pri pridelovani pameti.\n");
                return 0;
            }
    }

    printf(" S T A R T O V N I  L I S T I N A - K O M B I N A C E  Z E N Y \n");
    printf("---------------------------------------------------------------------------------\n");
    printf("poradi |   jmeno   |    prijmeni    | stat |  sjezd  | slalom |\n");
    printf("---------------------------------------------------------------------------------\n");
    for(i = 0; i < pocetRadku; i++){
        printf(" %5d | %9s | %14s | %4s | %7s | %6s |\n", zavodnici[i].cislo,
               zavodnici[i].jmeno, zavodnici[i].prijmeni,zavodnici[i].narodnost, zavodnici[i].sjezd, zavodnici[i].slalom);
    }
    printf("---------------------------------------------------------------------------------\n");

    seradZavodnice(zavodnici, pocetRadku);

    if(fclose(fp)==EOF){
        printf("Soubor %s nebyl uspesne uzavren.\n",VSTUP);
        return(EXIT_FAILURE);
    }

    fp = fopen(VYSTUP,"w");
    if(fp == NULL){
        printf("Soubor %s nebyl otevren.\n",VYSTUP);
        return(EXIT_FAILURE);
    }

    fprintf(fp,"                    V Y S L E D K O V A     L I S T I N A \n");
    fprintf(fp,"-----------------------------------------------------------------------------------------\n");
    fprintf(fp,"poradi |   jmeno   |    prijmeni    | stat |  sjezd  | slalom |  celkem  |  ztrata  |\n");
    fprintf(fp,"-----------------------------------------------------------------------------------------\n");
    for(i = 0; i < pocetRadku; i++){
        fprintf(fp,"%5d. | %9s | %14s | %4s | %7s | %6s | %8s | %8s |\n", i+1,
               zavodnici[i].jmeno, zavodnici[i].prijmeni,zavodnici[i].narodnost, zavodnici[i].sjezd, zavodnici[i].slalom,
               setinyNaCas(casNaSetiny(zavodnici[i].sjezd, 1)+casNaSetiny(zavodnici[i].slalom, 0)), i == 0 ? "" : setinyNaCas((casNaSetiny(zavodnici[i].sjezd, 1)+casNaSetiny(zavodnici[i].slalom, 0)) - (casNaSetiny(zavodnici[0].sjezd, 1)+casNaSetiny(zavodnici[0].slalom, 0))));
    }

    // Uvolnìní pamìti
    free(pch);
    free(zavodnici);

    if(fclose(fp)==EOF){
        printf("Soubor %s nebyl uspesne uzavren.\n",VYSTUP);
        return(EXIT_FAILURE);
    }
    else{
        printf("Soubor %s byl vytvoren.\n", VYSTUP);
    }

    return 0;
}
