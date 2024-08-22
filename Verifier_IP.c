#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Fonction qui vérifie si l'adresse IP est valide
int isValidIP(char *ip) {
    int num;          // Variable pour stocker chaque segment de l'adresse IP
    int dots = 0;     // Compteur pour le nombre de points (".") dans l'adresse IP
    char *ptr;        // Pointeur pour parcourir chaque segment de l'adresse IP

    if (ip == NULL)   // Vérifie si l'adresse IP est NULL (non initialisée)
        return 0;

    // Compte le nombre de points dans l'adresse IP
    for (int i = 0; ip[i]; i++) {
        if (ip[i] == '.')
            dots++;
    }

    // L'adresse IP doit avoir exactement 3 points pour être valide
    if (dots != 3)
        return 0;

    // Découpe l'adresse IP en segments séparés par des points
    ptr = strtok(ip, ".");

    while (ptr) {
        // Vérifie si chaque segment contient uniquement des chiffres
        if (!isdigit(*ptr))
            return 0;

        num = atoi(ptr);  // Convertit le segment en entier

        // Chaque segment de l'adresse IP doit être compris entre 0 et 255
        if (num < 0 || num > 255)
            return 0;

        ptr = strtok(NULL, ".");  // Passe au segment suivant
    }

    return 1;  // L'adresse IP est valide
}

// Fonction qui détermine la classe de l'adresse IP en fonction du premier octet
char getIPClass(int firstOctet) {
    if (firstOctet >= 0 && firstOctet <= 127)
        return 'A';  // Classe A
    else if (firstOctet >= 128 && firstOctet <= 191)
        return 'B';  // Classe B
    else if (firstOctet >= 192 && firstOctet <= 223)
        return 'C';  // Classe C
    else if (firstOctet >= 224 && firstOctet <= 239)
        return 'D';  // Classe D
    else
        return 'E';  // Classe E
}

// Fonction pour extraire les données du formulaire CGI
void extractFormData(char *data, char *ip) {
    char *key;     // Pointeur pour le nom du champ (clé)
    char *value;   // Pointeur pour la valeur du champ

    // Découpe la chaîne de données en clé/valeur (ex: "ip=192.168.1.1")
    key = strtok(data, "=");       // Récupère le nom du champ (clé)
    value = strtok(NULL, "=");     // Récupère la valeur du champ

    if (strcmp(key, "ip") == 0) {  // Si le champ est "ip"
        strncpy(ip, value, 15);    // Copier la valeur de l'IP dans la variable ip
        ip[15] = '\0';             // Assure la terminaison correcte de la chaîne
    }
}

int main() {
    char *data;          // Pointeur pour stocker les données du formulaire (query string)
    char ip[16];         // Tableau pour stocker l'adresse IP extraite
    char ipCopy[16];     // Copie de l'adresse IP pour éviter de modifier l'originale
    int firstOctet;      // Variable pour stocker le premier octet de l'adresse IP

    // Déclaration du type de contenu pour une page web (en-tête HTTP)
    printf("Content-Type: text/html\n\n");

    // Récupération des données du formulaire à partir de la query string
    data = getenv("QUERY_STRING");

    if (data == NULL) {  // Vérifie si aucune donnée n'a été reçue
        printf("Aucune donnée reçue.\n");
        return 1;  // Termine le programme si aucune donnée n'est reçue
    }

    // Extraction de l'adresse IP des données du formulaire
    extractFormData(data, ip);

    // Créer une copie de l'IP pour éviter la modification par strtok
    strncpy(ipCopy, ip, sizeof(ipCopy));

    // Vérifie si l'adresse IP est valide
    if (!isValidIP(ipCopy)) {
		printf ("<html> <style> h2{ color:red } </style>");
        printf("<body> <h2>Adresse IP invalide.</h2> </body> </html");
        return 1;  // Termine le programme si l'IP est invalide
    }

    // Réinitialiser ipCopy pour extraire le premier octet
    strncpy(ipCopy, ip, sizeof(ipCopy));
    firstOctet = atoi(strtok(ipCopy, "."));  // Convertit le premier segment en entier

    // Détermine la classe de l'adresse IP
    char ipClass = getIPClass(firstOctet);
    printf ("<html> <style> h2{ color:red }</style>");
    printf("<body> <h2>La classe de l'adresse IP %s est: %c</h2> </body </html", ip, ipClass);

    return 0;  // Le programme s'est exécuté avec succès
}
