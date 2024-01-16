#include <stdio.h>
#include <stdlib.h>

struct Composto {
    int custo;
    int limite;
}; 
typedef struct Composto Composto_t;

int main() {
    int n, m;

    scanf("%d %d", &n, &m);

    int valorProdutos[n];
    Composto_t compostos[m];
    float limitantes[n][m];

    for (int i = 0; i < n; i++){
        scanf("%d", &valorProdutos[i]);
    }

    for (int i = 0; i < m; i++){
        scanf("%d", &compostos[i].custo);
        scanf("%d", &compostos[i].limite);
    }

    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            scanf("%f", &limitantes[i][j]);
        }
    }

    float vetorAux[n];
    for(int i = 0; i < n; i++){
        vetorAux[i] = 0.0;
        for(int j = 0; j < m; j++){
            vetorAux[i] += limitantes[i][j] * compostos[j].custo;
        }
        vetorAux[i] = valorProdutos[i] - vetorAux[i];
    }
    
    FILE *arquivo;

    arquivo = fopen("entrada_lp_solve.txt", "w");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.");
        return 1;
    }

    fprintf(arquivo, "max :");

    for(int i = 0; i < n; i++){
        if(i == n-1)
            fprintf(arquivo, " %.1fx%d;\n\n", vetorAux[i], i+1);
        else
            fprintf(arquivo, " %.1fx%d +", vetorAux[i], i+1);
    }

    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            if(j == n-1)
                fprintf(arquivo, "%.1fx%d", limitantes[j][i], j+1);
            else
                fprintf(arquivo, "%.1fx%d + ", limitantes[j][i], j+1);
        }
        fprintf(arquivo, " <= %d;\n", compostos[i].limite);
    }

    fclose(arquivo);

    return 0;
}