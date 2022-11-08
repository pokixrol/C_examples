#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define POCET 50
#define VELIKOST 250
#define VSTUP "zavodnici.txt"
#define VYSTUP "vysledkova_listina.txt"
#define ODDELOVACE ":."

typedef struct {
    int cislo;
    char prijmeni[POCET];
    char jmeno[POCET];
    char cas1[POCET];
    char cas2[POCET];
} ZAVODNIK;

int nactiSoubor(ZAVODNIK *z) {
    int i = 0;
    char buffer[VELIKOST];
    FILE *file = fopen(VSTUP, "r");
    if (file == NULL) {
        printf("Soubor %s nebyl otevren\n", VSTUP);
    }else {
        if (fgets(buffer, VELIKOST, file) != NULL) {
            while (fscanf(file, "%d %s %s %s %s", &z[i].cislo, z[i].prijmeni, z[i].jmeno,
                          z[i].cas1, z[i].cas2) != EOF) {
                i++;
            }
        }
        if (fclose(file) == EOF) {
            printf("Soubor %s nebyl spravne uzavren\n", VSTUP);
        }
    }
    return i;
}

void vypisListiny(ZAVODNIK *z, int pocet) {
	int i;
    printf("	S T A R T O V N I  L I S T I N A  -  S L A L O M  Z E N Y\n");
    printf("------------------------------------------------------------------------\n");
    printf("startovni cislo |    prijmeni  |   jmeno   | cas 1. kola | cas 2. kola |\n");
    printf("------------------------------------------------------------------------\n");
    for (i = 0; i < pocet; i++) {
        printf("%16d| %13s| %10s|   %10s|   %10s|\n", z[i].cislo, z[i].prijmeni, z[i].jmeno,
               z[i].cas1, z[i].cas2);
    }
    printf("------------------------------------------------------------------------\n");
}


int casNaSetiny(const char *cas){
    char pomoc[VELIKOST];
    strcpy(pomoc, cas);
    int celkem = 0;
    int castCyklu = 1;
    char *pch = NULL;
    pch = strtok(pomoc, ODDELOVACE);
    while(pch!=NULL){
        switch (castCyklu) {
            case 1:
                celkem += atoi(pch)*6000;
                break;
            case 2:
                celkem += atoi(pch)*100;
                break;
            case 3:
                celkem += atoi(pch);
        }
        castCyklu++;
        pch = strtok(NULL, ODDELOVACE);
    }
    return celkem;
}

void serad(ZAVODNIK *z, int pocet){
    ZAVODNIK pomoc;
    int i;
    int j;
    for(i = 0; i < pocet; i++){
        for(j = 0; j < pocet-i-1; j++){
            if((casNaSetiny(z[j].cas1) + casNaSetiny(z[j].cas2)) > (casNaSetiny(z[j+1].cas1) + casNaSetiny(z[j+1].cas2))){
                pomoc = z[j];
                z[j] = z[j+1];
                z[j+1] = pomoc;
            }
        }
    }
}

char *setinyNaCas(int setiny){
    char *cas = (char*) malloc(20 * sizeof(char));
    int m = setiny / 6000;
    int sek = (setiny % 6000) / 100;
    int set = setiny % 100;
    sprintf(cas,"%02d:%02d.%02d", m, sek, set);
    return cas;
}
void vypisDoSouboru(ZAVODNIK *z, int pocet){
	int i;
    FILE *fp= fopen(VYSTUP,"w");
    if(fp == NULL){
        printf("Soubor %s nebyl otevren.\n",VYSTUP);
    }
     serad(z, pocet);
    fprintf(fp,"                 		V Y S L E D K O V A     L I S T I N A \n");
    fprintf(fp,"---------------------------------------------------------------------------------------------------\n");
    fprintf(fp,"poradi | cislo |    prijmeni   |   jmeno   | cas 1. kola | cas 2. kola |   celkem   |   ztrata    |\n");
    fprintf(fp,"---------------------------------------------------------------------------------------------------\n");
    for(i = 0; i < pocet; i++){
        fprintf(fp,"%6d | %5d | %13s | %9s | %11s | %11s | %10s |  %s%9s |\n",i+1, z[i].cislo,
               z[i].prijmeni, z[i].jmeno, z[i].cas1, z[i].cas2,
               setinyNaCas(casNaSetiny(z[i].cas1) + casNaSetiny(z[i].cas2)), i==0 ? " " : "+" , i==0 ? "" : 
			   setinyNaCas((casNaSetiny(z[i].cas1) + casNaSetiny(z[i].cas2)) - (casNaSetiny(z[0].cas1) + casNaSetiny(z[0].cas2))));
    }
 	fprintf(fp,"---------------------------------------------------------------------------------------------------\n");
    free(z);

    if(fclose(fp)==EOF){
        printf("Soubor %s nebyl uspesne uzavren.\n",VYSTUP);
    }
    else{
        printf("Soubor %s byl vytvoren.\n", VYSTUP);
    }
}
int main() {
	
    ZAVODNIK zavodnici[POCET];
    int pocetZavodniku = nactiSoubor(zavodnici);
    vypisListiny(zavodnici, pocetZavodniku);
    vypisDoSouboru(zavodnici, pocetZavodniku);
   
    return 0;
}

