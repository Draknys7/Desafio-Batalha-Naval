#include <stdio.h>

#define TAMANHO_TABULEIRO 10  //  10x10
#define TAMANHO_NAVIO 3       

int main() {
    int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO];
    int navioHorizontal[TAMANHO_NAVIO] = {3, 3, 3};
    int navioVertical[TAMANHO_NAVIO] = {3, 3, 3};

    // navios iniciais 
    int linhaHorizontal = 2;   // linha inicial navio horizontal (0-9)
    int colunaHorizontal = 4;  // coluna inicial navio horizontal (0-7)

    int linhaVertical = 5;     // linha inicial do navio vertical (0-7)
    int colunaVertical = 7;    // coluna inicial do navio vertical (0-9)

    //  inicio tabuleiro
    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            tabuleiro[i][j] = 0;
        }
    }

    // navio horizontal
    if (colunaHorizontal + TAMANHO_NAVIO <= TAMANHO_TABULEIRO) {
        for (int i = 0; i < TAMANHO_NAVIO; i++) {
            tabuleiro[linhaHorizontal][colunaHorizontal + i] = navioHorizontal[i];
        }
    } else {
        printf("Erro: Navio horizontal sairia do limite do tabuleiro!\n");
        return 1;
    }

    // 3. Posicionar navio vertical
    if (linhaVertical + TAMANHO_NAVIO <= TAMANHO_TABULEIRO) {
        int sobreposicao = 0;
        for (int i = 0; i < TAMANHO_NAVIO; i++) {
            if (tabuleiro[linhaVertical + i][colunaVertical] != 0) {
                sobreposicao = 1;
                break;
            }
        }
        if (!sobreposicao) {
            for (int i = 0; i < TAMANHO_NAVIO; i++) {
                tabuleiro[linhaVertical + i][colunaVertical] = navioVertical[i];
            }
        } else {
            printf("Erro: Navio vertical sobrepõe o navio horizontal!\n");
            return 1;
        }
    } else {
        printf("Erro: Navio vertical sairia do limite do tabuleiro!\n");
        return 1;
    }

    // exibir o tabuleiro com cabeçalho e rótulos de linhas/colunas
    printf("\n========== BATTLESHIP ==========\n");

    // imprime cabeçalho das colunas 1 a 10
    printf("   "); // espaço para alinhar com as letras das linhas
    for (int j = 1; j <= TAMANHO_TABULEIRO; j++) {
        printf("%2d ", j); // formato com largura 2 para alinhar
    }
    printf("\n");

    // imprime linhas do tabuleiro com rótulos A-J
    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        printf("%c  ", 'A' + i); // Nome da linha (A-J)
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            printf("%2d ", tabuleiro[i][j]);
        }
        printf("\n");
    }

    return 0;
}
