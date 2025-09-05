#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <string.h>

#define MAX_RECORDS 5

int largura = 80, altura = 20;
int velocidade = 100;
int nivelMapa = 1; // Define o mapa a ser carregado. Padrão: 1 (Fácil)

typedef struct
{
    char nome[50];
    double tempo;
} Recorde;

typedef struct
{
    Recorde recordes[MAX_RECORDS];
    int quantidade;
} PilhaRecordes;

int i, j;

void ordenaRecordes(PilhaRecordes* pilha)
{
    for (i = 0; i < pilha->quantidade - 1; i++)
    {
        for (j = 0; j < pilha->quantidade - i - 1; j++)
        {
            if (pilha->recordes[j].tempo > pilha->recordes[j + 1].tempo)
            {
                Recorde temp = pilha->recordes[j];
                pilha->recordes[j] = pilha->recordes[j + 1];
                pilha->recordes[j + 1] = temp;
            }
        }
    }
}

void inicializarPilhaRecordes(PilhaRecordes* pilha)
{
    pilha->quantidade = 0;
}

void carregarRecordes(PilhaRecordes* pilha)
{
    FILE* arquivo = fopen("recordes.txt", "rb");
    if (arquivo == NULL)
    {
        return;
    }
    Recorde recorde;
    while (fread(&recorde, sizeof(Recorde), 1, arquivo) == 1)
    {
        if (pilha->quantidade < MAX_RECORDS)
        {
            pilha->recordes[pilha->quantidade] = recorde;
            pilha->quantidade++;
        }
        else
        {
            break;
        }
    }
    fclose(arquivo);
}

void salvarRecordes(PilhaRecordes* pilha)
{
    FILE* arquivo = fopen("recordes.txt", "wb");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo de recordes para escrita.\n");
        return;
    }
    for (i = 0; i < pilha->quantidade; i++)
    {
        fwrite(&pilha->recordes[i], sizeof(Recorde), 1, arquivo);
    }
    fclose(arquivo);
}

void exibirRecordes(PilhaRecordes* pilha)
{
    printf("\nRecordes:\n");
    if (pilha->quantidade == 0)
    {
        printf("Nenhum recorde encontrado.\n");
    }
    else
    {
        ordenaRecordes(pilha);
        for (i = 0; i < pilha->quantidade; i++)
        {
            printf("%d. Nome: %s\n", i + 1, pilha->recordes[i].nome);
            printf("   Tempo: %.2f segundos\n", pilha->recordes[i].tempo);
        }
    }
    printf("\n");
}

void salvarPontuacao(double tempo_decorrido, PilhaRecordes* pilha)
{
    char nome[50];
    system("cls");
    printf("Parabens! Voce entrou para os recordes!\n");
    printf("Qual o seu nome? ");

    fflush(stdin);
    fgets(nome, sizeof(nome), stdin); 
    nome[strcspn(nome, "\n")] = 0;

    Recorde novo_recorde;
    strcpy(novo_recorde.nome, nome);
    novo_recorde.tempo = tempo_decorrido;

    if (pilha->quantidade < MAX_RECORDS)
    {
        pilha->recordes[pilha->quantidade] = novo_recorde;
        pilha->quantidade++;
    }
    else
    {
        ordenaRecordes(pilha);
        if (tempo_decorrido < pilha->recordes[MAX_RECORDS - 1].tempo) {
            pilha->recordes[MAX_RECORDS - 1] = novo_recorde;
        }
    }
    ordenaRecordes(pilha);
    salvarRecordes(pilha);
    printf("\nRegistro de recorde adicionado com sucesso!\n");
    printf("Pressione qualquer tecla para continuar...");
    getch();
}

typedef struct {
    int x;
    int y;
} Posicao;

void salvarTrajetoJogo(Posicao trajeto[], int contador) {
    FILE* arquivo = fopen("trajeto_jogo.txt", "w");
    if (arquivo != NULL) {
        for (i = 0; i < contador; i++) {
            fprintf(arquivo, "%d,%d\n", trajeto[i].x, trajeto[i].y);
        }
        fclose(arquivo);
    }
}

void posicao(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int verificarColisaoObstaculo(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    char buffer[1];
    DWORD charsRead;
    COORD coord = { (SHORT)x, (SHORT)y };

    // Lê 1 caractere da tela na coordenada especificada
    if (ReadConsoleOutputCharacter(hConsole, (LPTSTR)buffer, 1, coord, &charsRead)) {
        // Se o caractere lido for um obstáculo '#', retorna 1 (colisão)
        if (buffer[0] == '#') {
            return 1;
        }
    }
    // Caso contrário, retorna 0 (sem colisão)
    return 0;
}

int carregarMapaDeArquivo(const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        posicao(10, 5);
        printf("ERRO: Nao foi possivel abrir o arquivo do mapa: %s\n", nomeArquivo);
        posicao(10, 6);
        printf("Verifique se o arquivo esta na mesma pasta do executavel.");
        posicao(10, 8);
        printf("Pressione qualquer tecla para voltar ao menu...");
        getch();
        return 0;
    }
    char linha[256];
    for (i = 0; i < altura; i++) {
        if (fgets(linha, sizeof(linha), arquivo) != NULL) {
            posicao(0, i);
            printf("%s", linha);
        }
    }
    fclose(arquivo);
    return 1;
}

void gerarMapaProcedural() {
    // Cria um mapa local para ser modificado
    char mapa[altura][largura + 1]; // +1 para o caractere nulo '\0'
    int x, y;

    // Preenche o mapa inteiro com paredes sólidas
    for (y = 0; y < altura; y++) {
        for (x = 0; x < largura; x++) {
            mapa[y][x] = '#';
        }
    }

    // Lógica do "Escavador" (Random Walk)
    srand(time(NULL)); // Garante que a aleatoriedade seja diferente a cada execução
    int walkerX = largura / 2;
    int walkerY = altura / 2;
    
    // Define quantos passos o escavador dará. Mais passos = mapa mais aberto.
    int numPassos = (largura * altura) / 0.25; 

    for (i = 0; i < numPassos; i++) {
        // Escava a posição atual
        mapa[walkerY][walkerX] = ' ';

        // Escolhe uma direção aleatória (0:Norte, 1:Sul, 2:Leste, 3:Oeste)
        int direcaoAleatoria = rand() % 4;
        
        switch(direcaoAleatoria) {
            case 0: walkerY--; break; // Cima
            case 1: walkerY++; break; // Baixo
            case 2: walkerX++; break; // Direita
            case 3: walkerX--; break; // Esquerda
        }

        // Garante que o escavador não saia das bordas do mapa
        if (walkerX < 1) walkerX = 1;
        if (walkerX > largura - 2) walkerX = largura - 2;
        if (walkerY < 1) walkerY = 1;
        if (walkerY > altura - 2) walkerY = altura - 2;
    }
    
    // 3. Garante que a área inicial da cobra esteja limpa
    mapa[altura / 2][largura / 2] = ' ';

    // 4. Desenha o mapa gerado na tela
    system("cls");
    for (y = 0; y < altura; y++) {
        posicao(0, y);
        mapa[y][largura] = '\0'; // Adiciona o terminador nulo para imprimir como string
        printf("%s", mapa[y]);
    }
}

int gameOver(int tam, int x[], int y[])
{
    for(i = 3; i < tam; i++)
    {
        if(x[0] == x[i] && y[0] == y[i])
        {
            posicao((largura / 2) - 4, altura / 2);
            printf("GAME OVER");
            getch();
            return 1;
        }
    }
    return 0;
}

void snake(int x[100], int y[100], int tam)
{
    posicao(x[1], y[1]);
    printf("%c", 'O');
    for(i = 2; i < tam; i++)
    {
        posicao(x[i], y[i]);
        printf("%c", 219);
    }
    for(i = tam; i > 1; i--)
    {
        x[i] = x[i-1];
        y[i] = y[i-1];
    }
}

void refazerTrajetoJogo() {
    FILE* arquivo = fopen("trajeto_jogo.txt", "r");
    if (arquivo == NULL) {
        printf("Nenhum trajeto salvo para exibir. Pressione qualquer tecla para voltar.\n");
        getch();
        return;
    }
    system("cls");
    printf("Reexibindo ultimo trajeto...");
    Sleep(1000);
    system("cls");
    Posicao p;
    while (fscanf(arquivo, "%d,%d\n", &p.x, &p.y) == 2) {
        posicao(p.x, p.y);
        printf("%c", 'O');
        Sleep(50);
        posicao(p.x, p.y);
        printf(" ");
    }
    fclose(arquivo);
    printf("\n\nFim do trajeto. Pressione qualquer tecla para voltar ao menu.");
    getch();
}

char tecla(char direcao)
{
    if(kbhit())
    {
        char input = getch();
        switch (input)
        {
            case 72: case 'w': if(direcao != 's') direcao = 'n'; break;
            case 80: case 's': if(direcao != 'n') direcao = 's'; break;
            case 75: case 'a': if(direcao != 'l') direcao = 'o'; break;
            case 77: case 'd': if(direcao != 'o') direcao = 'l'; break;
        }
    }
    return direcao;
}

void maca(int mx, int my)
{
    posicao(mx, my);
    printf("%c", '@');
}

void pontuacao(int pontos)
{
    posicao(largura - 13, 1);
    printf("Pontos: %d", pontos);
}

double calculaTempo(time_t start_time)
{
    double total = (double)(clock() - start_time) / CLOCKS_PER_SEC;
    posicao(largura - 30, 1);
    printf("Tempo: %.2f", total);
    return total;
}

void menuDificuldade() {
    system("cls");
    posicao(10, 1);
    printf("J O G O  D A  C O B R I N H A\n\n");
    printf("\tSelecione o nivel de dificuldade:\n\n");
    printf("\t[1] Muito Facil (Mapa 1)\n");
    printf("\t[2] Facil       (Mapa 2)\n");
    printf("\t[3] Medio       (Mapa 3)\n");
    printf("\t[4] Dificil     (Mapa 4)\n");
    printf("\t[5] Expert      (Mapa 5)\n");
    printf("\t[6] Insano  (Mapa Aleatorio)\n\n");
    printf("\tSua escolha: ");

    char input = getch();
    switch (input) {
        case '1':
            velocidade = 125;
            nivelMapa = 1;
            break;
        case '2':
            velocidade = 100;
            nivelMapa = 2;
            break;
        case '3':
            velocidade = 50;
            nivelMapa = 3;
            break;
        case '4':
            velocidade = 25;
            nivelMapa = 4;
            break;
        case '5':
            velocidade = 15;
            nivelMapa = 5;
            break;
        case '6':
            velocidade = 75; // Uma velocidade intermediária para o modo procedural
            nivelMapa = 6;
            break;
        default: break;
    }
}

void menuComoJogar() {
    system("cls");
    posicao(10, 1);
    printf("J O G O  D A  C O B R I N H A\n\n");
    printf("\tINSTRUCOES:\n");
    printf("\t- O objetivo e comer 10 macas no menor tempo possivel!\n"); // <-- ALTERADO
    printf("\t- Se a cobra bater nela mesma, o jogo acaba!\n\n");
    printf("\tCONTROLES:\n");
    printf("\t- Use as setas do teclado ou as teclas 'W', 'A', 'S', 'D' para mover.\n\n\n");
    printf("\tPressione qualquer tecla para voltar!");
    getch();   
}

void exibirMenu(PilhaRecordes* recordes)
{
    char menus[6][100] = {
        "Jogar", "Ultimo Trajeto", "Recordes",
        "Dificuldade", "Como jogar?", "Sair"
    };
    int opcao = 0;
    int opcaoAnterior = -1;
    int count;
    while(1)
    {
        if (opcao != opcaoAnterior)
        {
            system("cls");
            posicao(10, 1);
            printf("J O G O  D A  C O B R I N H A\n\n\n");
            for (count = 0; count < 6; count++)
            {
                printf("\t%s %s\n", (opcao == count) ? "->" : "  ", menus[count]);
            }
            posicao((largura / 2) - 23, altura - 4);
            printf("ALUNOS: Caio Renan, Pedro Jose, Victor Holanda, Rafael Rios");
            opcaoAnterior = opcao;
        }
        if(kbhit())
        {
            char input = getch();
            switch (input)
            {
                case 72: if(opcao > 0) opcao--; else opcao = 5; break;
                case 80: if(opcao < 5) opcao++; else opcao = 0; break;
                case 13:
                    switch(opcao)
                    {
                        case 0: return;
                        case 1: refazerTrajetoJogo(); opcaoAnterior = -1; break;
                        case 2: system("cls"); exibirRecordes(recordes); printf("Pressione qualquer tecla para voltar..."); getch(); opcaoAnterior = -1; break;
                        case 3: menuDificuldade(); opcaoAnterior = -1; break;
                        case 4: menuComoJogar(); opcaoAnterior = -1; break;
                        case 5: exit(0);
                    }
                    break;
            }
        }
        Sleep(100);
    }
}

int main() {
    srand(time(NULL));

    PilhaRecordes recordes;
    inicializarPilhaRecordes(&recordes);
    carregarRecordes(&recordes);
    
    int tamanhoTrajeto = 100000;
    Posicao* trajeto = (Posicao*)malloc(tamanhoTrajeto * sizeof(Posicao));
    if (trajeto == NULL) {
        printf("Erro fatal ao alocar memoria para o trajeto.\n");
        return 1;
    }
    
    int* x = (int*)malloc(100 * sizeof(int));
    int* y = (int*)malloc(100 * sizeof(int));
    if (x == NULL || y == NULL) {
        printf("Erro fatal ao alocar memoria para a cobra.\n");
        free(trajeto);
        return 1;
    }

    while (1)
    {
        exibirMenu(&recordes);
        
        system("cls");

         if (nivelMapa == 6) {
            // Se for o nível 6, gera um mapa procedural em vez de carregar um arquivo
            gerarMapaProcedural();
        } else {
            // Para todos os outros níveis, carrega o mapa do arquivo correspondente
            char nomeMapa[20];
            sprintf(nomeMapa, "mapa%d.txt", nivelMapa); 
            if (!carregarMapaDeArquivo(nomeMapa)) {
                continue;
            }
        }

        int tam = 5, pontos = 0, over = 0, contadorTrajeto = 0;
        char direcao = 'l';
        int mx, my;

        x[0] = largura / 2;
        y[0] = altura / 2;
        x[1] = x[0];
        y[1] = y[0];

        // Loop para garantir que a maçã não apareça em um obstáculo
        do {
            mx = (rand() % (largura - 4)) + 2;
            my = (rand() % (altura - 4)) + 3;
        } while (verificarColisaoObstaculo(mx, my)); // Repete se mx,my for um obstáculo '#'

        maca(mx, my);

        time_t start_time = clock();

        while(!over)
        {
            if (contadorTrajeto < tamanhoTrajeto) {
                trajeto[contadorTrajeto].x = x[1];
                trajeto[contadorTrajeto].y = y[1];
                contadorTrajeto++;
            }

            pontuacao(pontos);
            double tempo_decorrido = calculaTempo(start_time);

            posicao(x[tam], y[tam]);
            printf(" ");

            direcao = tecla(direcao);

             switch(direcao)
            {
                case 'n': 
                    y[1]--; 
                    if(y[1] <= 2) over = 1;
                    break;
                case 's': 
                    y[1]++; 
                    if(y[1] >= altura - 1) over = 1;
                    break;
                case 'l': 
                    x[1]++; 
                    if(x[1] >= largura - 2) over = 1;
                    break;
                case 'o': 
                    x[1]--; 
                    if(x[1] <= 1) over = 1;
                    break;
            }

            // --- NOVA VERIFICAÇÃO DE COLISÃO COM OBSTÁCULO ---
            // Verifica se a nova posição da cabeça (x[1], y[1]) é um '#'
            if (verificarColisaoObstaculo(x[1], y[1])) {
                over = 1; // Se for, o jogo acaba
            }
        

            snake(x, y, tam);

            x[0] = x[1];
            y[0] = y[1];

            if(x[1] == mx && y[1] == my)
            {
                // Loop para garantir que a NOVA maçã não apareça em um obstáculo
                do {
                    mx = (rand() % (largura - 4)) + 2;
                    my = (rand() % (altura - 4)) + 3;
                } while (verificarColisaoObstaculo(mx, my)); // Repete se mx,my for um obstáculo '#'

                maca(mx, my);
                pontos++;
                tam++;
            }

            // <-- ALTERADO
            if(tam == 15)
            {
                posicao((largura / 2) - 15, altura / 2);
                printf("PARABENS! VOCE CONCLUIU O JOGO!");
                posicao((largura / 2) - 15, (altura / 2) + 2);
                printf("Seu tempo foi de: %.2f segundos", tempo_decorrido);
                getch();
                salvarPontuacao(tempo_decorrido, &recordes);
                over = 1;
            }
            over = over || gameOver(tam, x, y);
            Sleep(velocidade);
        }
        salvarTrajetoJogo(trajeto, contadorTrajeto);
    }

    free(trajeto);
    free(x);
    free(y);
    return 0;
}
