#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define VSTUP "beh.txt"
#define VYSTUP "vysledkova_listina.txt"
#define VELIKOST 300
#define DELKA 100
#define ODDELOVACE ";\n"
#define ODDELOVACE_CAS ":"
#define ODDELOVACE_ROK "."

typedef struct {
    int cislo;
    char prijmeni[DELKA];
    char jmeno[DELKA];
    char narozen[DELKA];
    char narodnost[DELKA];
    char klub[DELKA];
    char cas[DELKA];
}BEZCI;

int zjistiRocnik(const char *datum){
    int castData = 1;
    char pomoc[DELKA] = "";
    strcpy(pomoc, datum);
    char *pch = NULL;
    pch = strtok(pomoc, ODDELOVACE_ROK);
    while (pch!=NULL){
        if(castData == 3){
            return atoi(pch);
        }
        castData++;
        pch = strtok(NULL, ODDELOVACE_ROK);
    }
    return 0;
}

int vek(int rocnik){
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    return(tm->tm_year + 1900 - rocnik);
}

int casNaSekundy(const char *cas){
    char pomoc[VELIKOST];
    strcpy(pomoc, cas);
    int celkem = 0;
    int kolo = 1;
    char *pch = NULL;
    pch = strtok(pomoc, ODDELOVACE_CAS);
    while (pch!=NULL){
        switch (kolo) {
            case 1:
                celkem += atoi(pch) * 3600;
                break;
            case 2:
                celkem += atoi(pch) * 60;
                break;
            case 3:
                celkem += atoi(pch);
                break;
        }
        kolo++;
        pch = strtok(NULL, ODDELOVACE_CAS);
    }
    return celkem;
}

char *sekundyNaCas(int sekundy){
    char *cas = (char*) malloc(20*sizeof(char));
    int h = sekundy / 3600;
    int m = (sekundy % 3600) / 60;
    int s = sekundy % 60;
    sprintf(cas, "+%d:%02d:%02d", h, m, s);
    return cas;
}

void seradit(BEZCI *zavodnici, int pocet){
    BEZCI pomoc;
    int i;
    int j;
    for(i = 0; i < pocet; i++){
        for(j = 0; j < pocet-i-1; j++){
            if(casNaSekundy(zavodnici[j].cas) > casNaSekundy(zavodnici[j+1].cas)){
                pomoc = zavodnici[j];
                zavodnici[j] = zavodnici[j+1];
                zavodnici[j+1] = pomoc;
            }
        }
    }
    return;
}

int main() {
    char buffer[VELIKOST];
    char *pch = NULL;
    FILE *soubor;
    BEZCI *b = NULL;
    int pocet = 0;
    int kolo;
    int indexN = 0;
    int cesi = 0;
    int i;
    soubor = fopen(VSTUP, "r");
    if(soubor == NULL){
        printf("Soubor %s se nepodarilo otevrit.\n",VSTUP);
        return(EXIT_FAILURE);
    }

    while ((fgets(buffer, VELIKOST, soubor))!=NULL){
        if(pocet!=0){
            b = (BEZCI*) realloc(b, pocet * sizeof(BEZCI));
            if(b != NULL){
                kolo = 1;
                pch = strtok(buffer, ODDELOVACE);
                while (pch != NULL){
                    switch (kolo) {
                        case 1:
                            b[pocet-1].cislo = atoi(pch);
                            break;
                        case 2:
                            strcpy(b[pocet-1].prijmeni, pch);
                            break;
                        case 3:
                            strcpy(b[pocet-1].jmeno, pch);
                            break;
                        case 4:
                            strcpy(b[pocet-1].narozen, pch);
                            break;
                        case 5:
                            strcpy(b[pocet-1].narodnost, pch);
                            break;
                        case 6:
                            strcpy(b[pocet-1].klub, pch);
                            break;
                        case 7:
                            strcpy(b[pocet-1].cas, pch);
                            break;
                    }
                    pch = strtok(NULL, ODDELOVACE);
                    kolo++;
                }
            } 
        }
        pocet++;
    }

    if(fclose(soubor)==EOF){
        printf("Soubor %s se nepodarilo uzavrit.\n",VSTUP);
    }

    printf("					S T A R T O V N I    L I S T I N A  -  M A R A T O N\n");
    printf("------------------------------------------------------------------------------------------------------\n");
    printf("Cislo |  Prijmeni  |    Jmeno    |  Narozen   | Stat |                Klub                 |   Cas   |\n");
    printf("------------------------------------------------------------------------------------------------------\n");
    for(i = 0; i < pocet-1; i++){

        if(vek(zjistiRocnik(b[i].narozen)) > vek(zjistiRocnik(b[indexN].narozen))){
            indexN = i;
        }

        printf(" %4d | %10s | %11s | %10s |  %3s | %35s | %7s |\n", b[i].cislo, b[i].prijmeni, b[i].jmeno,
                                    b[i].narozen, b[i].narodnost, b[i].klub, b[i].cas);
        if(strcmp("CZE", b[i].narodnost) == 0){
            cesi++;
        }

    }
    printf("------------------------------------------------------------------------------------------------------\n");
    printf("Celkovy pocet zavodniku je %d.\n", pocet - 1);
    printf("Cechu je %d.\n", cesi);
    printf("Nejstarsim zavodnikem je %s %s narozen v roce %d. Je mu %d let.\n", b[indexN].prijmeni,
           b[indexN].jmeno, zjistiRocnik(b[indexN].narozen), vek(zjistiRocnik(b[indexN].narozen)));
    printf("\n");

    seradit(b, pocet-1);

    soubor = fopen(VYSTUP,"w");
    if(soubor == NULL){
        printf("Soubor %s se nepodarilo otevrit.\n",VYSTUP);
        return(EXIT_FAILURE);
    }

    fprintf(soubor,"							V Y S L E D K O V A  L I S T I N A  -  M A R A T O N\n");
    fprintf(soubor,"--------------------------------------------------------------------------------------------------------------------------\n");
    fprintf(soubor,"Poradi | Cislo |  Prijmeni  |    Jmeno    |  Narozen   | Stat |                Klub                 |   Cas   |  Ztrata  |\n");
        fprintf(soubor,"--------------------------------------------------------------------------------------------------------------------------\n");
    for(i = 0; i < pocet - 1; i++){
        pch = sekundyNaCas((casNaSekundy(b[i].cas)) - (casNaSekundy(b[0].cas)));

        fprintf(soubor, "%6d | %5d | %10s | %11s | %10s |  %3s | %35s | %7s | %8s |\n",i+1, b[i].cislo, b[i].prijmeni, b[i].jmeno,
               b[i].narozen, b[i].narodnost, b[i].klub, b[i].cas, i==0 ? "":pch);
    }
        fprintf(soubor,"--------------------------------------------------------------------------------------------------------------------------\n");
    free(pch);
    free(b);

    if(fclose(soubor)==EOF){
        printf("Soubor %s se nepodarilo uzavrit.\n",VYSTUP);
    }
    else{
        printf("Soubor %s byl vytvoren.\n", VYSTUP);
    }

    return 0;
}

