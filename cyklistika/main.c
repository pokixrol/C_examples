#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define VSTUP "cyklistika.txt"
#define VYSTUP "vysledkova_listina.txt"
#define ODDELOVACE ";\n"
#define ODDELOVACE_CASU ":."
#define DELKA 50
#define VELIKOST 200

int minimum(int pole[], int pocet) {
    int min = 2000;
    int i, index;
    for (i = 0; i < pocet; i++) {
        if (pole[i] < min) {

            min = pole[i];
            index = i;
        }

    }
    return index;
}

int prevodCasu (const char *cas){
	int vysledek = 0;
	int kolo = 1;
	char pomoc[50];
	strcpy(pomoc, cas);
	char *p = strtok(pomoc, ODDELOVACE_CASU);
	while(p != NULL){
		switch(kolo){
            case 1:
                vysledek += atoi(p)*36000;
                break;
            case 2:  
                vysledek += atoi(p)*600;
                break;
            case 3:
                vysledek += atoi(p)*10;
                break;
            case 4:
            	vysledek += atoi(p);
            	break;
		}
		p = strtok(NULL, ODDELOVACE_CASU);
		kolo++;
	}
	return vysledek;
}

char *vypocetZtraty (int prevedenyCas, int ztrata){
	char *cas = (char*) malloc(sizeof(char)*50);
	int pomoc =(prevedenyCas - (prevedenyCas/36000)*36000)/600;
	sprintf(cas, "%s%02d:%02d:%02d.%d", ztrata ? "+":"" , prevedenyCas/36000, pomoc,
	(prevedenyCas -((prevedenyCas/36000)*36000) - (pomoc*600))/10, prevedenyCas%10);
	return cas;
}

int main(int argc, char *argv[]) {

    typedef struct {
        int cislo;
        char prijmeni[DELKA];
        char jmeno[DELKA];
        char narodnost[DELKA];
        int rocnik;
        char klub[DELKA];
        char cas[DELKA];
    } CYKLISTI;
    int pocetRadku = 0;
    int castRadku;
    int cz = 0;
    int sk = 0;
    int jine = 0;
    CYKLISTI *zavodnici = NULL;
    char *ztrata;
    FILE *soubor;
    soubor = fopen(VSTUP, "r");
    if (soubor == NULL) {
        printf("Soubor %s nebyl otevren.\n", VSTUP);
        return (EXIT_FAILURE);
    } else {
        //printf("Soubor %s byl uspesne otevren.\n", VSTUP);
    }
    char buffer[VELIKOST];



    char *pch;
    while (fgets(buffer, VELIKOST, soubor) != NULL) {
        if (pocetRadku != 0) {
            castRadku = 1;
            zavodnici = (CYKLISTI*) realloc(zavodnici, sizeof (CYKLISTI) * pocetRadku);
            pch = strtok(buffer, ODDELOVACE);
            while (pch != NULL) {
                switch (castRadku) {
                    case 1:
                        zavodnici[pocetRadku - 1].cislo = atoi(pch);
                        break;
                    case 2:
                        strcpy(zavodnici[pocetRadku - 1].prijmeni, pch);
                        break;
                    case 3:
                        strcpy(zavodnici[pocetRadku - 1].jmeno, pch);
                        break;
                    case 4:
                        strcpy(zavodnici[pocetRadku - 1].narodnost, pch);
                        break;
                    case 5:
                        zavodnici[pocetRadku - 1].rocnik = atoi(pch);
                        break;
                    case 6:
                        strcpy(zavodnici[pocetRadku - 1].klub, pch);
                        break;
                    case 7:
                        strcpy(zavodnici[pocetRadku - 1].cas, pch);
                        break;
                }
                pch = strtok(NULL, ODDELOVACE);
                castRadku++;
            }
        }
        pocetRadku++;
    }
    pocetRadku--;


    if (fclose(soubor) == EOF) {
        printf("Soubor %s nebyl spravne uzavren.\n", VSTUP);
    }

    int i;
    int poleRocniku[150];
    char polePuvod[150];
    printf("                                     S T A T O V N I    L I S T I N A\n");
    printf("--------------------------------------------------------------------------------------------------------------\n");
    printf(" Cislo |   Prijmeni   |   Jmeno   | Narodnost | Rocnik |                  Klub                   |    Cas    |\n");
    printf("--------------------------------------------------------------------------------------------------------------\n");
    for (i = 0; i < pocetRadku; i++) {
        poleRocniku[i] = zavodnici[i].rocnik;
        polePuvod[i] = zavodnici[i].narodnost;
        printf("%7d| %13s| %10s| %10s| %7d| %40s| %10s|\n", zavodnici[i].cislo, zavodnici[i].prijmeni
                , zavodnici[i].jmeno, zavodnici[i].narodnost, zavodnici[i].rocnik, zavodnici[i].klub
                , zavodnici[i].cas); 
        if(strcmp(zavodnici[i].narodnost, "CZE")==0){
            cz++;
        }
        else if(strcmp(zavodnici[i].narodnost, "SVK")==0){
            sk++;
        }else{
        	jine++;
		}
    }

    printf("Na startu je %d zavodniku.\n", pocetRadku);

    int poradiNejstarsiho = minimum(poleRocniku, pocetRadku);
     time_t seconds=time(NULL);
    struct tm* current_time=localtime(&seconds); 
    printf("Nejstarsim zavodnikem je %s %s narozen v roce %d. Je mu %d let.\n", zavodnici[poradiNejstarsiho].jmeno,
	 zavodnici[poradiNejstarsiho].prijmeni, zavodnici[poradiNejstarsiho].rocnik, (current_time->tm_year + 1900) - (zavodnici[poradiNejstarsiho].rocnik));

    printf("Zavodnici dle narodnosti: Cesi     -  %d\n"
        	"			  Slovaci  -  %d\n "
            "			  Jine     -  %d\n", cz, sk, jine);

	CYKLISTI pomoc;
	int j;
	for (i = 0; i < pocetRadku; i++){
        for (j = 0; j < pocetRadku-i-1; j++){
            if(prevodCasu(zavodnici[j].cas) >
              prevodCasu(zavodnici[j+1].cas)){
                pomoc = zavodnici[j];
                zavodnici[j] = zavodnici[j+1];
                zavodnici[j+1] = pomoc;
            }
        }
    }

    soubor = fopen(VYSTUP, "w");
    if (soubor == NULL) {
        printf("Soubor %s nebyl otevren.\n", VYSTUP);
        return (EXIT_FAILURE);
    }

    fprintf(soubor, "                                             V Y S L E D K O V A    L I S T I N A\n");
    fprintf(soubor, "------------------------------------------------------------------------------------------------------------------------------------\n");
    fprintf(soubor, " Poradi | Cislo |   Prijmeni   |   Jmeno   | Narodnost | Rocnik |                  Klub                   |    Cas    |   Ztrata   |\n");
    fprintf(soubor, "------------------------------------------------------------------------------------------------------------------------------------\n");
    for (i = 0; i < pocetRadku; i++) {
        fprintf(soubor, "%8d| %6d| %13s| %10s| %10s| %7d| %40s| %10s|", i + 1, zavodnici[i].cislo, zavodnici[i].prijmeni
                , zavodnici[i].jmeno, zavodnici[i].narodnost, zavodnici[i].rocnik, zavodnici[i].klub
                , zavodnici[i].cas);
        ztrata = vypocetZtraty((prevodCasu(zavodnici[i].cas)-(prevodCasu(zavodnici[0].cas))),1);
        fprintf(soubor, "%12s|\n",i==0 ? "" : ztrata);
        free(ztrata);
    }

    if (fclose(soubor) == EOF) {
        printf("Soubor %s nebyl spravne uzavren.\n", VYSTUP);
    } else {
        printf("Soubor %s byl uspesne vytvoren.\n", VYSTUP);
    }
    return 0;
}
