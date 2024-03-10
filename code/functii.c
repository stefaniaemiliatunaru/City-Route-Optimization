/* TUNARU Stefania-Emilia 311CB*/
# include "header.h"

/* functia de alocare a grafului */
TGL* AlocG(int nr) { /* aloca spatiu pentru descriptor graf si vector de (n+1) adrese liste arce */
  TGL* g = (TGL*)malloc(sizeof(TGL));
  if (!g) return NULL;
  g->x = (AArc*)calloc((nr+1), sizeof(AArc)); /* spatiu vector adrese liste arce */
  if (!g->x) {     /* daca nu reuseste alocarea vectorului, eliberez memoria ocupata de graf */
    free(g);
    return NULL;
  }
  g->n = nr;
  return g;
}

/* functia de distrugere a grafului si eliberare a memoriei ocupate de acesta */
void DistrG(TGL** ag) {
  int i = 1;
  AArc p = NULL, aux = NULL;
  for (i = 1; i <= (*ag)->n; i++) {       /* parcurg graful */
    p = (*ag)->x[i];
    while (p) {                   /* eliberez memoria ocupata de nodurile grafului */
      aux = p;
      p = p->urm;
      free(aux);
    }
  }
  free((*ag)->x);          /* eliberez vectorul de adrese */
  free(*ag);               /* eliberez graful */
  *ag = NULL;
}

/* functia de citire a grafului din matrice */
TGL* CitGraf(char matrice[OMIE][OSUTA]) {
  FILE* fin = fopen("tema3.in", "r");    /* deschid fisierul pentru citire */
  char sursa[OSUTA], destinatie[OSUTA];
  int cost = 0, nr_obiective = 0, nr_drumuri = 0;
  AArc *p = NULL, aux = NULL;
  if (!fin) return NULL;                /* daca nu se poate deschide fisierul de intrare returnez NULL */
  if (!fscanf (fin, "%d", &nr_obiective) || nr_obiective <= 0) return NULL;  /* citesc numarul de noduri */
  if (!fscanf (fin, "%d", &nr_drumuri) || nr_drumuri <= 0) return NULL;       /* citesc numarul arcelor */
  TGL* g = AlocG(nr_obiective);       /* aloc graful cu numarul nodurilor citit anterior */
  if (!g) return NULL;                /* daca alocarea nu a reusit returnez NULL */
  int i = 0;          /* contorul pentru numarul drumurilor citite */
  int contor = 0;     /* contorul pentru codificarea surselor / destinatiilor */
  for (i = 0; i < nr_drumuri; i++) {
    int codificare_sursa = 0, codificare_destinatie = 0;
    fscanf(fin, "%s", sursa);                // citesc sursa
    fscanf(fin, "%s", destinatie);           // citesc destinatia
    fscanf(fin, "%d", &cost);                // citesc costul
    codificare_sursa = codificare(matrice, &contor, sursa);               // codific numeric sursa
    codificare_destinatie = codificare(matrice, &contor, destinatie);     // codific numeric destinatia
    p = g->x + codificare_sursa;
    while (*p && (*p)->d < codificare_destinatie) p = &(*p)->urm;  // parcurg vectorul de adrese pana la ultimul element
    aux = (TCelArc*)calloc(sizeof(TCelArc), 1);      // aloc un nou nod
    if (!aux) {
      DistrG(&g); return NULL;        // daca alocarea nu a reusit, returnez NULL
    }
    aux->urm = *p; *p = aux;
    aux->d = codificare_destinatie; aux->c = cost;  // atribui valorile campurilor corespunzatoare
    p = g->x + codificare_destinatie;
    while (*p && (*p)->d < codificare_sursa) p = &(*p)->urm;  // parcurg vectorul de adrese pana la ultimul element
    aux = (TCelArc*)calloc(sizeof(TCelArc), 1);      // aloc un nou nod
    if (!aux) {
      DistrG(&g); return NULL;       // daca alocarea nu a reusit, returnez NULL
    }
    aux->urm = *p; *p = aux;
    aux->d = codificare_sursa; aux->c = cost;  // atribui valorile campurilor corespunzatoare
  }
  fclose(fin);
  return g;
}

/* algoritmul ce atribuie codificari numerice elementelor grafului */
int codificare(char matrice[OMIE][OSUTA], int* contor, char sir[OSUTA]) {
  int i = 1;
  for (i = 1; i <= (*contor); i++) {      // parcurg codificarile
    if (strcmp(sir, matrice[i]) == 0) {   // verific daca sirul verificat are o codificare existenta
      return i;     // returnez codificarea corespunzatoare
    }
  }
  snprintf(matrice[i], strlen(sir)+1, "%s", sir);
  (*contor)++;   // incrementez contorul
  return i;      // returnez codificarea corespunzatoare
}

/* functia de afisare a grafului, folosita pentru verificarea corectitudinii constructiei grafului */
void AfiGrafL(TGL* g) {
  AArc l = NULL;
  int i = 1;
  for (i = 1; i <= g->n; i++) {
    l = g->x[i];
    if (!l) {
      printf("%d: - \n", i);
    } else {
      printf("%d: ", i);
    }
    for (l = g->x[i]; l != NULL; l = l->urm)
      printf(" %d (%d)", l->d, l->c);
    printf("\n");
  }
}

/* algoritmul de parcurgere in adancime a grafului */
void DFS(int nr_noduri, AArc vector[OSUTA], int nod, int vizitat[OSUTA]) {
  vizitat[nod] = 1;          //  marchez nodul drept vizitat
  AArc aux = vector[nod];
  while (aux) {
    if (!vizitat[aux->d]) {        // daca nodul nu a fost vizitat
      DFS(nr_noduri, vector, aux->d, vizitat);  // se apeleaza recursiv functia
    }
    aux = aux->urm;
  }
}

/* algoritmul ce afla numarul componentelor conexe */
void CompConex(TGL *g) {
  FILE *fout = fopen("tema3.out", "w");  // se deschide fisierul de iesire
  if (!fout) return;                // daca nu se poate deschide fisierul de iesire returnez NULL
  int vizitat[OMIE] = {0};
  int nr_componente_conexe = 0;
  int i = 0, j = 0;
  int vector_sume[OMIE] = {0};  // vectorul in care retin costul minim al parcurgerii fiecarei componente conexe
  for (i = 1; i < g->n; i++) {  // parcurg toate nodurile grafului
    if (!vizitat[i]) {
      DFS(g->n, g->x, i, vizitat);  // se apeleaza functia de parcurgere in adancime
      vector_sume[nr_componente_conexe] = Prim(i, g);  // se adauga parcurgerea minima a componentei conexe
      nr_componente_conexe++;  // cresc numarul componentelor conexe
    }
  }
  fprintf(fout, "%d", nr_componente_conexe);
  for (i = 0; i < nr_componente_conexe - 1; i++) {
    for (j = i + 1; j < nr_componente_conexe; j++) {  // sortez vectorul sumelor crescator
      if (vector_sume[i] > vector_sume[j]) {
        int aux = vector_sume[i];
        vector_sume[i] = vector_sume[j];
        vector_sume[j] = aux;
      }
    }
  }
  fprintf(fout, "\n");
  for (i = 0; i < nr_componente_conexe; i++) {  // afisez vectorul sortat
    fprintf(fout, "%d\n", vector_sume[i]);
  }
  fclose(fout);  // inchid fisierul de iesire
}

/* functia auxiliara care afla nodul pana la care se parcurge distanta minima */
int DistantaMinima(int* vizitat, int* distante, int nr_noduri) {
  int min = INT_MAX;
  int nod = 0;
  int i = 1;
  for (i = 1; i <= nr_noduri; i++) {  // parcurg nodurile grafului
    if (vizitat[i] == 0 && distante[i] < min) {  // daca nodul nu a fost vizitat si are distanta minima
      min = distante[i];   // actualizez distanta minima
      nod = i;             // retin nodul cu distanta minima
    }
  }
  return nod;
}

/* functia ce afla costul total minim al drumurilor */
int Prim(int nod, TGL* g) {
  int nr_noduri = g->n;
  int* distante = (int*) malloc((nr_noduri + 1) * sizeof(int));  // vectorul in care retin distantele minime
  int* vizitat = (int*) malloc((nr_noduri + 1) * sizeof(int));    // vectorul in care retin nodurile vizitate
  int i = 1;
  for (i = 1; i <= nr_noduri; i++) {  // initializare vectori
    distante[i] = INT_MAX;
    vizitat[i] = 0;
  }
  distante[nod] = 0;
  for (i = 1; i <= nr_noduri; i++) {  // parcurg nodurile grafului
    int min_node = DistantaMinima(vizitat, distante, nr_noduri);  // aflu nodul cu distanta minima
    if (min_node == 0) break;
    vizitat[min_node] = 1;
    AArc L = g->x[min_node];
    AArc p =NULL;
    for (p = L; p != NULL; p = p->urm) {  // parcurg vectorul de arce ale nodului minim
      if (vizitat[p->d] == 0 && (p->c < distante[p->d])) {  // nod nevizitat si cost mai mic decat distanta curenta
        distante[p->d] = p->c;  // se actualizeaza costul minim
      }
    }
  }
  int suma_minima = 0;
  for (i = 1; i <= nr_noduri; i++) {
    if (distante[i] != INT_MAX)
      suma_minima = suma_minima + distante[i];  // se calculeaza suma minima a costurilor
  }
  free(distante);
  free(vizitat);
  return suma_minima;
}

TGL2* AlocG2(int nr) { /* aloca spatiu pentru descriptor graf si vector de (n+1) adrese liste arce */
  TGL2* g = (TGL2*)malloc(sizeof(TGL2));
  if (!g) return NULL;
  g->x = (AArc*)calloc((nr+1), sizeof(AArc)); /* spatiu vector adrese liste arce */
  if (!g->x) {     /* daca nu reuseste alocarea vectorului, eliberez memoria ocupata de graf */
    free(g);
    return NULL;
  }
  g->n = nr;
  return g;
}

/* functia de distrugere a grafului si eliberare a memoriei ocupate de acesta */
void DistrG2(TGL2** ag) {
  int i = 1;
  AArc p = NULL, aux = NULL;
  for (i = 1; i <= (*ag)->n; i++) {       /* parcurg graful */
    p = (*ag)->x[i];
    while (p) {                   /* eliberez memoria ocupata de nodurile grafului */
      aux = p;
      p = p->urm;
      free(aux);
    }
  }
  free((*ag)->x);          /* eliberez vectorul de adrese */
  free(*ag);               /* eliberez graful */
  *ag = NULL;
}

/* citirea grafului orientat */
TGL2* CitGraf2(char matrice[OMIE][OSUTA], int *greutate) {
  FILE* fin = fopen("tema3.in", "r");    /* deschid fisierul pentru citire */
  char sursa[OSUTA], destinatie[OSUTA];
  int cost = 0, nr_obiective = 0, nr_drumuri = 0;
  AArc *p = NULL, aux = NULL;
  if (!fin) return NULL;                /* daca nu se poate deschide fisierul de intrare returnez NULL */
  if (!fscanf (fin, "%d", &nr_obiective) || nr_obiective <= 0) return NULL;  /* citesc numarul de noduri */
  if (!fscanf (fin, "%d", &nr_drumuri) || nr_drumuri <= 0) return NULL;       /* citesc numarul arcelor */
  TGL2* g = AlocG2(nr_obiective);       /* aloc graful cu numarul nodurilor citit anterior */
  if (!g) return NULL;                /* daca alocarea nu a reusit returnez NULL */
  int i = 0;          /* contorul pentru numarul drumurilor citite */
  int contor = 0;     /* contorul pentru codificarea surselor / destinatiilor */
  for (i = 0; i < nr_drumuri; i++) {
    int codificare_sursa = 0, codificare_destinatie = 0;
    fscanf(fin, "%s", sursa);                // citesc sursa
    fscanf(fin, "%s", destinatie);           // citesc destinatia
    fscanf(fin, "%d", &cost);                // citesc costul
    codificare_sursa = codificare(matrice, &contor, sursa);               // codific numeric sursa
    codificare_destinatie = codificare(matrice, &contor, destinatie);     // codific numeric destinatia
    p = g->x + codificare_sursa;
    while (*p && (*p)->d < codificare_destinatie) p = &(*p)->urm;  // parcurg vectorul de adrese pana la ultimul element
    aux = (TCelArc*)calloc(sizeof(TCelArc), 1);      // aloc un nou nod
    if (!aux) {
      DistrG2(&g);
      return NULL;        // daca alocarea nu a reusit, returnez NULL
    }
    aux->urm = *p; *p = aux;
    aux->d = codificare_destinatie; aux->c = cost;  // atribui valorile campurilor corespunzatoare
  }
  g->ad = (int*)malloc((nr_obiective + 1) * sizeof(int));   // aloc vectorul de adancimi
  if (!(g->ad)) {
    DistrG2(&g);
  }
  char nod[OSUTA];
  for (i = 1; i <= nr_obiective; i++) {         // parcurg nodurile grafului
    fscanf(fin, "%s", nod);
    int codificare_nod = codificare(matrice, &contor, nod);    // identific nodul al carui adancime o citesc
    fscanf(fin, "%d", &g->ad[codificare_nod]);  // citesc adancimea nodului in vectorul de adancime
  }
  fscanf(fin, "%d", greutate);   // citesc greutatea
  fclose(fin);
  return g;
}

/* functia de afisare a grafului orientat (folosita pentru verificarea corectitudinii construirii grafului orientat) */
void AfiGrafL2(TGL2* g) {
  AArc l = NULL;
  int i = 1;
  for (i = 1; i <= g->n; i++) {
    l = g->x[i];
    if (!l) {
      printf("%d (%d): - \n", i, g->ad[i]);
    } else {
      printf("%d (%d): ", i, g->ad[i]);
    }
    for (l = g->x[i]; l != NULL; l = l->urm)
      printf(" %d (%d)", l->d, l->c);
    printf("\n");
  }
}

/* functia care returneaza nodul cu scorul minim */
int ScorMinim(int* vizitat, float* scor, int nr_noduri) {
  float min = INT_MAX;
  int nod = 0;
  int i = 1;
  for (i = 1; i <= nr_noduri; i++) {  // parcurg nodurile grafului
    if (vizitat[i] == 0 && scor[i] < min) {  // daca nodul nu a fost vizitat si are scorul minim
      min = scor[i];   // actualizez scorul minim
      nod = i;             // retin nodul cu scorul minim
    }
  }
  return nod;
}

/* algoritmul Dijkstra pentru aflarea drumului cu costul minim */
void Dijkstra(int nod, TGL2* g, char matrice[OMIE][OSUTA], int greutate) {
  FILE* fout = fopen("tema3.out", "w");                               // deschid fisierul de iesire
  if (!fout) return;                /* daca nu se poate deschide fisierul de iesire returnez NULL */
  int nr_noduri = g->n;
  float* scor = (float*) malloc((nr_noduri + 1) * sizeof(float));  // aloc vectorul de scor
  int* distante = (int*) malloc((nr_noduri + 1) * sizeof(int));    // aloc vectorul de distante
  int* vizitat = (int*) malloc((nr_noduri + 1)* sizeof(int));      // aloc vectorul in care contorizez nodurile vizitat
  int* tati = (int*)malloc((nr_noduri + 1) * sizeof(int));    // aloc vectorul de tati folosit pentru afisarea drumului
  int i = 1;
  for (i = 1; i <= nr_noduri; i++) {    // initializez vectorii alocati anterior
      scor[i] = INT_MAX;
      distante[i] = INT_MAX;
      vizitat[i] = 0;
      tati[i] = 0;
  }
  int aux_nod = codificare(matrice, &nr_noduri, "Corabie");     // codific numeric nodul Corabie
  distante[aux_nod] = 0;
  scor[aux_nod] = 0;
  tati[aux_nod] = 0;
  for (i = 1; i <= nr_noduri; i++) {    // parcurg nodurile grafului
    int nod_min = ScorMinim(vizitat, scor, nr_noduri);  // aflu nodul cu scorul minim
    vizitat[nod_min] = 1;              // marchez nodul drept vizitat
    AArc L = g->x[nod_min], p =NULL;
    for (p = L; p != NULL; p = p->urm) {  // parcurg vectorul de arce ale nodului cu scor minim
      float s_curent = (float)(p->c) / (float)(g->ad[p->d]);  // calculez scorul curent conform formulei
      if (vizitat[p->d] == 0 && (scor[nod_min] + s_curent < scor[p->d])) {  // nod nevizitat si s_curent < scor_minim
        scor[p->d] = scor[nod_min] + s_curent;              // actualizez scorul minim
        distante[p->d] = distante[nod_min] + p->c;          // actualizez costul
        tati[p->d] = nod_min;      // actualizez vectorul de tati
      }
    }
  }
  if (distante[nod] == INT_MAX) {     // cazul in care nu exista un drum de la corabie la insula
    fprintf(fout, "Echipajul nu poate ajunge la insula");
    free(g->ad);
    free(scor);
    free(distante);
    free(vizitat);
    free(tati);
    fclose(fout);
    return;
  }
  for (i = 1; i <= nr_noduri; i++) {    // initializez vectorii alocati anterior
      scor[i] = INT_MAX;
      distante[i] = INT_MAX;
      vizitat[i] = 0;
      tati[i] = 0;
  }
  distante[nod] = 0;
  scor[nod] = 0;
  tati[nod] = 0;
  for (i = 1; i <= nr_noduri; i++) {    // parcurg nodurile grafului
    int nod_min = ScorMinim(vizitat, scor, nr_noduri);  // aflu nodul cu scorul minim
    vizitat[nod_min] = 1;              // marchez nodul drept vizitat
    AArc L = g->x[nod_min], p =NULL;
    for (p = L; p != NULL; p = p->urm) {
      float s_curent = (float)(p->c) / (float)(g->ad[p->d]);  // calculez scorul curent conform formulei
      if (vizitat[p->d] == 0 && (scor[nod_min] + s_curent < scor[p->d])) {  // nod nevizitat si s_curent < scor_minim
        scor[p->d] = scor[nod_min] + s_curent;              // actualizez scorul minim
        distante[p->d] = distante[nod_min] + p->c;          // actualizez costul
        tati[p->d] = nod_min;      // actualizez vectorul de tati
      }
    }
  }
  i = codificare(matrice, &nr_noduri, "Corabie");
  if (distante[i] == INT_MAX) {  // cazul in care nu exista niciun drum de la insula la corabie
    fprintf(fout, "Echipajul nu poate transporta comoara inapoi la corabie");
    free(g->ad);
    free(scor);
    free(distante);
    free(vizitat);
    free(tati);
    fclose(fout);
    return;
  }
  int auxiliar[OMIE];
  int aux = 0;
  i = codificare(matrice, &nr_noduri, "Corabie");
  int adancime_minima = INT_MAX;
  while (i) {
    if (g->ad[i] < adancime_minima && g->ad[i] != 1) {  // aflu adancimea minima
      adancime_minima = g->ad[i];   // actualizez adancimea minima
    }
    auxiliar[aux] = i;   // inversez vectorul de tati pentru afisarea drumului de la insula la corabie
    aux++;
    i = tati[i];
  }
  for (i = aux - 1; i >=0; i--) {
    fprintf(fout, "%s ", matrice[auxiliar[i]]);  // afisez drumul de la insula la corabie
  }
  fprintf(fout, "\n");
  fprintf(fout, "%d", distante[codificare(matrice, &nr_noduri, "Corabie")]);  // afisez costul
  fprintf(fout, "\n");
  fprintf(fout, "%d", adancime_minima);  // afisez adancimea minima
  fprintf(fout, "\n");
  fprintf(fout, "%d", greutate/adancime_minima);  // afisez numarul de drumuri care trebuie facute
  free(g->ad);        // eliberez memoria ocupata de vectorii alocati dinamic
  free(scor);
  free(distante);
  free(vizitat);
  free(tati);
  fclose(fout);
}
