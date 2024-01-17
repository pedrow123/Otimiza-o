#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define MAX_N 100

int n, k, m;
int conflitos[MAX_N][MAX_N];
int afinidades[MAX_N][MAX_N];
int limite = 0;
int min_conflitos = MAX_N;
int vetor_final1[MAX_N] = {0};
int vetor_final2[MAX_N] = {0};
int escolhido[MAX_N];
int chamadas;
int usar_bound = 1;
int usar_viab = 1; 

int checa_conflitos(int grupo[], int grupo_tam) {
    int conflitos_grupo = 0;
    for (int i = 1; i <= grupo_tam; ++i) {
        for (int j = i + 1; j <= grupo_tam; ++j) {
            int u = grupo[i] - 1;
            int v = grupo[j] - 1;
            if (conflitos[u][v] || conflitos[v][u])
                conflitos_grupo++;
        }
    }
    return conflitos_grupo;
}

int checa_afinidades(int grupo[], int grupo_tam) {
    if(m == 0)
        return 1;
    for (int i = 1; i <= grupo_tam; ++i) {
        for (int j = i + 1; j <= grupo_tam; ++j) {
            int u = grupo[i] - 1;
            int v = grupo[j] - 1;
            if (!afinidades[u][v] && !afinidades[v][u])
                return 0;
        }
    }
    return 1;
}

int grupos_iguais(int grupo1[], int grupo2[]) {
    if (grupo1[0] == grupo2[0]) {
        for (int i = 1; i <= grupo1[0]; i++) {
            if (grupo1[i] != grupo2[i]) {
                return 0;
            }
        }
        return 1;
    }
    return 0;
}

int contem_elemento(int grupo[], int elemento) {
    for (int i = 1; i <= grupo[0]; i++) {
        if (grupo[i] == elemento) {
            return 1;
        }
    }
    return 0;
}

int verifica_triangulos(int grupo1[], int grupo2[]) {

    int count = 0;
    int i, j, k;

    // Verifica todas as combinações possíveis de triângulos na matriz de conflitos
    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            for (k = j + 1; k < n; k++) {
                if (conflitos[i][j] == 1 && conflitos[j][k] == 1 && conflitos[k][i] == 1 &&
                    escolhido[i] == 0 && escolhido[j] == 0 && escolhido[k] == 0) {
                    int shared_vertex = 0;
                    int x;
                    
                    // Verifica se há vértices compartilhados com outros triângulos
                    for (x = 0; x < n; x++) {
                        if ((x != i) && (x != j) && (x != k) &&
                            (conflitos[i][x] == 1) && (conflitos[j][x] == 1) && (conflitos[k][x] == 1) &&
                            contem_elemento(grupo1, i) && contem_elemento(grupo2, i) &&
                            contem_elemento(grupo1, j) && contem_elemento(grupo2, j) &&
                            contem_elemento(grupo1, k) && contem_elemento(grupo2, k)) {
                            shared_vertex = 1;
                            break;
                        }
                    }
                    
                    if (!shared_vertex) {
                        count++; // Encontrou um triângulo sem vértices compartilhados
                    }
                }
            }
        }
    }

    return count;
}

int bound(int grupo1[], int grupo2[]){
    int triang = verifica_triangulos(grupo1, grupo2);
    int confs  = checa_conflitos(grupo1, grupo1[0]) + checa_conflitos(grupo2, grupo2[0]);

    return triang + confs;
}

void gerar_combinacoes(int grupo1[], int grupo2[], int idx) {
    if (limite < ((int)pow(2, n) - 2) / 2){
        if (idx == n + 1) {
            if (grupo1[0] >= 1 && grupo1[0] <= n - 1 && grupo2[0] >= 1 && grupo2[0] <= n - 1 && checa_afinidades(grupo1, grupo1[0]) &&
                checa_afinidades(grupo2, grupo2[0])) {
                int grupo_completo[MAX_N * 2] = {0};

                grupo_completo[0] = grupo1[0] + grupo2[0];
                for (int i = 1; i <= grupo1[0]; i++) {
                    grupo_completo[i] = grupo1[i];
                }
                for (int i = 1, j = grupo1[0] + 1; i <= grupo2[0]; i++, j++) {
                    grupo_completo[j] = grupo2[i];
                }

                int valido = 1;
                for (int i = 1; i <= n; i++) {
                    if (!contem_elemento(grupo_completo, i)) {
                        valido = 0;
                        break;
                    }
                }

                if (valido) {
                    int soma_conflitos = checa_conflitos(grupo1, grupo1[0]) + checa_conflitos(grupo2, grupo2[0]);
                    if (soma_conflitos < min_conflitos) {
                        min_conflitos = soma_conflitos;
                        vetor_final1[0] = grupo1[0];
                        for (int i = 1; i <= grupo1[0]; i++)
                            vetor_final1[i] = grupo1[i];
                        vetor_final2[0] = grupo2[0];
                        for (int i = 1; i <= grupo2[0]; i++)
                            vetor_final2[i] = grupo2[i];
                    }
                }
            }
        } else {

            grupo1[0]++;
            grupo1[grupo1[0]] = idx;
            escolhido[idx] = 1;
            // int bound1 = bound(grupo1, grupo2);
            // printf("grupo 1: \n");
            // for(int i = 1; i <= grupo1[0]; ++i){
            //     printf("%d ", grupo1[i]);
            // }
            // printf("\n");
            // printf("grupo 2: \n");
            // for(int i = 1; i <= grupo2[0]; ++i){
            //     printf("%d ", grupo2[i]);
            // }
            // printf("\n");
            // branch and bound aqui
            // if(min_conflitos > bound(grupo1, grupo2)){
            if(min_conflitos > bound(grupo1, grupo2) || !usar_bound){
                chamadas++;
                gerar_combinacoes(grupo1, grupo2, idx + 1);
            }
            
            grupo1[0]--;
            escolhido[idx] = 0;

            grupo2[0]++;
            grupo2[grupo2[0]] = idx;
            escolhido[idx] = 1;
            // branch and bound aqui

            // if(min_conflitos > bound(grupo1, grupo2)){
            if(min_conflitos > bound(grupo1, grupo2) || !usar_bound){
                chamadas++;
                gerar_combinacoes(grupo1, grupo2, idx + 1);
            }
            grupo2[0]--;
            escolhido[idx] = 0;
        }
    }
}

void gerar_combinacoes_viab(int grupo1[], int grupo2[], int idx) {
        if (idx == n + 1) {
            if (checa_afinidades(grupo1, grupo1[0]) &&
                checa_afinidades(grupo2, grupo2[0])) {
                // int grupo_completo[MAX_N * 2] = {0};

                // grupo_completo[0] = grupo1[0] + grupo2[0];
                // for (int i = 1; i <= grupo1[0]; i++) {
                //     grupo_completo[i] = grupo1[i];
                // }
                // for (int i = 1, j = grupo1[0] + 1; i <= grupo2[0]; i++, j++) {
                //     grupo_completo[j] = grupo2[i];
                // }

                // int valido = 1;
                // for (int i = 1; i <= n; i++) {
                //     if (!contem_elemento(grupo_completo, i)) {
                //         valido = 0;
                //         break;
                //     }
                // }

                // if (valido) {
                    int soma_conflitos = checa_conflitos(grupo1, grupo1[0]) + checa_conflitos(grupo2, grupo2[0]);
                    if (soma_conflitos < min_conflitos) {
                        min_conflitos = soma_conflitos;
                        vetor_final1[0] = grupo1[0];
                        for (int i = 1; i <= grupo1[0]; i++)
                            vetor_final1[i] = grupo1[i];
                        vetor_final2[0] = grupo2[0];
                        for (int i = 1; i <= grupo2[0]; i++)
                            vetor_final2[i] = grupo2[i];
                    }
                // }
            }
        } else {

            grupo1[0]++;
            grupo1[grupo1[0]] = idx;
            escolhido[idx] = 1;
            // int bound1 = bound(grupo1, grupo2);
            // printf("grupo 1: \n");
            // for(int i = 1; i <= grupo1[0]; ++i){
            //     printf("%d ", grupo1[i]);
            // }
            // printf("\n");
            // printf("grupo 2: \n");
            // for(int i = 1; i <= grupo2[0]; ++i){
            //     printf("%d ", grupo2[i]);
            // }
            // printf("\n");
            // branch and bound aqui
            // if(min_conflitos > bound(grupo1, grupo2)){
            // if(min_conflitos > bound(grupo1, grupo2) || !usar_bound){
                // chamadas++;
                gerar_combinacoes(grupo1, grupo2, idx + 1);
            // }
            
            grupo1[0]--;
            escolhido[idx] = 0;

            grupo2[0]++;
            grupo2[grupo2[0]] = idx;
            escolhido[idx] = 1;
            // branch and bound aqui

            // if(min_conflitos > bound(grupo1, grupo2)){
            // if(min_conflitos > bound(grupo1, grupo2) || !usar_bound){
                gerar_combinacoes(grupo1, grupo2, idx + 1);
                                chamadas++;

            // }
            grupo2[0]--;
            // escolhido[idx] = 0;
        }
    
}


void imprime(int grupo1[], int grupo2[]){
    if(usar_viab == 1)
        gerar_combinacoes(grupo1, grupo2, 1);
    else
        gerar_combinacoes_viab(grupo1, grupo2, 1);

    printf(">>>>>>>>>>>>>>>>> %d chamadas.\n", chamadas);

    if (min_conflitos == MAX_N) {
        printf("Não existe solução\n");
    } else {
        printf("Mínimo conflitos: %d\n", min_conflitos);
        for (int i = 1; i <= vetor_final1[0]; i++) {
            printf("%d ", vetor_final1[i]);
        }
        printf("\n");
    }

}

int main(int argc, char **argv) {
    scanf("%d %d %d", &n, &k, &m);

    for (int i = 0; i < k; i++) {
        int a, b;
        scanf("%d %d", &a, &b);
        conflitos[a - 1][b - 1] = 1;
        conflitos[b - 1][a - 1] = 1;
    }

    for (int i = 0; i < m; i++) {
        int a, b;
        scanf("%d %d", &a, &b);
        afinidades[a - 1][b - 1] = 1;
        afinidades[b - 1][a - 1] = 1;
    }

    int grupo1[MAX_N] = {0};
    int grupo2[MAX_N] = {0};

    grupo1[0] = 0;
    grupo2[0] = 0;

    // escolhido[1] = 1;

    // int count = verifica_triangulos(grupo1, grupo2);

    // printf("Num de triangulos de conflitos: %d\n" , count);

   
    printf("%d", argc);
    if(argc == 1){
        clock_t inicio = clock();
        imprime(grupo1, grupo2);
        clock_t fim = clock();
        double tempo = (double)(fim - inicio) * 1000.0 / CLOCKS_PER_SEC;

        // Exibição do tempo de execução
        printf("Tempo de execucao: %.3f milissegundos\n", tempo);
        return 0;
    }

    char option;
    clock_t inicio, fim;
    double tempo;

    while((option = getopt(argc, argv, "foa:")) != -1){
        switch (option){
            case 'f':      // option -a was set
                usar_viab = 0;
                // usar_bound = 0;
                inicio = clock();
                imprime(grupo1, grupo2);
                fim = clock();
                tempo = (double)(fim - inicio) * 1000.0 / CLOCKS_PER_SEC;

                printf("Tempo de execucao: %.3f milissegundos\n", tempo);
                break;
            case 'o': 
                usar_bound = 0;
                inicio = clock();
                imprime(grupo1, grupo2);
                fim = clock();
                tempo = (double)(fim - inicio) * 1000.0 / CLOCKS_PER_SEC;

                // Exibição do tempo de execução
                printf("Tempo de execucao: %.3f milissegundos\n", tempo);
                break;
            case 'a':      // option -c was set with value
                break;
            default:
                imprime(grupo1, grupo2);
                fprintf (stderr, "Usage: %s -a -b -c value\n", argv[0]);
	            exit (1) ;
        }
    }
    

    return 0;
}
