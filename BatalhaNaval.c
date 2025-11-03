#include <stdio.h>

#define N 10            // Tamanho do tabuleiro (10x10)
#define NAVIO 3         // Valor que representa parte de navio no tabuleiro
#define HABILIDADE 5    // Valor que representa área afetada por habilidade
#define TAM_MATRIZ 5    // Usaremos matrizes 5x5 para as habilidades (pode ser 5 ou 7)

/*
  Programa:
  - Cria tabuleiro 10x10 (0 = água)
  - Posiciona 2 navios (tamanho fixo 3): um horizontal e outro vertical
  - Constrói três matrizes de habilidade (Cone, Cruz, Octaedro) com valores 0/1
    usando loops aninhados e condicionais
  - Sobrepõe cada matriz ao tabuleiro com pontos de origem definidos no código
  - Marca as posições afetadas com o valor HABILIDADE (5)
  - Exibe o tabuleiro com colunas A-J e linhas 1-10
*/

void imprimirTabuleiro(int tab[N][N]) {
    // Cabeçalho das colunas: A-J
    printf("   ");
    for (int c = 0; c < N; c++) {
        printf(" %c ", 'A' + c);
    }
    printf("\n");

    // Linhas numeradas 1-10
    for (int r = 0; r < N; r++) {
        printf("%2d ", r + 1);
        for (int c = 0; c < N; c++) {
            printf("%2d ", tab[r][c]);
        }
        printf("\n");
    }
}

int main() {
    int tabuleiro[N][N];

    // Inicializar tabuleiro com 0 (água)
    for (int r = 0; r < N; r++)
        for (int c = 0; c < N; c++)
            tabuleiro[r][c] = 0;

    // ----------------------------
    // Posicionar 2 navios (tamanho 3)
    // ----------------------------
    // Valores definidos diretamente no código (simplificação do enunciado)
    int linhaHorizontal = 2;   // índice 0-based -> linha 3 visualmente
    int colunaHorizontal = 4;  // índice 0-based -> coluna E (começa em 0)
    int linhaVertical = 5;     // índice 0-based -> linha 6
    int colunaVertical = 7;    // índice 0-based -> coluna H

    // Navio horizontal (3 casas) - validação de limite
    if (colunaHorizontal + 3 <= N) {
        for (int i = 0; i < 3; i++) {
            tabuleiro[linhaHorizontal][colunaHorizontal + i] = NAVIO;
        }
    } else {
        printf("Erro: navio horizontal sairia do limite do tabuleiro!\n");
        return 1;
    }

    // Navio vertical (3 casas) - validação de limite e sobreposição
    if (linhaVertical + 3 <= N) {
        int sobreposicao = 0;
        for (int i = 0; i < 3; i++) {
            if (tabuleiro[linhaVertical + i][colunaVertical] != 0) {
                sobreposicao = 1;
                break;
            }
        }
        if (!sobreposicao) {
            for (int i = 0; i < 3; i++) {
                tabuleiro[linhaVertical + i][colunaVertical] = NAVIO;
            }
        } else {
            printf("Erro: navio vertical sobrepõe navio horizontal!\n");
            return 1;
        }
    } else {
        printf("Erro: navio vertical sairia do limite do tabuleiro!\n");
        return 1;
    }

    // ---------------------------------------------------
    // Construir matrizes de habilidade (TAM_MATRIZ x TAM_MATRIZ)
    // Usamos TAM_MATRIZ = 5 (índices 0..4), centro em 2
    // ---------------------------------------------------
    int cone[TAM_MATRIZ][TAM_MATRIZ];
    int cruz[TAM_MATRIZ][TAM_MATRIZ];
    int octaedro[TAM_MATRIZ][TAM_MATRIZ];
    int center = TAM_MATRIZ / 2; // para 5 => 2

    // Construção do CONE:
    // Topo do cone = linha 0 (um 1 no centro), expande para baixo: linha r tem 1s
    // nas colunas center - r ... center + r (condição: |col - center| <= r)
    for (int r = 0; r < TAM_MATRIZ; r++) {
        for (int c = 0; c < TAM_MATRIZ; c++) {
            if (r <= center) {
                // para r = 0..center a largura cresce: cond |c - center| <= r
                cone[r][c] = ( (c >= center - r) && (c <= center + r) ) ? 1 : 0;
            } else {
                // abaixo do centro (quando TAM_MATRIZ ímpar) podemos manter 0
                cone[r][c] = 0;
            }
        }
    }

    // Construção da CRUZ:
    // 1s na linha central e na coluna central
    for (int r = 0; r < TAM_MATRIZ; r++) {
        for (int c = 0; c < TAM_MATRIZ; c++) {
            if (r == center || c == center) cruz[r][c] = 1;
            else cruz[r][c] = 0;
        }
    }

    // Construção do OCTAEDRO (vista frontal -> losango/diamante):
    // linhas com contagens 1,3,5,3,1 (centralizado)
    for (int r = 0; r < TAM_MATRIZ; r++) {
        int dist = abs(r - center);        // 0,1,2,1,0 -> largura = center - dist
        int half_width = center - dist;    // 2,1,0,1,2
        for (int c = 0; c < TAM_MATRIZ; c++) {
            // queremos 1 quando |c - center| <= (center - dist)
            if (abs(c - center) <= (center - dist)) octaedro[r][c] = 1;
            else octaedro[r][c] = 0;
        }
    }

    // ---------------------------------------------------
    // Definir pontos de origem para cada habilidade no tabuleiro
    // (valores definidos no código; podem ser alterados para testar)
    // ---------------------------------------------------
    // Para o CONE: ponto de origem é o topo do cone (o centro da primeira linha do patch).
    // Mapearemos a linha 0 da matriz do cone para a origem; a matriz expande para baixo.
    int origem_cone_r = 1; // linha no tabuleiro (0-based)
    int origem_cone_c = 5; // coluna no tabuleiro (0-based) -> o centro do topo do cone

    // Para a CRUZ: ponto de origem é o centro da matriz (center)
    int origem_cruz_r = 6;
    int origem_cruz_c = 2;

    // Para o OCTAEDRO: origem no centro da matriz
    int origem_octa_r = 3;
    int origem_octa_c = 8;

    // ---------------------------------------------------
    // Função: sobrepor matriz de habilidade ao tabuleiro
    // Regras de mapeamento:
    // - Para cone: matriz linha m_r -> board row = origem_cone_r + m_r
    //               matriz col m_c -> board col = origem_cone_c + (m_c - center)
    // - Para cruz e octaedro: centro da matriz (center,center) -> origem no tabuleiro:
    //               board row = origem_r + (m_r - center)
    //               board col = origem_c + (m_c - center)
    // Sempre verificar limites do tabuleiro (0..N-1)
    // Marcar as posições afetadas com HABILIDADE (5) quando matriz[.] == 1
    // ---------------------------------------------------

    // Sobrepor CONE
    for (int mr = 0; mr < TAM_MATRIZ; mr++) {
        for (int mc = 0; mc < TAM_MATRIZ; mc++) {
            if (cone[mr][mc] == 1) {
                int br = origem_cone_r + mr;                         // board row
                int bc = origem_cone_c + (mc - center);             // board col (center alignment)
                if (br >= 0 && br < N && bc >= 0 && bc < N) {
                    tabuleiro[br][bc] = HABILIDADE;
                }
            }
        }
    }

    // Sobrepor CRUZ
    for (int mr = 0; mr < TAM_MATRIZ; mr++) {
        for (int mc = 0; mc < TAM_MATRIZ; mc++) {
            if (cruz[mr][mc] == 1) {
                int br = origem_cruz_r + (mr - center);
                int bc = origem_cruz_c + (mc - center);
                if (br >= 0 && br < N && bc >= 0 && bc < N) {
                    tabuleiro[br][bc] = HABILIDADE;
                }
            }
        }
    }

    // Sobrepor OCTAEDRO
    for (int mr = 0; mr < TAM_MATRIZ; mr++) {
        for (int mc = 0; mc < TAM_MATRIZ; mc++) {
            if (octaedro[mr][mc] == 1) {
                int br = origem_octa_r + (mr - center);
                int bc = origem_octa_c + (mc - center);
                if (br >= 0 && br < N && bc >= 0 && bc < N) {
                    tabuleiro[br][bc] = HABILIDADE;
                }
            }
        }
    }

    // ---------------------------------------------------
    // Exibir as matrizes de habilidade (opcional, para verificar visualmente)
    // Mostramos 1s e 0s para cada matriz antes da sobreposição (bom para debug)
    // ---------------------------------------------------
    printf("\nMatriz CONE (1 = efeito):\n");
    for (int r = 0; r < TAM_MATRIZ; r++) {
        for (int c = 0; c < TAM_MATRIZ; c++) {
            printf("%d ", cone[r][c]);
        }
        printf("\n");
    }

    printf("\nMatriz CRUZ (1 = efeito):\n");
    for (int r = 0; r < TAM_MATRIZ; r++) {
        for (int c = 0; c < TAM_MATRIZ; c++) {
            printf("%d ", cruz[r][c]);
        }
        printf("\n");
    }

    printf("\nMatriz OCTAEDRO (1 = efeito):\n");
    for (int r = 0; r < TAM_MATRIZ; r++) {
        for (int c = 0; c < TAM_MATRIZ; c++) {
            printf("%d ", octaedro[r][c]);
        }
        printf("\n");
    }

    // ---------------------------------------------------
    // Imprimir tabuleiro final com navios e áreas de habilidade
    // 0 = água, 3 = navio, 5 = área afetada
    // ---------------------------------------------------
    printf("\n=== TABULEIRO COM HABILIDADES ===\n\n");
    imprimirTabuleiro(tabuleiro);

    return 0;
}
