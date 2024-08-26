#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "connect4.h"

void 
connect4_init (connect4* game, int rows, int columns, int num_of_players, int win_size){  

    //"connect4* game" es un apuntador, entonces hacemos una "desreferenciacion" con "game -> ...",
    //accedemos a rows, columns, num_of_players, win_size y escribimos los valores
    //que nos pasan como parametro
    game -> rows = rows;
    game -> columns = columns;
    game -> num_of_players = num_of_players;
    game -> win_size = win_size;

                                    //memoria para las filas del tablero
    game -> board = (char**) malloc(sizeof(char*) * rows);

    //memoria para las columnas de cada fila
    int row = 0;
    while(row < rows){
      game -> board[row] = (char*) malloc(columns);
      row++;
    }

    //rellenamos todo el tablero (filas y columnas) con 0's
    for (int row = 0; row < game -> rows; row++){
        for (int column = 0; column < game->columns; column++){
          game -> board[row][column]  = '0';
      }
    }
}

void
connect4_free (connect4* game){

    //hacemos una "desreferenciacion" con "game -> ..." y asignamos una variable a las rows
    int rows = game->rows;

    //por medio de free liberamos la memoria que adquirimos en connect4_init
    int row = 0;
    while(row < rows){
      //liberando memoria
      free(game->board[row]);
      row++;
    }
    //liberando memoria
    free(game->board);
}

char* 
connect4_to_string(connect4* game){

    //hacemos una "desreferenciacion" con "game -> ..." y asignamos a variables las columns y rows
    int columns = game->columns;
    int rows = game->rows;

    //pedimos con malloc la memoria que vamos a necesitar para la representacion de un string
    int total_size = columns * rows + 1;
    char* string_final = malloc(sizeof(char) * total_size);
  
    //esta variable va a ayudarnos como indice para movernos en string_final
    int k = 0;
  
    for(int i = rows-1; i >= 0; i--) {
        char* row = game->board[i];

        for(int j = 0; j < columns; j++) {
          string_final[k] = row[j]; //va a ir copiando los valores en la fila (con row[j]) a string_final
          k++; //aumentamos el valor del indice
        }

        //saltos de linea si no es la ultima fila (i > 0)
        if(i){
          string_final[k] = '\n';
          k++; //aumentamos el valor del indice
        }
    }
    //El'\0' representa el fin del string
    string_final[k] = '\0';
    return string_final;
}

int 
connect4_make_play (connect4* game, int player, int column){
    
    //hacemos una "desreferenciacion" con "game -> ..." y asignamos a variables las columns, rows y num_of_players
    int columns = game->columns;
    int rows = game->rows;
    int num_of_players = game->num_of_players;

    //verifica para columna invalida
    if (column >= columns || column < 0){ //fuera de los limites de columnas
        return INVALID_COLUMN;
    }

    //verifica para jugador invalido
    if(player > num_of_players || player <= 0){ //fuera de los limites en el num de jugadores
        return INVALID_PLAYER;
    }

    int row = 0;
    while(row < rows){ //recorre el tablero buscando donde poner el token
        if(game->board[row][column] == '0'){ //ha encontrado donde
          game->board[row][column] = '0' + player; //el jugador pone su token en el tablero
          return row; //la fila donde fue colocado el token
        }
        row++; //si no ha encontrado donde entonces avanza de fila
    }
    //si llegamos aca es porque la columna se encuentra llena
    return FULL_COLUMN;
}

//funcion auxiliar para connect4_player_won
bool 
connect4_player_won_diagonal(connect4* game, int player, int row, int column){

    //hacemos una "desreferenciacion" con "game -> ..." y asignamos a variables las columns, rows y win_size
    int columns = game->columns;
    int rows = game->rows;
    int win_size = game->win_size; //es el numero de tokens consecutivos que necesita tener un jugador para ganar

    //el token del jugador que puso durante sus jugadas en el tablero
    char token = player + '0';

    //-----verifica si el jugador ha ganado de izquierda a derecha-----

    int f = 0;
    bool player_won_diag_id; //variable booleana para saber si gana el jugador

    while(f <= rows - win_size){
      for(int j = 0; j <= columns - win_size; j++){
        player_won_diag_id = true; //comienzamos asignandole el valor de true

        int i = 0;
        //este while es para verificar si estan los 4 tokens en diagonal
        while(i < win_size){
          //busca a lo largo de la diagonal de izquierda a derecha
          //si no encuentra el token cambia a false la variable booleana
          if(game->board[f + i][j + i] != token) player_won_diag_id = false;
          i++;
        }

        //si al final la variable player_won es true entonces gano el jugador
        if(player_won_diag_id == true){
          return true; //ha ganado
        } 
      }
      f++;
    }

    //-----verifica si el jugador ha ganado de derecha a izquierda-----
    
    int d = 0; 
    bool player_won_diag_di; //variable booleana para saber si gana el jugador

    while(d <= rows - win_size){ //para iterar en filas
      for(int i = columns - 1; i >= win_size - 1; i--){ //para iterar en columnas
        player_won_diag_di = true; //comienzamos asignandole el valor de true
        
        int j = 0;
        //este while es para verificar si estan los 4 tokens en diagonal
        while(j < win_size){
          //busca a lo largo de la diagonal de derecha a izquierda 
          //si no encuentra el token cambia a false la variable booleana
          if(game->board[d + j][i - j] != token) player_won_diag_di = false;
          j++;
        }
        //si al final la variable player_won es true entonces gano el jugador
        if(player_won_diag_di == true){
          return true; //ha ganado
        } 
      }
      d++;
    }
    //si al final no ha entrado en alguno de los if(player_won_diag_....) entonces no ha ganado el jugador
    return false;
}

//funcion auxiliar para connect4_player_won
bool 
connect4_player_won_column(connect4* game, int player, int row, int column){

    //hacemos una "desreferenciacion" con "game -> ..." y asignamos a variables las rows y win_size
    int rows = game->rows;
    int win_size = game->win_size; //es el numero de tokens consecutivos que necesita tener un jugador para ganar

    //el token del jugador que puso durante sus jugadas en el tablero
    char token = player + '0';

    //verifica si el jugador ha ganado en una columna

    int i = 0; //variable para la iteracion
    bool player_won_column; //variable booleana para saber si gana el jugador

    while(i <= rows - win_size){
      player_won_column = true; //comienzamos asignandole el valor de true

      int j = 0;
      //este while es para verificar si estan los 4 tokens en la columna
      while(j < win_size){ 
        //por eso solo avanzan en posicion las filas, porque verifica en la columna en busca del token del jugador,
        //y si no encuentra el token cambia a false la variable booleana
        if(game->board[i + j][column] != token) player_won_column = false; 
        j++;
      }

      //si al final la variable player_won es true entonces gano el jugador
      if(player_won_column == true){
        return true; //ha ganado
      } 
      i++;
    }
    //si al final no ha entrado en if(player_won_column) entonces no ha ganado el jugador
    return false;
}

//funcion auxiliar para connect4_player_won
bool 
connect4_player_won_row(connect4* game, int player, int row, int column){

    //hacemos una "desreferenciacion" con "game -> ..." y asignamos a variables las columns y win_size
    int columns = game->columns;
    int win_size = game->win_size; //es el numero de tokens consecutivos que necesita tener un jugador para ganar

    //el token del jugador que puso durante sus jugadas en el tablero
    char token = player + '0';
    
    //funciona de manera similar a la implementacion para columna
    //verifica si el jugador ha ganado en una fila

    int k = 0; //variable para la iteracion
    //va a almacenar el valor booleano para saber si gano el jugador o no
    bool player_won_row = true; 

    while(k <= columns - win_size){
      player_won_row = true; //comienzamos asignandole el valor de true
      int j = 0;
      //este while es para verificar si estan los 4 tokens en la columna
      while(j < win_size){
        //por eso solo avanzan en posicion las columnas, porque verifica en la fila en busca del token del jugador,
        //si no esta el token marca won como false
        if(game->board[row][k + j] != token) player_won_row = false;
        j++;
      }
      
      //si al final la variable player_won es true entonces gano el jugador
      if(player_won_row == true){
        return true; //ha ganado
      } 
      k++;
    }
    //si al final no ha entrado en if(player_won_row) entonces no ha ganado el jugador
    return false;
}

//ahora si, connect4_player_won
bool 
connect4_player_won(connect4* game, int player, int row, int column){
    return   connect4_player_won_diagonal(game, player, row, column)
          || connect4_player_won_column(game, player, row, column)  
          || connect4_player_won_row(game, player, row, column);
}