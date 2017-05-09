/*

   PROJETO --> PACIÊNCIA
   
   Desenvolvedores: RODRIGO SANTOS MENEZES
                    PEDRO AUGUSTO DA CAMARA DE OLIVEIRA
   
   Pofessor: PEDRO HENRIQUE
   Disciplina: Estrutura de dados e arquivos
   Turma: 5G5H
                    
   Objetivo: Construir um jogo de paciência usando manipulação de pilhas 
   
   Desenvolvimento do Projeto
     
     -> O projeto foi desenvolvido usando-se da linguagem c em conjunto com 
     a biblioteca da Microsoft, windows.h, que é responsavel pela manipulação
     de eventos de teclado e mouse usados no programa.
         O jogo possui sistema de regras semelhante às presentes no jogo para 
     Windows.
         A movimento das cartas é efetuado aplicando-se um clique duplo sobre
     a carta que deseja-se movimentar, em seguida o jogador deve aplicar um 
     clique duplo sobre a plilha onde a carta deverá ser colocada.
         No topo da janela é possível visualizar 3 opções, são elas: novo jogo,
     ajuda e sair. Essas são acessadas através das teclas de atalho informadas
     também no topo da janela.
         No rodapé da janela, está vísivel uma barra que mostra o status das a-
     ções do jogador.

*/


 //=============================================================================
//BIBLIOTECAS UTILIZADAS & DEFINES
#include <stdio.h>
#include <conio.c>
#include <time.h>
#include <windows.h>
#define MENU "F1 - AJUDA                    F2 - NOVO JOGO                      ESC - SAIR    "
#define MSG  "                                                                                "
#define PCIMA 1
#define PBAIXO 2

 //=============================================================================
//ESTRUTURA BASICA DAS CARTAS

typedef struct 
{
   int linha;
   int coluna;
   char valCarta;
   char naipe;
   int posicao;  
}cartas;

 //=============================================================================
//ESTRUTURA BÁSICA DAS PILHAS 

struct pilhaPrincipal 
{
   cartas carta;
   struct pilhaPrincipal *link;
};  

typedef struct pilhaPrincipal pilhaP;
int codPilhaAux;
 //=============================================================================
// DECLARAÇÃO DAS PILHAS DO JOGO

pilhaP *topo  = NULL;
pilhaP *topo1 = NULL;
pilhaP *topo2 = NULL;
pilhaP *topo3 = NULL;
pilhaP *topo4 = NULL;
pilhaP *topo5 = NULL;
pilhaP *topo6 = NULL;
pilhaP *topo7 = NULL;
pilhaP *topo8 = NULL;
pilhaP *topo9 = NULL;
pilhaP *topo10 = NULL;
pilhaP *topo11 = NULL;
pilhaP *topo12 = NULL;
pilhaP *topoAux = NULL;
pilhaP *pilhaTemp = NULL;

 //=============================================================================
//VARIÁVEL CARTA PARA A MANIPULAÇÃO DAS CARTAS NO JOGO
int selecionou;
cartas CARTA;
cartas CARTA2;
 //=============================================================================
//GERENCIAMENTO DE EVENTOS

void MouseEventProc(MOUSE_EVENT_RECORD,HANDLE *hStdin, DWORD *fdwMode,int *novoJogo);
VOID ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD);
VOID KeyEventProc(KEY_EVENT_RECORD,int *novoJogo,HANDLE *hStdin, DWORD *fdwMode);
VOID GetInputEvents(VOID);
void gotoxy2(int x, int y);

 //=============================================================================
//FUNÇÕES PILHA

void push(cartas *CARTA, int codPilha);
void pop(cartas *CARTA, int codPilha);
void acessoPilha();
void organizaPilhasIniciais();
void esvaziaPilhas();
int  contaElementos(pilhaP *TOPO);
int posicaoPilha(int x, int y);
int verificaMovimento(pilhaP temp);
void varrePilhas(void);

 //=============================================================================
//FUNÇÕES DAS CARTAS

void desenhoCarta(int col,int linha,char valCarta,char naipe,int posicao);
void embaralha(void);
char getNaipe(int i);
void desenhaAreaNaipes();
int posicaoCartaY(int x, int y);
int posicaoCartaX(int x, int y);
int moveCartas(int codPilha, int posCarta);
int moveCartasDestino(int codPilha);
void moveTempOrigem();
void mostraLayout();
void verificaFimJogo(int *novoJogo, HANDLE *hStdin, DWORD *fdwMode);

 //=============================================================================
//FUNÇÃO MAIN

int main(void)
{
// Declaracoes

   pilhaP *topoTemp = NULL;
   cartas cartaTemp;
   int i,j,novoJogo;
   HANDLE hStdin;
   DWORD cNumRead, fdwMode, k;
   INPUT_RECORD irInBuf[128];
// Instrucoes
   system("TITLE PACIENCIA");
   do{
      textcolor(BLACK);
      textbackground(GREEN);
      system("cls");
      textbackground(WHITE);
      printf(MENU);
      gotoxy(1,50);
      printf(MSG);
      gotoxy(1,50);
      printf("JOGO INICIADO! BEM-VINDO!");
      gotoxy(1,1);
      textbackground(GREEN);   
      desenhaAreaNaipes();
      embaralha();
      organizaPilhasIniciais();
      gotoxy(2,2);
      acessoPilha();
      
      // Obtém o controlador padrão de entrada
      hStdin = GetStdHandle(STD_INPUT_HANDLE);
      
      // Ativa os eventos de entrada da janela e do mouse.
      fdwMode = ENABLE_MOUSE_INPUT;
      SetConsoleMode(hStdin, fdwMode);

      // Laço para leitura e controle da entrada de eventos
      novoJogo = 1;
      while (novoJogo == 1)
      {
      // Espera pelo evento
         ReadConsoleInput(
                           hStdin, // buffer do controlador de entrada
                           irInBuf, // buffer para a leitura
                           128, // tamanho do buffer de leitura
                           &cNumRead  // número de gravações lidas
                         );
        
      // Envia o evento para o controlador apropriado.
         for (k = 0; k < cNumRead; k++)
         {
            switch(irInBuf[k].EventType)
            {
               case KEY_EVENT: // Entrada do teclado
                  KeyEventProc(irInBuf[k].Event.KeyEvent, &novoJogo,&hStdin,&fdwMode);
                  break;

               case MOUSE_EVENT: // Entrada do mouse
                  MouseEventProc(irInBuf[k].Event.MouseEvent,&hStdin,&fdwMode,&novoJogo);
                  break;

               case FOCUS_EVENT: //Ignorar evento de focus

               case MENU_EVENT: //Ignorar evento de menu
                  break;
            }
         }
      }
   }while(1);   
   return 0;
}

/* Função que desenha as cartas de acordo com o naipe, valor, posição (virada
para cima ou para baixo) e localização na tela (coluna e linha) */
void desenhoCarta(int col,int linha,char valCarta,char naipe, int posicao)
{
   int i;
   
   for(i = 1;i<8;i++)//Parte interna da carta
   {
      textbackground(WHITE);
      gotoxy(col+1,linha+i);
      printf("        ");
   }
   if(posicao == PBAIXO)    
   {
      for(i = 1;i<8;i++)
      {
         textbackground(WHITE);
         gotoxy(col+1,linha+i);
         printf("\xb2\xb2\xb2\xb2\xb2\xb2\xb2\xb2");
      }
   }
   else
   {  
      switch(naipe)//Escrita de naipe e número da carta
      {
         case '\x5':
              textcolor(BLACK);
              gotoxy(col+1,linha+1);
              if(valCarta != 'D')
              {
                 printf("%c\x5",valCarta);
                 gotoxy(col+6,linha+7);
                 printf("%c\x5",valCarta);            
              }
              else
              {
                 printf("10\x5");
                 gotoxy(col+5,linha+7);
                 printf("10\x5");                       
              }
              break;
         case '\x4':
              textcolor(LIGHTRED);
              gotoxy(col+1,linha+1);
              if(valCarta != 'D')
              {
                 printf("%c\x4",valCarta);
                 gotoxy(col+6,linha+7);
                 printf("%c\x4",valCarta);            
              }
              else
              {
                 printf("10\x4");
                 gotoxy(col+5,linha+7);
                 printf("10\x4");                       
              }
              break;
         case '\x6':
              textcolor(BLACK);
              gotoxy(col+1,linha+1);
              if(valCarta != 'D')
              {
                 printf("%c\x6",valCarta);
                 gotoxy(col+6,linha+7);
                 printf("%c\x6",valCarta);            
              }
              else
              {
                 printf("10\x6");
                 gotoxy(col+5,linha+7);
                 printf("10\x6");                       
              }
              break;
         case '\x3':
              textcolor(LIGHTRED);
              gotoxy(col+1,linha+1);
              if(valCarta != 'D')
              {
                 printf("%c\x3",valCarta);
                 gotoxy(col+6,linha+7);
                 printf("%c\x3",valCarta);            
              }
              else
              {
                 printf("10\x3");
                 gotoxy(col+5,linha+7);
                 printf("10\x3");                       
              }
              break;
      }                                 
   }//Desenho da bordas das cartas
   textcolor(BLACK);
   gotoxy(col,linha);
   printf("\xDA");
   for(i=1;i<8;i++)//Desenhos das linhas superiores e inferiores
   {
      gotoxy(col+i,linha);
      printf("\xC4");
      gotoxy(col+i,linha+8);
      printf("\xC4");       
   }
   gotoxy(col+8,linha);
   printf("\xBF");
   for(i = 1;i<8;i++)//Desenhos das colunas esquerdas e direitas
   {
      gotoxy(col,linha+i);
      printf("\xB3");
      gotoxy(col+8,linha+i);
      printf("\xB3");  
   }
   gotoxy(col,linha+8);
   printf("\xC0"); 
   gotoxy(col+8,linha+8);
   printf("\xD9");
   //getch();
}

// ????????????????????????
/* Denha a área das cartas  */
void desenhaAreaNaipes()
{
   int col = 35,linha = 4, i,j;
   for(j = 0; j< 4; j++)
   {
      textcolor(BLACK);
      for(i = 1;i<8;i++)
      {
         textbackground(GREEN);
         gotoxy(col+1,linha+i);
         printf("\xb1\xb1\xb1\xb1\xb1\xb1\xb1\xb1");
      }
      gotoxy(col,linha);
      printf("\xDA");
      for(i=1;i<8;i++)//Desenhos das linhas superiores e inferiores
      {
         gotoxy(col+i,linha);
         printf("\xC4");
         gotoxy(col+i,linha+8);
         printf("\xC4");       
      }
      gotoxy(col+8,linha);
      printf("\xBF");
      for(i = 1;i<8;i++)//Desenhos das colunas esquerdas e direitas
      {
         gotoxy(col,linha+i);
         printf("\xB3");
         gotoxy(col+8,linha+i);
         printf("\xB3");  
      }
      gotoxy(col,linha+8);
      printf("\xC0"); 
      gotoxy(col+8,linha+8);
      printf("\xD9");
      switch(j)
      {
         case 0:
            gotoxy(col+4,linha+4);
            printf("\x5");
            break;
         case 1:
            gotoxy(col+4,linha+4);
            printf("\x6");
            break;
         case 2:
            textcolor(RED);
            gotoxy(col+4,linha+4);
            printf("\x4");
            break;
         case 3:
            textcolor(RED);            
            gotoxy(col+4,linha+4);
            printf("\x3");
            break;
      }
      textcolor(BLACK);
      col = col + 10;
   }
}

/* Função que embaralha as cartas e armazena as mesmas na memória: de acordo com 
um n° sorteado, que vai de 1 a 52, o vetor cartas[n° sorteado] vai receber 
valores e naipes diferentes. Esse processo continua até que todas as posições do
vetor cartas[1..52] sejam visitados */
void embaralha(void)
{
       char cartas [52] = {'A','2','3','4','5','6','7','8','9','D','J','Q','K',
                           'A','2','3','4','5','6','7','8','9','D','J','Q','K',
                           'A','2','3','4','5','6','7','8','9','D','J','Q','K',
                           'A','2','3','4','5','6','7','8','9','D','J','Q','K'};
       int i, continua,j,k = 0;
       
       srand(time(NULL));
       i = rand()%52;
       do
       {
          if(cartas[i] != (char)NULL)
          {
             k = k+2;
             CARTA.linha = 4;
             CARTA.coluna = 5;
             CARTA.valCarta = cartas[i];
             CARTA.naipe = getNaipe(i);
             CARTA.posicao = PBAIXO;
             push(&CARTA,0);  
          }
          cartas[i] = (char)NULL;
          i = rand()%52;
          continua = 0;
          for(j = 0; j<52; j++)
          {
             if(cartas[j]!=(char)NULL)
             {
               continua = 1;
               break;
             }  
          }
          
       }while(continua);
       
}

/* Retorna o naipe de acordo com a posição do vetor carta[], usado na função 
"embaralha()" */
char getNaipe(int i)
{
   if(i<13)
      return '\x3';
   if(i>12 && i<26)
      return '\x4';
   if(i>25 && i<39)
      return '\x5';
   if(i>38 && i<52)
      return '\x6';  
}

/* Conta o n° de elementos que tem em determinada pilha e retorna esse n° */
int contaElementos(pilhaP *TOPO)
{
   pilhaP *temp;
   int i = 0;
   
   temp = TOPO;
   while(temp != NULL)
   {
      i++;
      temp = temp->link;
   }
   return i;
}

/* Aloca na memória as pilhas de acordo com seu código: cada código, que varia de
0 à 13, representa uma pilha diferente */
void push(cartas *CARTA, int codPilha)
{
   struct pilhaPrincipal *temp;
   
   temp = (pilhaP *) malloc(sizeof(pilhaP));
   if(!temp)
   {
      puts("Nao foi possivel alocar memoria.");
      getch();
      exit(1);
   }
   
   switch (codPilha)
   {
      case 0:
         temp->link = topo;
         temp->carta = *CARTA;
         topo = temp;
      break;
      case 1:
         temp->link = topo1;
         temp->carta = *CARTA;
         topo1 = temp;
      break;
      case 2:
         temp->link = topo2;
         temp->carta = *CARTA;
         topo2 = temp;
      break;
      case 3:
         temp->link = topo3;
         temp->carta = *CARTA;
         topo3 = temp;
      break;
      case 4:
         temp->link = topo4;
         temp->carta = *CARTA;
         topo4 = temp;
      break;
      case 5:
         temp->link = topo5;
         temp->carta = *CARTA;
         topo5 = temp;
      break;
      case 6:
         temp->link = topo6;
         temp->carta = *CARTA;
         topo6 = temp;
      break;
      case 7:
         temp->link = topo7;            
         temp->carta = *CARTA;
         topo7 = temp;
      break;
      case 8:
         temp->link = topo8;            
         temp->carta = *CARTA;
         topo8 = temp;
      break;
      case 9:
         temp->link = topo9;            
         temp->carta = *CARTA;
         topo9 = temp;
      break;
      case 10:
         temp->link = topo10;            
         temp->carta = *CARTA;
         topo10 = temp;
      break;
      case 11:
         temp->link = topo11;            
         temp->carta = *CARTA;
         topo11 = temp;
      break;
      case 12:
         temp->link = topo12;            
         temp->carta = *CARTA;
         topo12 = temp;
      break;
      case 13:
         temp->link = pilhaTemp;            
         temp->carta = *CARTA;
         pilhaTemp = temp;
      break;

   }
   
}

/* Retira elementos de cada pilha usando-se seu código, que varia de 0 à 13 e cada
código representa uma pilha diferente, e o tranfere para uma pilha temporário a fim
de facilitar o manuseio dos dados */
void pop(cartas *CARTA, int codPilha)
{
   
   pilhaP *temp;
   cartas tCARTA;
   int i,j;
   
   switch (codPilha)
   {
      case 0:   
         if (topo == NULL)
         {
            j = contaElementos(topo8);
            printf("%d",j);
            for (i = 0; i < j; i++)
            {
               pop(&tCARTA,8);
               push(&tCARTA,0);
               
            }
         }
         *CARTA = topo->carta;
         temp = topo;
         topo = topo->link;
      break;
      case 1:
         if (topo1 == NULL)
         {
            puts("Pilha vazia");
            getch();
            return;
         }
         *CARTA = topo1->carta;
         temp = topo1;
         topo1 = topo1->link;
      break;
      case 2:
         if (topo2 == NULL)
         {
            puts("Pilha vazia");
            getch();
            return;
         }
         *CARTA = topo2->carta;
         temp = topo2;
         topo2 = topo2->link;
      break;
      case 3:
         if (topo3 == NULL)
         {
            puts("Pilha vazia");
            getch();
            return;
         }
         *CARTA = topo3->carta;
         temp = topo3;
         topo3 = topo3->link;
      break;
      case 4:
         if (topo4 == NULL)
         {
            puts("Pilha vazia");
            getch();
            return;
         }
         *CARTA = topo4->carta;
         temp = topo4;
         topo4 = topo4->link;
      break;
      case 5:
         if (topo5 == NULL)
         {
            puts("Pilha vazia");
            getch();
            return;
         }
         *CARTA = topo5->carta;
         temp = topo5;
         topo5 = topo5->link;
      break;
      case 6:
         if (topo6 == NULL)
         {
            puts("Pilha vazia");
            getch();
            return;
         }
         *CARTA = topo6->carta;
         temp = topo6;
         topo6 = topo6->link;
      break;
      case 7:
         if (topo7 == NULL)
         {
            puts("Pilha vazia");
            getch();
            return;
         }
         *CARTA = topo7->carta;
         temp = topo7;
         topo7 = topo7->link;
         break;
      case 8:
         if (topo8 == NULL)
         {
            puts("Pilha vazia");
            getch();
            return;
         }
         *CARTA = topo8->carta;
         temp = topo8;
         topo8 = topo8->link;
         break;
      case 9:
         if (topo9 == NULL)
         {
            puts("Pilha vazia");
            getch();
            return;
         }
         *CARTA = topo9->carta;
         temp = topo9;
         topo9 = topo9->link;
         break;
      case 10:
         if (topo10 == NULL)
         {
            puts("Pilha vazia");
            getch();
            return;
         }
         *CARTA = topo10->carta;
         temp = topo10;
         topo10 = topo10->link;
         break;
      case 11:
         if (topo11 == NULL)
         {
            puts("Pilha vazia");
            getch();
            return;
         }
         *CARTA = topo11->carta;
         temp = topo11;
         topo11 = topo11->link;
         break;
      case 12:
         if (topo12 == NULL)
         {
            puts("Pilha vazia");
            getch();
            return;
         }
         *CARTA = topo12->carta;
         temp = topo12;
         topo12 = topo12->link;
         break;
      case 13:
         if (pilhaTemp == NULL)
         {
            puts("Pilha vazia");
            getch();
            return;
         }
         *CARTA = pilhaTemp->carta;
         temp = pilhaTemp;
         pilhaTemp = pilhaTemp->link;
         break;
   }
   
   free(temp);
   
}

/* Função que auxilia ao desenhar as cartas, alocando na memória as cartas nas 
devidas posições, que ficam erradas quando se dar push em uma pilha. Pois os topos
ficam invertidos, dificultando o processo de desenhar as cartas corretamente */
void popTopoAux(cartas *CARTA)
{
   pilhaP *temp;
   
   if (topoAux == NULL)
   {
      puts("Pilha vazia");
      getch();
      return;
   }
   *CARTA = topoAux->carta;
   temp = topoAux;
   topoAux = topoAux->link;
   
   free(temp);
}

/* Aloca numa pilha temporária os elementos de uma outra pilha, de forma que o o topo
mude a sua extremidade a fim de se facilitar o processo de desenhar as cartas */
void pushTopoAux(cartas CARTA)
{
   pilhaP *temp;
   
   temp = (pilhaP *) malloc(sizeof(pilhaP));
   if(!temp)
   {
      puts("Nao foi possivel alocar memoria.");
      getch();
      exit(1);
   }
   temp->link = topoAux;
   temp->carta = CARTA;
   topoAux = temp;
      
}

/* Distribue e desenha as pilhas no inicio do jogo */
void acessoPilha()
{
   int i,j,k;
   pilhaP *temp,*temp2,*topoSeguro = topo;
   cartas cartaTemp;
   for(i = 0; i<13; i++)
   {   
      switch(i)
      {
         case 0:
            temp = topo;
            while(temp != NULL)
            {
               cartaTemp = temp->carta;
               desenhoCarta(cartaTemp.coluna,cartaTemp.linha,cartaTemp.valCarta,cartaTemp.naipe,cartaTemp.posicao);
               temp = temp->link;
            }
            break;
         case 1:
            temp = topo1;
            for(j = 0; j<contaElementos(topo1); j++)
            {
               pushTopoAux(temp->carta);
               temp = temp->link;
            }
            for(j = 0; j<contaElementos(topo1); j++)
            {
               popTopoAux(&cartaTemp);
               desenhoCarta(cartaTemp.coluna,cartaTemp.linha,cartaTemp.valCarta,cartaTemp.naipe,cartaTemp.posicao);
            }
            break;
         case 2:
            gotoxy(40,5);
            temp = topo2;
            for(j = 0; j<contaElementos(topo2); j++)
            {
               pushTopoAux(temp->carta);
               temp = temp->link;
            }
            for(j = 0; j<contaElementos(topo2); j++)
            {
               popTopoAux(&cartaTemp);
               desenhoCarta(cartaTemp.coluna,cartaTemp.linha,cartaTemp.valCarta,cartaTemp.naipe,cartaTemp.posicao);
            }
            break;
         case 3:
            gotoxy(40,6);
            temp = topo3;
            for(j = 0; j<contaElementos(topo3); j++)
            {
               pushTopoAux(temp->carta);
               temp = temp->link;
            }
            for(j = 0; j<contaElementos(topo3); j++)
            {
               popTopoAux(&cartaTemp);
               desenhoCarta(cartaTemp.coluna,cartaTemp.linha,cartaTemp.valCarta,cartaTemp.naipe,cartaTemp.posicao);
            }
            break;
         case 4:
            temp = topo4;
            for(j = 0; j<contaElementos(topo4); j++)
            {
               pushTopoAux(temp->carta);
               temp = temp->link;
            }
            for(j = 0; j<contaElementos(topo4); j++)
            {
               popTopoAux(&cartaTemp);
               desenhoCarta(cartaTemp.coluna,cartaTemp.linha,cartaTemp.valCarta,cartaTemp.naipe,cartaTemp.posicao);
            }
            break;
         case 5:
            temp = topo5;
            for(j = 0; j<contaElementos(topo5); j++)
            {
               pushTopoAux(temp->carta);
               temp = temp->link;
            }
            for(j = 0; j<contaElementos(topo5); j++)
            {
               popTopoAux(&cartaTemp);
               desenhoCarta(cartaTemp.coluna,cartaTemp.linha,cartaTemp.valCarta,cartaTemp.naipe,cartaTemp.posicao);
            }
            break;
         case 6:
            temp = topo6;
            for(j = 0; j<contaElementos(topo6); j++)
            {
               pushTopoAux(temp->carta);
               temp = temp->link;
            }
            for(j = 0; j<contaElementos(topo6); j++)
            {
               popTopoAux(&cartaTemp);
               desenhoCarta(cartaTemp.coluna,cartaTemp.linha,cartaTemp.valCarta,cartaTemp.naipe,cartaTemp.posicao);
            }
            break;
         case 7:
            temp = topo7;
            for(j = 0; j<contaElementos(topo7); j++)
            {
               pushTopoAux(temp->carta);
               temp = temp->link;
            }
            for(j = 0; j<contaElementos(topo7); j++)
            {
               popTopoAux(&cartaTemp);
               desenhoCarta(cartaTemp.coluna,cartaTemp.linha,cartaTemp.valCarta,cartaTemp.naipe,cartaTemp.posicao);
            }
            break;
         case 8:
            temp = topo8;
            for(j = 0; j<contaElementos(topo8); j++)
            {
               pushTopoAux(temp->carta);
               temp = temp->link;
            }
            for(j = 0; j<contaElementos(topo8); j++)
            {
               popTopoAux(&cartaTemp);
               desenhoCarta(cartaTemp.coluna,cartaTemp.linha,cartaTemp.valCarta,cartaTemp.naipe,PCIMA);
            }
            break;
         case 9:
            temp = topo9;
            for(j = 0; j<contaElementos(topo9); j++)
            {
               pushTopoAux(temp->carta);
               temp = temp->link;
            }
            for(j = 0; j<contaElementos(topo9); j++)
            {
               popTopoAux(&cartaTemp);
               desenhoCarta(cartaTemp.coluna,cartaTemp.linha,cartaTemp.valCarta,cartaTemp.naipe,cartaTemp.posicao);
            }
            break;
         case 10:
            temp = topo10;
            for(j = 0; j<contaElementos(topo10); j++)
            {
               pushTopoAux(temp->carta);
               temp = temp->link;
            }
            for(j = 0; j<contaElementos(topo10); j++)
            {
               popTopoAux(&cartaTemp);
               desenhoCarta(cartaTemp.coluna,cartaTemp.linha,cartaTemp.valCarta,cartaTemp.naipe,cartaTemp.posicao);
            }
            break;
         case 11:
            temp = topo11;
            for(j = 0; j<contaElementos(topo11); j++)
            {
               pushTopoAux(temp->carta);
               temp = temp->link;
            }
            for(j = 0; j<contaElementos(topo11); j++)
            {
               popTopoAux(&cartaTemp);
               desenhoCarta(cartaTemp.coluna,cartaTemp.linha,cartaTemp.valCarta,cartaTemp.naipe,cartaTemp.posicao);
            }
            break;
         case 12:
            temp = topo12;
            for(j = 0; j<contaElementos(topo12); j++)
            {
               pushTopoAux(temp->carta);
               temp = temp->link;
            }
            for(j = 0; j<contaElementos(topo12); j++)
            {
               popTopoAux(&cartaTemp);
               desenhoCarta(cartaTemp.coluna,cartaTemp.linha,cartaTemp.valCarta,cartaTemp.naipe,cartaTemp.posicao);
            }
            break;
      }
   }
}

/* Organiza as pilhas iniciais, as que ficam em baixo. Variando-se o código da pilha,
representado por "i", varia-se também os valores que as posições das cartas irão receber
e em qual pilha vai ser armazenada os elementos da pilha principal. Todos os elementos
(cartas) que estão no final de cada pilha, receberão um inteiro que caracteriza o 
desenho da carta virada para cima */
void organizaPilhasIniciais()
{
   cartas cartaTemp;
   int i,j;
   
   for(i = 1; i<8 ;i++)
   {
      switch(i)
      {
         case 1:
            pop(&cartaTemp,0);
            cartaTemp.linha = 15;
            cartaTemp.coluna  = 5;
            push(&cartaTemp,i);
            break;
         case 2:
            for(j = 2; j>0; j--)
            {
               pop(&cartaTemp,0);
               cartaTemp.linha = 19 -(j*2);
               cartaTemp.coluna  = 5+(5*i);
               push(&cartaTemp,i);
            }
            break;
         case 3:
            for(j = 3;j>0;j--)
            {
               pop(&cartaTemp,0);
               cartaTemp.linha = 21 -(j*2);
               cartaTemp.coluna  = 10+(5*i);
               push(&cartaTemp,i);
            }
            break;      
         case 4:
            for(j = 4;j>0;j--)
            {
               pop(&cartaTemp,0);
               cartaTemp.linha = 23 -(j*2);
               cartaTemp.coluna  = 15+(5*i);
               push(&cartaTemp,i);
            }
            break;      
         case 5:
            for(j = 5;j>0;j--)
            {
               pop(&cartaTemp,0);
               cartaTemp.linha = 25 -(j*2);
               cartaTemp.coluna  = 20+(5*i);     
               push(&cartaTemp,i);
            }
            break;      
         case 6:
            for(j = 6;j>0;j--)
            {
               pop(&cartaTemp,0);
               cartaTemp.linha = 27 -(j*2);
               cartaTemp.coluna  = 25+(5*i);            
               push(&cartaTemp,i);
            }
            break;      
         case 7:
            for(j = 7;j>0;j--)
            {
               pop(&cartaTemp,0);
               cartaTemp.linha = 29 -(j*2);
               cartaTemp.coluna  = 30+(5*i);
               push(&cartaTemp,i);
            }
            break;      
      }
   }
   topo1->carta.posicao = PCIMA;
   topo2->carta.posicao = PCIMA;
   topo3->carta.posicao = PCIMA;
   topo4->carta.posicao = PCIMA;
   topo5->carta.posicao = PCIMA;
   topo6->carta.posicao = PCIMA;
   topo7->carta.posicao = PCIMA;
   
}

//Eventos do mouse
/* 
Esta função controla todos os eventos do mouse seja se movimento ou seu cliques
o controle da transição das carta de pilha para pilha é feito também por ela
*/
VOID MouseEventProc(MOUSE_EVENT_RECORD ir, HANDLE *hStdin, DWORD *fdwMode,int *novoJogo)
{   
   
   if (ir.dwEventFlags == MOUSE_MOVED)
      gotoxy2(ir.dwMousePosition.X,ir.dwMousePosition.Y);
   if(ir.dwEventFlags == MOUSEEVENTF_LEFTDOWN)
   {
      if (hotSpot(ir.dwMousePosition.X,ir.dwMousePosition.Y))
      {
         if(selecionou == 1)
            moveTempOrigem();
         if(!topo)
         {
            while(topo8!= NULL)
            {
               pop(&CARTA,8);
               CARTA.linha = 4;
               CARTA.coluna = 5;
               push(&CARTA,0);   
            }
         }
         pop(&CARTA,0);
         CARTA.linha = 4;
         CARTA.coluna = 15;
         push(&CARTA,8);
         varrePilhas();
         textbackground(GREEN);
         system("cls");
         textbackground(WHITE);
         gotoxy(1,1);
         printf(MENU);
         gotoxy(1,50);
         printf(MSG);
         gotoxy(1,50);
         printf("BARALHO ACIONADO!");
         gotoxy(1,1);
         desenhaAreaNaipes();
         acessoPilha();            
         SetConsoleMode(*hStdin, *fdwMode);
         selecionou = 0;
      }
      else
      {
         if(selecionou == 0)
         {
            if(moveCartas(posicaoCartaX(ir.dwMousePosition.X,ir.dwMousePosition.Y)
               ,posicaoCartaY(ir.dwMousePosition.X,ir.dwMousePosition.Y))== 0)
            {
               selecionou = 1;
               textbackground(WHITE);
               gotoxy(1,50);
               printf(MSG);
               gotoxy(1,50);
               printf("CARTA(s) SELECIONADA(s)");
               gotoxy(1,1);
            }
         }
         else
         {   
            if(selecionou == 1)
            {
               if(moveCartasDestino(posicaoCartaX(ir.dwMousePosition.X,ir.dwMousePosition.Y)) == 0)
               {
                  varrePilhas();
                  textbackground(GREEN);
                  system("cls");
                  textbackground(WHITE);
                  gotoxy(1,1);  
                  printf(MENU);
                  gotoxy(1,50);
                  printf(MSG);
                  gotoxy(1,50);
                  printf("MOVIMENTO EFETUADO!");
                  gotoxy(1,1);                  
                  desenhaAreaNaipes();
                  acessoPilha();            
                  gotoxy(2,2);
                  SetConsoleMode(*hStdin, *fdwMode);
                  selecionou = 0;
                  verificaFimJogo(novoJogo,hStdin,fdwMode);
               }
               else
               {
                  gotoxy(1,50);
                  printf(MSG);
                  gotoxy(1,50);
                  printf("MOVIMENTO INV\xB5LIDO!");
                  gotoxy(1,1);
                  selecionou = 0;
               }
            }
         }        
      }
   }  
}

//Eventos do teclado
/*
Este procedimento controla os eventos do teclado para as ações escolhidas
pelas teclas de atalho.
*/
VOID KeyEventProc(KEY_EVENT_RECORD dummy, int *novoJogo, HANDLE *hStdin, DWORD *fdwMode)
{
   if(dummy.wVirtualKeyCode == VK_ESCAPE)
   {
      gotoxy(73,2);
      printf("SAIR");
      gotoxy(0,0);
      exit(1);
   }
   if(dummy.wVirtualKeyCode == VK_F1)
   {
      gotoxy(6,2);
      gotoxy(1,50);
      printf(MSG);
      gotoxy(1,50);
      printf("AJUDA ACIONADA!");
      gotoxy(1,1);
      mostraLayout();
      textcolor(BLACK);
      getch();
      varrePilhas();
      textbackground(GREEN);
      system("cls");
      textbackground(WHITE);
      gotoxy(1,1);  
      printf(MENU);
      gotoxy(1,50);
      printf(MSG);
      gotoxy(1,50);
      printf("AJUDA ACIONADA!");
      gotoxy(1,1);                  
      desenhaAreaNaipes();
      acessoPilha();            
      gotoxy(2,2);
      SetConsoleMode(*hStdin, *fdwMode);
      selecionou = 0;
      gotoxy(0,0);
      return;   
   }
   if(dummy.wVirtualKeyCode == VK_F2)
   {
      gotoxy(21,2);
      printf("NOVO JOGO");
      selecionou = 0;
      esvaziaPilhas();
      *novoJogo = 0;
      gotoxy(0,0);
      return;
   }
}

//Função para a entrada de eventos faz-se necessária de acordo com a api da 
//windows.h
VOID GetInputEvents(VOID)
{
   return;
}

/*
Desenha o cursor do mause de acordo com o moviento do dispositivo
*/
void gotoxy2(int x, int y)
{
   COORD coord;
   coord.X = x;
   coord.Y = y;
   SetConsoleCursorPosition (GetStdHandle (STD_OUTPUT_HANDLE), coord);
}

/* Através de condições usadas pelas regras do jogo, essa função retornará 0 se o
movimento for válido ou 1 se o movimento for inválido. Na 1ª pilha selecionada,
as cartas armazenadas na pilha são passadas para uma pilha temporária. E a carta que 
está no topo dessa pilha temporária é usada para se fazer as comparações com a 2ª carta 
selecionada, que está na outra pilha. */
int verificaMovimento(pilhaP temp)
{
   CARTA2 = pilhaTemp->carta;
   CARTA = temp.carta;
   if((((CARTA2.naipe == '\x3')||(CARTA2.naipe == '\x4'))
     &&((CARTA.naipe  == '\x5')||(CARTA.naipe  == '\x6')))
     ||(((CARTA2.naipe== '\x5')||(CARTA2.naipe == '\x6'))
     &&((CARTA.naipe  == '\x3')||(CARTA.naipe  == '\x4'))))
   {
      if((CARTA2.valCarta == 'A')&&(CARTA.valCarta == '2'))
         return 0;
      else
      {
         if(((CARTA2.valCarta > 42)&&(CARTA2.valCarta < 57))
         && (CARTA.valCarta == CARTA2.valCarta + 1))
            return 0;
         else
         {
            if((CARTA2.valCarta == '9')&&(CARTA.valCarta == 'D'))
               return 0;
            else
            {
               if((CARTA2.valCarta == 'D')&&(CARTA.valCarta == 'J'))
                  return 0;
               else
               {
                  if((CARTA2.valCarta == 'J')&&(CARTA.valCarta == 'Q'))
                     return 0;
                  else
                  {
                     if((CARTA2.valCarta == 'Q')&&(CARTA.valCarta == 'K'))
                        return 0;
                     else
                     {
                        if((CARTA2.valCarta == 'K')&&(&CARTA == NULL))
                        {
                           return 0;
                        }
                     }
                  }
               }   
            }   
         }
      }   
   }
   else
      return 1;// SE O MOVIMENTO NÃO PUDER SER EXECUTADO
}

/* Verifica, de acordo com a regras do jogo, se a carta de origem pode ser desalocada
da sua pilha inicial e se alocada no topo da pilha selecionada. Esta função retornará
0 se o movimento for válido e 1 se o movimento for inválido */
int verificaMovimento2(pilhaP temp)
{
   CARTA2 = pilhaTemp->carta;
   CARTA = temp.carta;
   if(CARTA2.naipe == CARTA.naipe)
   {
      if((CARTA.valCarta == 'A')&&(CARTA2.valCarta == '2'))
         return 0;
      else
      {
         if(((CARTA.valCarta > 42)&&(CARTA.valCarta < 57))
         && (CARTA2.valCarta == CARTA.valCarta + 1))
            return 0;
         else
         {
            if((CARTA.valCarta == '9')&&(CARTA2.valCarta == 'D'))
               return 0;
            else
            {
               if((CARTA.valCarta == 'D')&&(CARTA2.valCarta == 'J'))
                  return 0;
               else
               {
                  if((CARTA.valCarta == 'J')&&(CARTA2.valCarta == 'Q'))
                     return 0;
                  else
                  {
                     if((CARTA.valCarta == 'Q')&&(CARTA2.valCarta == 'K'))
                        return 0;
                     else
                     {
                        if((CARTA.valCarta == 'K')&&(&CARTA2 == NULL))
                        {
                           return 0;
                        }
                     }
                  }
               }   
            }   
         }
      }   
   }
   else
      return 1;// SE O MOVIMENTO NÃO PUDER SER EXECUTADO
}

/* Esvazia todas as pilhas a fim de se evitar o acumulo de lixo virtual na memória */
void esvaziaPilhas()
{
   int i,j,nElementos;
   cartas cartaTemp;
   
   for(i = 0; i<14; i++)
   {
      switch(i)
      {
         case 0:
              nElementos = contaElementos(topo);
              for(j = 0; j<nElementos; j++)   
                 pop(&cartaTemp,0);
              break;
         case 1:
              nElementos = contaElementos(topo1);
              for(j = 0; j<nElementos; j++)   
                 pop(&cartaTemp,1);
              break;
         case 2:
              nElementos = contaElementos(topo2);
              for(j = 0; j<nElementos; j++)   
                 pop(&cartaTemp,2);
              break;
         case 3:
              nElementos = contaElementos(topo3);
              for(j = 0; j<nElementos; j++)   
                 pop(&cartaTemp,3);
              break;
         case 4:
              nElementos = contaElementos(topo4);
              for(j = 0; j<nElementos; j++)   
                 pop(&cartaTemp,4);
              break;
         case 5:
              nElementos = contaElementos(topo5);
              for(j = 0; j<nElementos; j++)   
                 pop(&cartaTemp,5);
              break;
         case 6:
              nElementos = contaElementos(topo6);
              for(j = 0; j<nElementos; j++)   
                 pop(&cartaTemp,6);
              break;
         case 7:
              nElementos = contaElementos(topo7);
              for(j = 0; j<nElementos; j++)   
                 pop(&cartaTemp,7);
              break;
         case 8:
              nElementos = contaElementos(topo8);
              for(j = 0; j<nElementos; j++)   
                 pop(&cartaTemp,8);
              break;
         case 9:
              nElementos = contaElementos(topo9);
              for(j = 0; j<nElementos; j++)   
                 pop(&cartaTemp,9);
              break;
         case 10:
              nElementos = contaElementos(topo10);
              for(j = 0; j<nElementos; j++)   
                 pop(&cartaTemp,10);
              break;
         case 11:
              nElementos = contaElementos(topo11);
              for(j = 0; j<nElementos; j++)   
                 pop(&cartaTemp,11);
              break;
         case 12:
              nElementos = contaElementos(topo12);
              for(j = 0; j<nElementos; j++)   
                 pop(&cartaTemp,12);
              break;
         case 13:
              nElementos = contaElementos(pilhaTemp);
              for(j = 0; j<nElementos; j++)   
                 pop(&cartaTemp,13);
              break;
      }
   }
}

/* Verifica se a pilha selecionada é a pilha principal usando como parâmetro de
comparação as posições na jenela onde se encontra a pilha principal */
int hotSpot(int x, int y)
{
   if ((x >= 4) && (x <= 12) && (y >= 3) && (y <=11))
      return 1; // verdadeiro
   else
      return 0; // falso
}

/* Verifica se a posição coluna do clique do mouse se encontra entre as colunas
específicas de cada pilha. Esta função retornará -1 caso a posição coluna do clique
não estaja em nenhum coluna de alguma pilha ou retornará o código da pilha pilha onde
houve o clique do mouse */
int posicaoCartaX(int x,int y)
{
   
   int minimo = 4, maximo = 12;
   int i = 0;
   if((y>=3)&&(y<=11))
   {
      if((x>=14)&&(x<=22))
         return 8;
      minimo = 34;
      maximo = 42;
      for (i = 0; i < 7; i++)
      {
         if ((x >= minimo) && (x <= maximo))
            return (i + 9);
         minimo += 10;
         maximo += 10;
      }    
   }
   else
   {
      for (i = 0; i < 7; i++)
      {
         if ((x >= minimo) && (x <= maximo))
            return (i + 1);
         minimo += 10;
         maximo += 10;
      }
   }
   return -1; // Erro: coluna inválida
}

/* Caso a posição da coluna seja válida, essa função retornará a posição da carta
selecionada pelo clique do mouse, ou seja, retornará a linha onde se encontra essa carta
na pilha selecionada pelo clique do mouse. Ou retornará -1 caso a posição da linha não 
esteja entre as linhas válidas */
int posicaoCartaY(int x, int y)
{
   
   int elementos = 0, i = 0;
   int minimo = 14, maximo = 15;
   if (posicaoCartaX(x,y) != -1) {
      switch (posicaoCartaX(x,y)) {
         case 1:
            elementos = contaElementos(topo1);
            break;   
         case 2:
            elementos = contaElementos(topo2);
            break;   
         case 3:
            elementos = contaElementos(topo3);
            break;   
         case 4:
            elementos = contaElementos(topo4);
            break;   
         case 5:
            elementos = contaElementos(topo5);
            break;   
         case 6:
            elementos = contaElementos(topo6);
            break;   
         case 7:
            elementos = contaElementos(topo7);
            break;
         case 8:
         case 9:
         case 10:
         case 11:
         case 12:
            return 1;
            
                
      }
      for (i = 0; i < elementos - 1; i++) {
         if ((y >= minimo) && (y <= maximo))
            return (i + 1);
         minimo += 2;
         maximo += 2;
      }
      if ((y >= 14) && (y <= maximo + 7))
         return elementos;
   }
   return -1; // Erro: linha inválida
}

/* Movimento inicial das cartas: move para uma pilha temporária as cartas selecionadas
de uma pilha para que essas possam ser alocadas na pilha escolhida, caso o movimento
seja válido. Se o movimento não for válido, as cartas armazenadas na pilha temporária
serão devolvidas a pilha de origem. Essa função retornará 1 caso seja possível não seja
possível ou 0 caso o movimento seja possível */
int moveCartas(int codPilha, int posCarta)
{
   if(posCarta == -1)
   {
      printf("if");
      return 1;
   }
   printf("passou");         
   int i,cont = 0,elementos;
   gotoxy(2,3);
   switch(codPilha)
   {  
      case 1:
         if(topo1 == NULL)
            return 1;         
         elementos = contaElementos(topo1);         
         for(i = posCarta; i<elementos + 1; i++)
         {
            cont++;
             pop(&CARTA, codPilha);
            push(&CARTA, 13);
         }
         break;
      case 2:         
         if(topo2 == NULL)
            return 1;         
         elementos = contaElementos(topo2);         
         for(i = posCarta; i<elementos + 1; i++)
         {
            cont++;
             pop(&CARTA, codPilha);
            push(&CARTA, 13);
         }
         break;
      case 3:         
         elementos = contaElementos(topo3);         
         for(i = posCarta; i<elementos + 1; i++)
         {
            cont++;
             pop(&CARTA, codPilha);
            push(&CARTA, 13);
         }
         break;
      case 4:         
         elementos = contaElementos(topo4);         
         for(i = posCarta; i<elementos + 1; i++)
         {
            cont++;
            pop(&CARTA, codPilha);
            push(&CARTA, 13);
         }        
         break;
      case 5://TALVES FUNCIONE
         elementos = contaElementos(topo5);         
         for(i = posCarta; i<elementos + 1; i++)
         {
            cont++;
            pop(&CARTA, codPilha);
            push(&CARTA, 13);
         }         
         break;
      case 6:         
         elementos = contaElementos(topo6);         
         for(i = posCarta; i<elementos + 1; i++)
         {
            cont++;
             pop(&CARTA, codPilha);
            push(&CARTA, 13);
         }         
         break;
      case 7:         
         elementos = contaElementos(topo7);         
         for(i = posCarta; i<elementos + 1; i++)
         {
            cont++;
             pop(&CARTA, codPilha);
            push(&CARTA, 13);
         }        
         break;
      case 8:
         pop(&CARTA, codPilha);
         CARTA.posicao = PCIMA;
         push(&CARTA, 13);
         break;
      case 9:
         if(topo9 == NULL)
            return 1;
         pop(&CARTA, codPilha);
         CARTA.posicao = PCIMA;
         push(&CARTA, 13);
         break;
      case 10:
         if(topo10 == NULL)
            return 1;         
         pop(&CARTA, codPilha);
         CARTA.posicao = PCIMA;
         push(&CARTA, 13);
         break;
      case 11:
         if(topo11 == NULL)
            return 1;         
         pop(&CARTA, codPilha);
         CARTA.posicao = PCIMA;
         push(&CARTA, 13);
         break;
      case 12:
         if(topo12 == NULL)
            return 1;         
         pop(&CARTA, codPilha);
         CARTA.posicao = PCIMA;
         push(&CARTA, 13);
         break;
   }
   if(pilhaTemp->carta.posicao == PBAIXO)
   {
      for(i = 0; i< posCarta; i++)
      {
         pop(&CARTA, 13);
         push(&CARTA, codPilha);
      }
      return 1; //DEU MERDA NÃO PODE
   }
   else
   {
      codPilhaAux = codPilha;
      return 0; //PODE
   }
}

/* Caso o movimento seja válido, esta função moverá da pilha temporária para a
pilha destino as cartas selecionadas. Usando o código de cada pilha, ela irá 
saber em qual pilha ela poderá alocar as cartas selecionadas. */
int moveCartasDestino(int codPilha)
{
   switch(codPilha)
   {
      case 1:
         CARTA2 = pilhaTemp->carta;
         if(topo1 == NULL)
         {
            if(CARTA2.valCarta == 'K')
            {
               while((pilhaTemp != NULL))
               {
                  pop(&CARTA, 13);
                  CARTA.linha = 15 +(2*contaElementos(topo1));
                  CARTA.coluna = 5;
                  push(&CARTA, codPilha);
               }
               return 0;
            }
            else
            {
               moveTempOrigem();
               return 1;
            }
         }        
         if(verificaMovimento(*topo1) == 0)
         {
            while(pilhaTemp != NULL)
            {
               pop(&CARTA, 13);
               CARTA.linha = 15 +(2*contaElementos(topo1));
               CARTA.coluna = 5;
               push(&CARTA, codPilha);
            }
            return 0;
         }
         else
            moveTempOrigem();
      break;
      case 2:
         CARTA2 = pilhaTemp->carta;
         if(topo2 == NULL)
         {
            if(CARTA2.valCarta == 'K')
            {
               while((pilhaTemp != NULL))
               {
                  pop(&CARTA, 13);
                  CARTA.linha = 15 +(2*contaElementos(topo2));
                  CARTA.coluna = 15;
                  push(&CARTA, codPilha);
               }
               return 0;
            }
            else
            {
               moveTempOrigem();
               return 1;
            }
         }        
         if(verificaMovimento(*topo2) == 0)
         {
            while(pilhaTemp != NULL)
            {
               pop(&CARTA, 13);
               CARTA.linha = 15 +(2*contaElementos(topo2));
               CARTA.coluna = 15;
               push(&CARTA, codPilha);
            }
            return 0;
         }
         else
            moveTempOrigem();
      break;
      case 3:
         CARTA2 = pilhaTemp->carta;
         if(topo3 == NULL)
         {
            if(CARTA2.valCarta == 'K')
            {
               while((pilhaTemp != NULL))
               {
                  pop(&CARTA, 13);
                  CARTA.linha = 15 +(2*contaElementos(topo3));
                  CARTA.coluna = 25;
                  push(&CARTA, codPilha);
               }
               return 0;
            }
            else
            {
               moveTempOrigem();
               return 1;
            }
         }        
         if(verificaMovimento(*topo3) == 0)
         {
            while(pilhaTemp != NULL)
            {
               pop(&CARTA, 13);
               CARTA.linha = 15 +(2*contaElementos(topo3));
               CARTA.coluna = 25;
               push(&CARTA, codPilha);
            }
            return 0;
         }
         else
            moveTempOrigem();
      break;
      case 4:
         CARTA2 = pilhaTemp->carta;
         if(topo4 == NULL)
         {
            if(CARTA2.valCarta == 'K')
            {
               while((pilhaTemp != NULL))
               {
                  pop(&CARTA, 13);
                  CARTA.linha = 15 +(2*contaElementos(topo4));
                  CARTA.coluna = 35;
                  push(&CARTA, codPilha);
               }
               return 0;
            }
            else
            {
               moveTempOrigem();
               return 1;
            }
         }        
         if(verificaMovimento(*topo4) == 0)
         {
            while(pilhaTemp != NULL)
            {
               pop(&CARTA, 13);
               CARTA.linha = 15 +(2*contaElementos(topo4));
               CARTA.coluna = 35;              
               push(&CARTA, codPilha);
            }
            return 0;
         }
         else
            moveTempOrigem();
      break;
      case 5:
         CARTA2 = pilhaTemp->carta;
         if(topo5 == NULL)
         {
            if(CARTA2.valCarta == 'K')
            {
               while((pilhaTemp != NULL))
               {
                  pop(&CARTA, 13);
                  CARTA.linha = 15 +(2*contaElementos(topo5));
                  CARTA.coluna = 45;
                  push(&CARTA, codPilha);
               }
               return 0;
            }
            else
            {
               moveTempOrigem();
               return 1;
            }
         }        

         if(verificaMovimento(*topo5) == 0)
         {
            while(pilhaTemp != NULL)
            {
               pop(&CARTA, 13);
               CARTA.linha = 15 +(2*contaElementos(topo5));
               CARTA.coluna = 45;              
               push(&CARTA, codPilha);
            }
            return 0;
         }
         else
            moveTempOrigem();
      break;
      case 6:
         CARTA2 = pilhaTemp->carta;
         if(topo6 == NULL)
         {
            if(CARTA2.valCarta == 'K')
            {
               while((pilhaTemp != NULL))
               {
                  pop(&CARTA, 13);
                  CARTA.linha = 15 +(2*contaElementos(topo6));
                  CARTA.coluna = 55;
                  push(&CARTA, codPilha);
               }
               return 0;
            }
            else
            {
               moveTempOrigem();
               return 1;
            }
         }        

         if(verificaMovimento(*topo6) == 0)
         {
            while(pilhaTemp != NULL)
            {
               pop(&CARTA, 13);
               CARTA.linha = 15 +(2*contaElementos(topo6));
               CARTA.coluna = 55;              
               push(&CARTA, codPilha);
            }
            return 0;
         }
         else
            moveTempOrigem();
      break;
      case 7:
         CARTA2 = pilhaTemp->carta;
         if(topo7 == NULL)
         {
            if(CARTA2.valCarta == 'K')
            {
               while((pilhaTemp != NULL))
               {
                  pop(&CARTA, 13);
                  CARTA.linha = 15 +(2*contaElementos(topo7));
                  CARTA.coluna = 65;
                  push(&CARTA, codPilha);
               }
               return 0;
            }
            else
            {
               moveTempOrigem();
               return 1;
            }
         }        
         if(verificaMovimento(*topo7) == 0)
         {
            while(pilhaTemp != NULL)
            {
               pop(&CARTA, 13);
               CARTA.linha = 15 +(2*contaElementos(topo7));
               CARTA.coluna = 65;              
               push(&CARTA, codPilha);
            }
            return 0;
         }
         else
            moveTempOrigem();
      break;
      
      case 8:
         moveTempOrigem();
         break;
           
      case 9:
         CARTA2 = pilhaTemp->carta;
         if(topo9 == NULL)
         {
            if((CARTA2.valCarta == 'A')&&(CARTA2.naipe == '\x5'))
            {
               while((pilhaTemp != NULL))
               {
                  pop(&CARTA, 13);
                  CARTA.linha = 4;
                  CARTA.coluna = 35;
                  push(&CARTA, codPilha);
               }
               return 0;
            }
            else
            {
               moveTempOrigem();
               return 1;
            }
         }        
         if(verificaMovimento2(*topo9) == 0)
         {
            while(pilhaTemp != NULL)
            {
               pop(&CARTA, 13);
               CARTA.linha = 4;
               CARTA.coluna = 35;              
               push(&CARTA, codPilha);
            }
            return 0;
         }
         else
            moveTempOrigem();
      break;
      case 10:
         CARTA2 = pilhaTemp->carta;
         if(topo10 == NULL)
         {
            if((CARTA2.valCarta == 'A')&&(CARTA2.naipe == '\x6'))
            {
               while((pilhaTemp != NULL))
               {
                  pop(&CARTA, 13);
                  CARTA.linha = 4;
                  CARTA.coluna = 45;
                  push(&CARTA, codPilha);
               }
               return 0;
            }
            else
            {
               moveTempOrigem();
               return 1;
            }
         }        
         if(verificaMovimento2(*topo10) == 0)
         {
            while(pilhaTemp != NULL)
            {
               pop(&CARTA, 13);
               CARTA.linha = 4;
               CARTA.coluna = 45;              
               push(&CARTA, codPilha);
            }
            return 0;
         }
         else
            moveTempOrigem();
      break;
      case 11:
         CARTA2 = pilhaTemp->carta;
         if(topo11 == NULL)
         {
            if((CARTA2.valCarta == 'A')&&(CARTA2.naipe == '\x4'))
            {
               while((pilhaTemp != NULL))
               {
                  pop(&CARTA, 13);
                  CARTA.linha = 4;
                  CARTA.coluna = 55;
                  push(&CARTA, codPilha);
               }
               return 0;
            }
            else
            {
               moveTempOrigem();
               return 1;
            }
         }        
         if(verificaMovimento2(*topo11) == 0)
         {
            while(pilhaTemp != NULL)
            {
               pop(&CARTA, 13);
               CARTA.linha = 4;
               CARTA.coluna = 55;              
               push(&CARTA, codPilha);
            }
            return 0;
         }
         else
            moveTempOrigem();
      break;
      case 12:
         CARTA2 = pilhaTemp->carta;
         if(topo12 == NULL)
         {
            if((CARTA2.valCarta == 'A')&&(CARTA2.naipe == '\x3'))
            {
               while((pilhaTemp != NULL))
               {
                  pop(&CARTA, 13);
                  CARTA.linha = 4;
                  CARTA.coluna = 65;
                  push(&CARTA, codPilha);
               }
               return 0;
            }
            else
            {
               moveTempOrigem();
               return 1;
            }
         }        
         if(verificaMovimento2(*topo12) == 0)
         {
            while(pilhaTemp != NULL)
            {
               pop(&CARTA, 13);
               CARTA.linha = 4;
               CARTA.coluna = 65;              
               push(&CARTA, codPilha);
            }
            return 0;
         }
         else
            moveTempOrigem();
      break;
   }
   return 1;
}

/* Caso o movimento seja inválido, esta função devolverá à pilha de origem suas
cartas, que foram retiradas e colocadas na pilha temporária */
void moveTempOrigem()
{
   while(pilhaTemp != NULL)
   {
      pop(&CARTA,13);
      push(&CARTA,codPilhaAux);
   }
}

/* Varre os topos das pilhas que ficam em baixo, verificando se, em algum topo,
o valor que caracteriza o desenho da carta esteja para cima, a fim de se evitar 
que as cartas que estão no topo de cada pilha não fiquem desenhadas para baixo */
void varrePilhas(void)
{
   int i;
   for (i = 1; i < 8; i++) {
      switch (i) {
         case 1:
            if(topo1 != NULL)
            { 
               if (topo1->carta.posicao == PBAIXO)
                  topo1->carta.posicao = PCIMA;
            }
         break;
         case 2:
            if(topo2 != NULL)
            { 
               if (topo2->carta.posicao == PBAIXO)
                  topo2->carta.posicao = PCIMA;
            }
         break;
         case 3:
            if(topo3 != NULL)
            { 
               if (topo3->carta.posicao == PBAIXO)
                  topo3->carta.posicao = PCIMA;
            }
         break;
         case 4:
            if(topo4 != NULL)
            { 
               if (topo4->carta.posicao == PBAIXO)
                  topo4->carta.posicao = PCIMA;
            }
         break;
         case 5:
            if(topo5 != NULL)
            { 
               if (topo5->carta.posicao == PBAIXO)
                  topo5->carta.posicao = PCIMA;
            }
         break;
         case 6:
            if(topo6 != NULL)
            { 
               if (topo6->carta.posicao == PBAIXO)
                  topo6->carta.posicao = PCIMA;
            }
         break;
         case 7:
            if(topo7 != NULL)
            { 
               if (topo7->carta.posicao == PBAIXO)
                  topo7->carta.posicao = PCIMA;
            }
         break;
      } 
   }
}

/*
Desenha a borda para mostrar a opção de 
AJUDA
*/
void mostraLayout()
{
   int aux;
   
   textcolor(WHITE);
   textbackground(LIGHTGRAY);
   textbackground(RED);
   textcolor(WHITE);
   gotoxy(10,4);
   for(aux=0;aux<50;aux++)
   {
      printf("\xcd");
   }
   gotoxy(10,4);
   printf("\xc9");
   gotoxy(10,5);
   for(aux=5;aux<27;aux++)
   {
      printf("\xba\n");
      gotoxy(10,aux);   
   }
   printf("\xc8");
   gotoxy(11,26);
   for(aux=1;aux<50;aux++)
   {
      printf("\xcd");
   }
   printf("\xbc");
   gotoxy(50,4);
   for(aux=5;aux<26;aux++)
   {
      gotoxy(60,aux);
      printf("\xba");
   }
   gotoxy(60,4);
   printf("\xbb");
   gotoxy(11,5);
   for(aux=5;aux<27;aux++)
   {
      printf("                                                 ");
      gotoxy(11,aux);
   }
   gotoxy(12,7);
   printf("                     AJUDA");
   gotoxy(12,11);
   printf("Selecione a carta desejada com um clique duplo ");
   gotoxy(12,12);
   printf("sobre a mesma, em seguida selecione a pilha pa-");
   gotoxy(12,13);
   printf("ra onde deseja mover a carta escolhida, tambem ");
   gotoxy(12,14);
   printf("com um clique duplo. Tambem com um clique duplo");
   gotoxy(12,15);
   printf("sao desviradas as cartas do baralho principal.");
   gotoxy(12,17);
   printf("No rodape da janela esta presente uma barra que");
   gotoxy(12,18);
   printf("indicara o status das acoes do jogador.");
   gotoxy(12,20);
   printf(" PRESSIONE QUALQUER TECLA PARA CONTINUAR...");   
      
}


/*
Verifica se o jogo acabou.
*/
void verificaFimJogo(int *novoJogo, HANDLE *hStdin, DWORD *fdwMode)
{
   if((contaElementos(topo9) == 13)&&(contaElementos(topo10) == 13)&&
      (contaElementos(topo11) == 13)&&(contaElementos(topo12) == 13))    
   {
      textbackground(GREEN);
      system("cls");
      textbackground(WHITE);
      gotoxy(1,1);  
      printf(MENU);
      gotoxy(1,50);
      printf(MSG);
      gotoxy(1,50);
      printf("PARABENS VOCE VENCEU!!!!! PRESSIONE QUALQUER TECLA PARA CONTINUAR");
      gotoxy(1,1);
      SetConsoleMode(*hStdin, *fdwMode);
      getch();
      esvaziaPilhas();
      *novoJogo = 0;                          
   }
   
}
