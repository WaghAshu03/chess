/*
Initial State of the board

    a   b   c   d   e   f   g   h
  +---+---+---+---+---+---+---+---+
8 | r | n | b | q | k | b | n | r | 8
  +---+---+---+---+---+---+---+---+
7 | p | p | p | p | p | p | p | p | 7
  +---+---+---+---+---+---+---+---+
6 |   |   |   |   |   |   |   |   | 6
  +---+---+---+---+---+---+---+---+
5 |   |   |   |   |   |   |   |   | 5
  +---+---+---+---+---+---+---+---+
4 |   |   |   |   |   |   |   |   | 4
  +---+---+---+---+---+---+---+---+
3 |   |   |   |   |   |   |   |   | 3
  +---+---+---+---+---+---+---+---+
2 | P | P | P | P | P | P | P | P | 2
  +---+---+---+---+---+---+---+---+
1 | R | N | B | Q | K | B | N | R | 1
  +---+---+---+---+---+---+---+---+
    a   b   c   d   e   f   g   h

                OR

    a   b   c   d   e   f   g   h
  +---+---+---+---+---+---+---+---+
8 | ♖ | ♘ | ♗ | ♕ | ♔ | ♗ | ♘ | ♖ | 8
  +---+---+---+---+---+---+---+---+
7 | ♙ | ♙ | ♙ | ♙ | ♙ | ♙ | ♙ | ♙ | 7
  +---+---+---+---+---+---+---+---+
6 |   |   |   |   |   |   |   |   | 6
  +---+---+---+---+---+---+---+---+
5 |   |   |   |   |   |   |   |   | 5
  +---+---+---+---+---+---+---+---+
4 |   |   |   |   |   |   |   |   | 4
  +---+---+---+---+---+---+---+---+
3 |   |   |   |   |   |   |   |   | 3
  +---+---+---+---+---+---+---+---+
2 | ♟ | ♟ | ♟ | ♟ | ♟ | ♟ | ♟ | ♟ | 2
  +---+---+---+---+---+---+---+---+
1 | ♜ | ♞ | ♝ | ♛ | ♚ | ♝ | ♞ | ♜ | 1
  +---+---+---+---+---+---+---+---+
    a   b   c   d   e   f   g   h

*/
#pragma execution_character_set("utf-8")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <unistd.h>

#define size 8

int move_num = 1;
int use_symbols = 0;

typedef struct piece
{
    int cost;
    char type;
    char color;
    char *symbol;
    int piece_active;
    int piece_moved;
} piece;

typedef struct square
{
    int rank;
    char file;
    piece *piece;
} square;

typedef struct piece_side
{
    char color;
    piece Pawn[size];
    piece King;
    piece Queen;
    piece Knight[2];
    piece Bishop[2];
    piece Rook[2];
} piece_side;

piece_side white = {
    .King = {.cost = 900, .type = 'K', .color = 'W', .symbol = "♚", .piece_active = 1, .piece_moved = 0},
    .Queen = {.cost = 9, .type = 'Q', .color = 'W', .symbol = "♛", .piece_active = 1},
    .Knight = {{.cost = 3, .type = 'N', .color = 'W', .symbol = "♞", .piece_active = 1}, {.cost = 3, .type = 'N', .color = 'W', .symbol = "♞", .piece_active = 1}},
    .Bishop = {{.cost = 3, .type = 'B', .color = 'W', .symbol = "♝", .piece_active = 1}, {.cost = 3, .type = 'B', .color = 'W', .symbol = "♝", .piece_active = 1}},
    .Rook = {{.cost = 5, .type = 'R', .color = 'W', .symbol = "♜", .piece_active = 1, .piece_moved = 0}, {.cost = 5, .type = 'R', .color = 'W', .symbol = "♜", .piece_active = 1, .piece_moved = 0}},
    .Pawn = {
        {.cost = 1, .type = 'P', .color = 'W', .symbol = "♟", .piece_active = 1},
        {.cost = 1, .type = 'P', .color = 'W', .symbol = "♟", .piece_active = 1},
        {.cost = 1, .type = 'P', .color = 'W', .symbol = "♟", .piece_active = 1},
        {.cost = 1, .type = 'P', .color = 'W', .symbol = "♟", .piece_active = 1},
        {.cost = 1, .type = 'P', .color = 'W', .symbol = "♟", .piece_active = 1},
        {.cost = 1, .type = 'P', .color = 'W', .symbol = "♟", .piece_active = 1},
        {.cost = 1, .type = 'P', .color = 'W', .symbol = "♟", .piece_active = 1},
        {.cost = 1, .type = 'P', .color = 'W', .symbol = "♟", .piece_active = 1},
    }};

piece_side black = {
    .King = {.cost = -900, .type = 'k', .color = 'B', .symbol = "♔", .piece_active = 1, .piece_moved = 0},
    .Queen = {.cost = -9, .type = 'q', .color = 'B', .symbol = "♕", .piece_active = 1},
    .Knight = {{.cost = -3, .type = 'n', .color = 'B', .symbol = "♘", .piece_active = 1}, {.cost = -3, .type = 'n', .color = 'B', .symbol = "♘", .piece_active = 1}},
    .Bishop = {{.cost = -3, .type = 'b', .color = 'B', .symbol = "♗", .piece_active = 1}, {.cost = -3, .type = 'b', .color = 'B', .symbol = "♗", .piece_active = 1}},
    .Rook = {{.cost = -5, .type = 'r', .color = 'B', .symbol = "♖", .piece_active = 1, .piece_moved = 0}, {.cost = -5, .type = 'r', .color = 'B', .symbol = "♖", .piece_active = 1, .piece_moved = 0}},
    .Pawn = {
        {.cost = -1, .type = 'p', .color = 'B', .symbol = "♙", .piece_active = 1},
        {.cost = -1, .type = 'p', .color = 'B', .symbol = "♙", .piece_active = 1},
        {.cost = -1, .type = 'p', .color = 'B', .symbol = "♙", .piece_active = 1},
        {.cost = -1, .type = 'p', .color = 'B', .symbol = "♙", .piece_active = 1},
        {.cost = -1, .type = 'p', .color = 'B', .symbol = "♙", .piece_active = 1},
        {.cost = -1, .type = 'p', .color = 'B', .symbol = "♙", .piece_active = 1},
        {.cost = -1, .type = 'p', .color = 'B', .symbol = "♙", .piece_active = 1},
        {.cost = -1, .type = 'p', .color = 'B', .symbol = "♙", .piece_active = 1},
    }};

int calc_captured_pieces_cost()
{
    int total = 0;

    // White Pieces
    if (white.King.piece_active)
        total += white.King.cost;

    if (white.Queen.piece_active)
        total += white.Queen.cost;

    if (white.Knight[0].piece_active)
        total += white.Knight[0].cost;

    if (white.Knight[1].piece_active)
        total += white.Knight[1].cost;

    if (white.Bishop[0].piece_active)
        total += white.Bishop[0].cost;

    if (white.Bishop[1].piece_active)
        total += white.Bishop[1].cost;

    if (white.Rook[0].piece_active)
        total += white.Rook[0].cost;

    if (white.Rook[1].piece_active)
        total += white.Rook[1].cost;

    for (int i = 0; i < size; i++)
        if (white.Pawn[i].piece_active)
            total += white.Pawn[i].cost;

    // Black Pieces
    if (black.King.piece_active)
        total += black.King.cost;

    if (black.Queen.piece_active)
        total += black.Queen.cost;

    if (black.Knight[0].piece_active)
        total += black.Knight[0].cost;

    if (black.Knight[1].piece_active)
        total += black.Knight[1].cost;

    if (black.Bishop[0].piece_active)
        total += black.Bishop[0].cost;

    if (black.Bishop[1].piece_active)
        total += black.Bishop[1].cost;

    if (black.Rook[0].piece_active)
        total += black.Rook[0].cost;

    if (black.Rook[1].piece_active)
        total += black.Rook[1].cost;

    for (int i = 0; i < size; i++)
        if (black.Pawn[i].piece_active)
            total += black.Pawn[i].cost;

    return total;
}

void pawn_promotion(piece *piece, int to_rank, int user_inp)
{
    /*
    user_inp:
    1 -> Q
    2 -> N
    3 -> B
    4 -> R
    */

    if (piece->type != 'P' && piece->type != 'p')
        return;
    if (piece->color == 'W' && to_rank != size - 1)
        return;
    if (piece->color == 'B' && to_rank != 0)
        return;

    if (piece->color == 'W')
    {
        switch (user_inp)
        {
        case 1:
            piece->type = 'Q';
            piece->cost = 9;
            piece->symbol = "♛";
            break;
        case 2:
            piece->type = 'N';
            piece->cost = 3;
            piece->symbol = "♞";
            break;
        case 3:
            piece->type = 'B';
            piece->cost = 3;
            piece->symbol = "♝";
            break;
        case 4:
            piece->type = 'R';
            piece->cost = 5;
            piece->symbol = "♜";
            break;
        default:
            printf("Invalid Input\n");
            break;
        }
    }
    else
    {
        switch (user_inp)
        {
        case 1:
            piece->type = 'q';
            piece->cost = -9;
            piece->symbol = "♕";
            break;
        case 2:
            piece->type = 'n';
            piece->cost = -3;
            piece->symbol = "♘";
            break;
        case 3:
            piece->type = 'b';
            piece->cost = -3;
            piece->symbol = "♗";
            break;
        case 4:
            piece->type = 'r';
            piece->cost = -5;
            piece->symbol = "♖";
            break;
        default:
            printf("Invalid Input\n");
            break;
        }
    }
}

int fileIndex(char file)
{
    int temp = file - 'a';
    if (temp > size - 1 || temp < 0)
        return -1;
    return temp;
}

int rankIndex(char rank)
{
    int temp = rank - '1';
    if (temp > size - 1 || temp < 0)
        return -1;
    return temp;
}

void print_board(square board[size][size])
{
    printf("\n");
    printf("    a   b   c   d   e   f   g   h\n");
    printf("  +---+---+---+---+---+---+---+---+\n");

    for (int row = size - 1; row >= 0; row--)
    {
        printf("%d |", row + 1);
        for (int col = 0; col < size; col++)
        {
            if (board[row][col].piece != NULL)
            {
                if (use_symbols && board[row][col].piece->symbol)
                    printf(" %s |", board[row][col].piece->symbol);
                else
                    printf(" %c |", board[row][col].piece->type);
            }
            else
                printf("   |");
        }
        printf(" %d\n", row + 1);

        if (row != 0)
            printf("  +---+---+---+---+---+---+---+---+\n");
    }

    printf("  +---+---+---+---+---+---+---+---+\n");
    printf("    a   b   c   d   e   f   g   h");
    printf("\n");
}

void reset_board(square board[size][size])
{
    for (int rank = 0; rank < size; rank++)
    {
        for (int file = 0; file < size; file++)
        {
            board[rank][file].rank = rank;
            board[rank][file].file = file;
            board[rank][file].piece = NULL;
        }
    }

    // White pieces
    board[0][fileIndex('a')].piece = &white.Rook[0];
    board[0][fileIndex('b')].piece = &white.Knight[0];
    board[0][fileIndex('c')].piece = &white.Bishop[0];
    board[0][fileIndex('d')].piece = &white.Queen;
    board[0][fileIndex('e')].piece = &white.King;
    board[0][fileIndex('f')].piece = &white.Bishop[1];
    board[0][fileIndex('g')].piece = &white.Knight[1];
    board[0][fileIndex('h')].piece = &white.Rook[1];

    board[0][fileIndex('a')].piece->piece_active = 1;
    board[0][fileIndex('b')].piece->piece_active = 1;
    board[0][fileIndex('c')].piece->piece_active = 1;
    board[0][fileIndex('d')].piece->piece_active = 1;
    board[0][fileIndex('e')].piece->piece_active = 1;
    board[0][fileIndex('f')].piece->piece_active = 1;
    board[0][fileIndex('g')].piece->piece_active = 1;
    board[0][fileIndex('h')].piece->piece_active = 1;

    // Black pieces
    board[size - 1][fileIndex('a')].piece = &black.Rook[0];
    board[size - 1][fileIndex('b')].piece = &black.Knight[0];
    board[size - 1][fileIndex('c')].piece = &black.Bishop[0];
    board[size - 1][fileIndex('d')].piece = &black.Queen;
    board[size - 1][fileIndex('e')].piece = &black.King;
    board[size - 1][fileIndex('f')].piece = &black.Bishop[1];
    board[size - 1][fileIndex('g')].piece = &black.Knight[1];
    board[size - 1][fileIndex('h')].piece = &black.Rook[1];

    board[size - 1][fileIndex('a')].piece->piece_active = 1;
    board[size - 1][fileIndex('b')].piece->piece_active = 1;
    board[size - 1][fileIndex('c')].piece->piece_active = 1;
    board[size - 1][fileIndex('d')].piece->piece_active = 1;
    board[size - 1][fileIndex('e')].piece->piece_active = 1;
    board[size - 1][fileIndex('f')].piece->piece_active = 1;
    board[size - 1][fileIndex('g')].piece->piece_active = 1;
    board[size - 1][fileIndex('h')].piece->piece_active = 1;

    for (int i = 0; i < size; i++)
    {
        board[1][fileIndex('a' + i)].piece = &white.Pawn[i];
        white.Pawn[i].cost = 1;
        white.Pawn[i].symbol = "♟";
        white.Pawn[i].type = 'P';
        white.Pawn[i].piece_active = 1;

        board[size - 2][fileIndex('a' + i)].piece = &black.Pawn[i];
        black.Pawn[i].cost = -1;
        black.Pawn[i].symbol = "♙";
        black.Pawn[i].type = 'p';
        black.Pawn[i].piece_active = 1;
    }

    move_num = 1;
}

int valid_pawn_move(square board[size][size], int from_rank, int from_file, int to_rank, int to_file, square *possible_en_passant_square)
{

    if (
        from_rank == -1 ||
        from_file == -1 ||
        to_rank == -1 ||
        to_file == -1 ||
        board[from_rank][from_file].piece == NULL || (board[from_rank][from_file].piece->type != 'P' && board[from_rank][from_file].piece->type != 'p'))
        return 0;

    if (board[from_rank][from_file].piece->color == 'W' && move_num % 2 == 0)
        return 0;
    if (board[from_rank][from_file].piece->color == 'B' && move_num % 2 == 1)
        return 0;

    // straight moves
    if (from_file == to_file)
    {
        int total_moves = to_rank - from_rank; // +ve for white and -ve for black

        if (total_moves == 1 && board[from_rank][from_file].piece->color == 'W' && board[to_rank][to_file].piece == NULL)
            return 1;
        else if (total_moves == -1 && board[from_rank][from_file].piece->color == 'B' && board[to_rank][to_file].piece == NULL)
            return 1;
        else if (total_moves == 2 && board[from_rank][from_file].piece->color == 'W' && from_rank == 1 && board[from_rank + 1][from_file].piece == NULL && board[from_rank + 2][from_file].piece == NULL)
            return 3;
        else if (total_moves == -2 && board[from_rank][from_file].piece->color == 'B' && from_rank == 6 && board[from_rank - 1][from_file].piece == NULL && board[from_rank - 2][from_file].piece == NULL)
            return 3;
    }

    // Captures, Including en passant
    if (from_file - to_file == 1 || from_file - to_file == -1)
    {
        // Simple Capture
        if (board[from_rank][from_file].piece->color == 'W' &&
            to_rank - from_rank == 1 &&
            board[to_rank][to_file].piece != NULL &&
            board[to_rank][to_file].piece->color == 'B')
            return 2;

        if (board[from_rank][from_file].piece->color == 'B' &&
            to_rank - from_rank == -1 &&
            board[to_rank][to_file].piece != NULL &&
            board[to_rank][to_file].piece->color == 'W')
            return 2;

        // En Passant
        if (possible_en_passant_square != NULL &&
            (to_rank - possible_en_passant_square->rank == 1 || to_rank - possible_en_passant_square->rank == -1) &&
            (to_file == possible_en_passant_square->file) &&
            (to_file - from_file == 1 || to_file - from_file == -1))
            return 4;
    }

    return 0;
}

int valid_knight_move(square board[size][size], int from_rank, int from_file, int to_rank, int to_file)
{
    if (
        from_rank == -1 ||
        from_file == -1 ||
        to_rank == -1 ||
        to_file == -1 ||
        board[from_rank][from_file].piece == NULL ||
        (board[from_rank][from_file].piece->type != 'N' && board[from_rank][from_file].piece->type != 'n'))
        return 0;

    if (board[from_rank][from_file].piece->color == 'W' && move_num % 2 == 0)
        return 0;
    if (board[from_rank][from_file].piece->color == 'B' && move_num % 2 == 1)
        return 0;

    if ((to_rank - from_rank == 2 || to_rank - from_rank == -2) && (to_file - from_file == 1 || to_file - from_file == -1))
    {
        if (board[to_rank][to_file].piece == NULL)
            return 1;
        else if (board[to_rank][to_file].piece->color != board[from_rank][from_file].piece->color)
            return 2;
    }
    else if ((to_rank - from_rank == 1 || to_rank - from_rank == -1) && (to_file - from_file == 2 || to_file - from_file == -2))
    {
        if (board[to_rank][to_file].piece == NULL)
            return 1;
        else if (board[to_rank][to_file].piece->color != board[from_rank][from_file].piece->color)
            return 2;
    }

    return 0;
}

int valid_rook_move(square board[size][size], int from_rank, int from_file, int to_rank, int to_file, int queen_move)
{
    if (
        from_rank == -1 ||
        from_file == -1 ||
        to_rank == -1 ||
        to_file == -1 ||
        board[from_rank][from_file].piece == NULL ||
        (!queen_move && board[from_rank][from_file].piece->type != 'R' && board[from_rank][from_file].piece->type != 'r'))
        return 0;

    if (board[from_rank][from_file].piece->color == 'W' && move_num % 2 == 0)
        return 0;
    if (board[from_rank][from_file].piece->color == 'B' && move_num % 2 == 1)
        return 0;

    if (to_rank == from_rank)
    {
        if (to_file - from_file > 0)
        { // Moving Left to Right
            // Checking in between squares
            for (int i = from_file + 1; i < to_file; i++)
                if (board[from_rank][i].piece != NULL)
                    return 0;
            if (board[to_rank][to_file].piece == NULL)
                return 1;
            else if (board[to_rank][to_file].piece->color != board[from_rank][from_file].piece->color)
                return 2;
        }
        else if (to_file - from_file < 0)
        { // Moving Right to Left

            for (int i = from_file - 1; i > to_file; i--)
                if (board[from_rank][i].piece != NULL)
                    return 0;
            if (board[to_rank][to_file].piece == NULL)
                return 1;
            else if (board[to_rank][to_file].piece->color != board[from_rank][from_file].piece->color)
                return 2;
        }
    }
    else if (to_file == from_file)
    {
        if (to_rank - from_rank > 0)
        { // Moving Up
            for (int i = from_rank + 1; i < to_rank; i++)
                if (board[i][from_file].piece != NULL)
                    return 0;

            if (board[to_rank][to_file].piece == NULL)
                return 1;
            else if (board[to_rank][to_file].piece->color != board[from_rank][from_file].piece->color)
                return 2;
        }
        else if (to_rank - from_rank < 0)
        { // Moving Down
            for (int i = from_rank - 1; i > to_rank; i--)
                if (board[i][from_file].piece != NULL)
                    return 0;

            if (board[to_rank][to_file].piece == NULL)
                return 1;
            else if (board[to_rank][to_file].piece->color != board[from_rank][from_file].piece->color)
                return 2;
        }
    }

    return 0;
}

int mag(int num)
{
    if (num < 0)
        return num * -1;
    return num;
}

int valid_bishop_move(square board[size][size], int from_rank, int from_file, int to_rank, int to_file, int queen_move)
{
    if (
        from_rank == -1 ||
        from_file == -1 ||
        to_rank == -1 ||
        to_file == -1 ||
        board[from_rank][from_file].piece == NULL ||
        (!queen_move && board[from_rank][from_file].piece->type != 'B' && board[from_rank][from_file].piece->type != 'b'))
        return 0;

    if (board[from_rank][from_file].piece->color == 'W' && move_num % 2 == 0)
        return 0;
    if (board[from_rank][from_file].piece->color == 'B' && move_num % 2 == 1)
        return 0;

    int rank_diff = to_rank - from_rank;
    int file_diff = to_file - from_file;

    if (rank_diff == 0 || file_diff == 0 || mag(rank_diff) != mag(file_diff))
        return 0;

    // Top Right Movement
    if (rank_diff > 0 && file_diff > 0)
    {
        int curr_rank = from_rank + 1;
        int curr_file = from_file + 1;

        while (curr_rank < to_rank && curr_file < to_file)
        {
            if (board[curr_rank][curr_file].piece != NULL)
                return 0;

            curr_rank++;
            curr_file++;
        }

        if (board[to_rank][to_file].piece == NULL)
            return 1;
        else if (board[to_rank][to_file].piece->color != board[from_rank][from_file].piece->color)
            return 2;
    }

    // Top Left Movement
    if (rank_diff > 0 && file_diff < 0)
    {
        int curr_rank = from_rank + 1;
        int curr_file = from_file - 1;

        while (curr_rank < to_rank && curr_file > to_file)
        {
            if (board[curr_rank][curr_file].piece != NULL)
                return 0;

            curr_rank++;
            curr_file--;
        }
        if (board[to_rank][to_file].piece == NULL)
            return 1;
        else if (board[to_rank][to_file].piece->color != board[from_rank][from_file].piece->color)
            return 2;
    }

    // Bottom Left Movement
    if (rank_diff < 0 && file_diff < 0)
    {
        int curr_rank = from_rank - 1;
        int curr_file = from_file - 1;

        while (curr_rank > to_rank && curr_file > to_file)
        {
            if (board[curr_rank][curr_file].piece != NULL)
                return 0;

            curr_rank--;
            curr_file--;
        }
        if (board[to_rank][to_file].piece == NULL)
            return 1;
        else if (board[to_rank][to_file].piece->color != board[from_rank][from_file].piece->color)
            return 2;
    }

    // Bottom Righ Movement
    if (rank_diff < 0 && file_diff > 0)
    {
        int curr_rank = from_rank - 1;
        int curr_file = from_file + 1;

        while (curr_rank > to_rank && curr_file < to_file)
        {
            if (board[curr_rank][curr_file].piece != NULL)
                return 0;

            curr_rank--;
            curr_file++;
        }
        if (board[to_rank][to_file].piece == NULL)
            return 1;
        else if (board[to_rank][to_file].piece->color != board[from_rank][from_file].piece->color)
            return 2;
    }
}

int valid_queen_move(square board[size][size], int from_rank, int from_file, int to_rank, int to_file)
{
    if (
        from_rank == -1 ||
        from_file == -1 ||
        to_rank == -1 ||
        to_file == -1 ||
        board[from_rank][from_file].piece == NULL ||
        (board[from_rank][from_file].piece->type != 'Q' && board[from_rank][from_file].piece->type != 'q'))
        return 0;

    if (board[from_rank][from_file].piece->color == 'W' && move_num % 2 == 0)
        return 0;
    if (board[from_rank][from_file].piece->color == 'B' && move_num % 2 == 1)
        return 0;

    if (to_rank - from_rank == 0 || to_file - from_file == 0)
        return valid_rook_move(board, from_rank, from_file, to_rank, to_file, 1);
    else
        return valid_bishop_move(board, from_rank, from_file, to_rank, to_file, 1);
}

int in_check(square board[size][size], int king_rank, int king_file, piece *king)
{
    // Check for Pawn Check
    if (king->color == 'W' && king_rank + 1 < size)
    {
        if (king_file + 1 < size && board[king_rank + 1][king_file + 1].piece && board[king_rank + 1][king_file + 1].piece->type == 'p')
            return 1;

        if (king_file - 1 >= 0 && board[king_rank + 1][king_file - 1].piece && board[king_rank + 1][king_file - 1].piece->type == 'p')
            return 1;
    }
    else if (king->color == 'B' && king_rank - 1 >= 0)
    {
        if (king_file + 1 < size && board[king_rank - 1][king_file + 1].piece && board[king_rank - 1][king_file + 1].piece->type == 'P')
            return 1;

        if (king_file - 1 >= 0 && board[king_rank - 1][king_file - 1].piece && board[king_rank - 1][king_file - 1].piece->type == 'P')
            return 1;
    }

    // Check Up
    for (int temp_rank = king_rank + 1; temp_rank < size; temp_rank++)
    {
        square *temp_square = &board[temp_rank][king_file];

        if (temp_square->piece)
        {
            if (temp_square->piece->color == king->color && temp_square->piece->type != king->type)
                break;

            if (king->color == 'W' && (temp_square->piece->type == 'q' || temp_square->piece->type == 'r'))
                return 1;

            if (king->color == 'B' && (temp_square->piece->type == 'Q' || temp_square->piece->type == 'R'))
                return 1;
        }
    }

    // Check Down
    for (int temp_rank = king_rank - 1; temp_rank >= 0; temp_rank--)
    {
        square *temp_square = &board[temp_rank][king_file];

        if (temp_square->piece)
        {
            if (temp_square->piece->color == king->color && temp_square->piece->type != king->type)
                break;

            if (king->color == 'W' && (temp_square->piece->type == 'q' || temp_square->piece->type == 'r'))
                return 1;

            if (king->color == 'B' && (temp_square->piece->type == 'Q' || temp_square->piece->type == 'R'))
                return 1;
        }
    }

    // Check Right
    for (int temp_file = king_file + 1; temp_file < size; temp_file++)
    {
        square *temp_square = &board[king_rank][temp_file];

        if (temp_square->piece)
        {
            if (temp_square->piece->color == king->color && temp_square->piece->type != king->type)
                break;

            if (king->color == 'W' && (temp_square->piece->type == 'q' || temp_square->piece->type == 'r'))
                return 1;

            if (king->color == 'B' && (temp_square->piece->type == 'Q' || temp_square->piece->type == 'R'))
                return 1;
        }
    }

    // Check Left
    for (int temp_file = king_file - 1; temp_file >= 0; temp_file--)
    {
        square *temp_square = &board[king_rank][temp_file];

        if (temp_square->piece)
        {
            if (temp_square->piece->color == king->color && temp_square->piece->type != king->type)
                break;

            if (king->color == 'W' && (temp_square->piece->type == 'q' || temp_square->piece->type == 'r'))
                return 1;

            if (king->color == 'B' && (temp_square->piece->type == 'Q' || temp_square->piece->type == 'R'))
                return 1;
        }
    }

    // Check Top Left
    int curr_rank = king_rank + 1;
    int curr_file = king_file + 1;

    while (curr_rank < size && curr_file < size)
    {
        square *temp_square = &board[curr_rank][curr_file];

        if (temp_square->piece)
        {
            if (temp_square->piece->color == king->color && temp_square->piece->type != king->type)
                break;

            if (king->color == 'W' && (temp_square->piece->type == 'q' || temp_square->piece->type == 'b'))
                return 1;

            if (king->color == 'B' && (temp_square->piece->type == 'Q' || temp_square->piece->type == 'B'))
                return 1;
        }

        curr_rank++;
        curr_file++;
    }

    // Check Top Right
    curr_rank = king_rank + 1;
    curr_file = king_file - 1;

    while (curr_rank < size && curr_file >= 0)
    {
        square *temp_square = &board[curr_rank][curr_file];

        if (temp_square->piece)
        {
            if (temp_square->piece->color == king->color && temp_square->piece->type != king->type)
                break;

            if (king->color == 'W' && (temp_square->piece->type == 'q' || temp_square->piece->type == 'b'))
                return 1;

            if (king->color == 'B' && (temp_square->piece->type == 'Q' || temp_square->piece->type == 'B'))
                return 1;
        }

        curr_rank++;
        curr_file--;
    }

    // Check Bottom Right
    curr_rank = king_rank - 1;
    curr_file = king_file - 1;

    while (curr_rank >= 0 && curr_file >= 0)
    {
        square *temp_square = &board[curr_rank][curr_file];

        if (temp_square->piece)
        {
            if (temp_square->piece->color == king->color && temp_square->piece->type != king->type)
                break;

            if (king->color == 'W' && (temp_square->piece->type == 'q' || temp_square->piece->type == 'b'))
                return 1;

            if (king->color == 'B' && (temp_square->piece->type == 'Q' || temp_square->piece->type == 'B'))
                return 1;
        }

        curr_rank--;
        curr_file--;
    }

    // Check Bottom Left
    curr_rank = king_rank - 1;
    curr_file = king_file + 1;

    while (curr_rank >= 0 && curr_file < size)
    {
        square *temp_square = &board[curr_rank][curr_file];

        if (temp_square->piece)
        {
            if (temp_square->piece->color == king->color && temp_square->piece->type != king->type)
                break;

            if (king->color == 'W' && (temp_square->piece->type == 'q' || temp_square->piece->type == 'b'))
                return 1;

            if (king->color == 'B' && (temp_square->piece->type == 'Q' || temp_square->piece->type == 'B'))
                return 1;
        }

        curr_rank--;
        curr_file++;
    }

    return 0;
}

square *get_king_position(square board[size][size], char king_type)
{
    if (king_type == 'K')
    {
        for (int rank = 0; rank < size; rank++)
            for (int file = 0; file < size; file++)
                if (board[rank][file].piece->type = king_type)
                    return &board[rank][file];
    }
    else if (king_type == 'k')
    {
        for (int rank = size - 1; rank >= 0; rank++)
            for (int file = 0; file < size; file++)
                if (board[rank][file].piece->type = king_type)
                    return &board[rank][file];
    }
    else
    {
        printf("Invalid Input\n");
        return NULL;
    }
}

int valid_king_move(square board[size][size], int from_rank, int from_file, int to_rank, int to_file)
{
    if (
        from_rank == -1 ||
        from_file == -1 ||
        to_rank == -1 ||
        to_file == -1 ||
        board[from_rank][from_file].piece == NULL ||
        (board[from_rank][from_file].piece->type != 'K' && board[from_rank][from_file].piece->type != 'k'))
        return 0;

    if (board[from_rank][from_file].piece->color == 'W' && move_num % 2 == 0)
        return 0;
    if (board[from_rank][from_file].piece->color == 'B' && move_num % 2 == 1)
        return 0;

    int rank_diff = to_rank - from_rank;
    int file_diff = to_file - from_file;

    // Castle king side
    if (
        board[from_rank][from_file].piece->color == 'W' &&
        from_rank == 0 &&
        to_rank == 0 &&
        from_file == 4 &&
        to_file == 6 &&
        !white.King.piece_moved &&
        !white.Rook[1].piece_moved &&
        board[0][5].piece == NULL &&
        board[0][6].piece == NULL && !in_check(board, 0, 4, board[from_rank][from_file].piece) &&
        !in_check(board, 0, 5, board[from_rank][from_file].piece) &&
        !in_check(board, 0, 6, board[from_rank][from_file].piece))
        return 5;

    if (
        board[from_rank][from_file].piece->color == 'B' &&
        from_rank == 7 &&
        to_rank == 7 &&
        from_file == 4 &&
        to_file == 6 &&
        !black.King.piece_moved &&
        !black.Rook[1].piece_moved &&
        board[7][5].piece == NULL &&
        board[7][6].piece == NULL &&
        !in_check(board, 7, 4, board[from_rank][from_file].piece) &&
        !in_check(board, 7, 5, board[from_rank][from_file].piece) &&
        !in_check(board, 7, 6, board[from_rank][from_file].piece))
        return 5;

    // Castle Queen Side
    if (
        board[from_rank][from_file].piece->color == 'W' &&
        from_rank == 0 &&
        to_rank == 0 &&
        from_file == 4 &&
        to_file == 2 &&
        !white.King.piece_moved &&
        !white.Rook[0].piece_moved &&
        board[0][1].piece == NULL &&
        board[0][2].piece == NULL &&
        board[0][3].piece == NULL &&
        !in_check(board, 0, 1, board[from_rank][from_file].piece) &&
        !in_check(board, 0, 2, board[from_rank][from_file].piece) &&
        !in_check(board, 0, 3, board[from_rank][from_file].piece) &&
        !in_check(board, 0, 4, board[from_rank][from_file].piece))
        return 6;

    if (
        board[from_rank][from_file].piece->color == 'B' &&
        from_rank == 7 &&
        to_rank == 7 &&
        from_file == 4 &&
        to_file == 2 &&
        !black.King.piece_moved &&
        !black.Rook[0].piece_moved &&
        board[7][1].piece == NULL &&
        board[7][2].piece == NULL &&
        board[7][3].piece == NULL &&
        !in_check(board, 7, 1, board[from_rank][from_file].piece) &&
        !in_check(board, 7, 2, board[from_rank][from_file].piece) &&
        !in_check(board, 7, 3, board[from_rank][from_file].piece) &&
        !in_check(board, 7, 4, board[from_rank][from_file].piece))
        return 6;

    if (
        (rank_diff != 1 &&
         rank_diff != -1 &&
         rank_diff != 0) ||

        (file_diff != 1 &&
         file_diff != -1 &&
         file_diff != 0))
        return 0;

    if (in_check(board, to_rank, to_file, board[from_rank][from_file].piece))
        return 0;
    else
    {
        if (
            board[to_rank][to_file].piece == NULL)
            return 1;
        else if (board[to_rank][to_file].piece->color != board[from_rank][from_file].piece->color)
            return 2;
    }

    return 0;
}

int move_directly(square board[size][size], char *move)
{
    int move_len = strlen(move);

    if (move_len == 4)
    {
        int from_rank = rankIndex(move[1]);
        int from_file = fileIndex(move[0]);
        int to_rank = rankIndex(move[3]);
        int to_file = fileIndex(move[2]);

        if (from_rank != -1 &&
            from_file != -1 &&
            to_rank != -1 &&
            to_file != -1)
        {
            if (board[from_rank][from_file].piece == NULL)
                return 0;

            if (board[to_rank][to_file].piece != NULL)
            {
                if (board[to_rank][to_file].piece->color == board[from_rank][from_file].piece->color)
                    return 0;
                return 2;
            }
            return 1;
            // board[to_rank][to_file].piece = board[from_rank][from_file].piece;
            // board[from_rank][from_file].piece = NULL;
        }
    }

    return 0;
}

void piece_move(square board[size][size], char *move)
{
    static square *possible_en_passant_square = NULL;

    if (strcmp(move, "reset") == 0)
    {
        reset_board(board);
        system("cls");
        return;
    }

    else if (strcmp(move, "stop") == 0 || strcmp(move, "exit") == 0)
    {
        for (int i = 3; i > 0; i--)
        {
            system("cls");
            printf("Exiting in %ds\n", i);
            sleep(1);
        }
        system("cls");
        printf("Exiting in 0s\n");
        exit(0);
    }

    int move_len = strlen(move),
        valid_move = 0,
        /*
        valid_move:
        0 -> Invalid Move
        1 -> Valid Move by any piece
        2 -> Valid Capture by any piece
        3 -> Two Moves by pawn
        4 -> Valid Capture by En passant
        5 -> Castle King Side
        6 -> Castle Queen Side
        */
        from_rank,
        from_file,
        to_rank,
        to_file;

    if (move_len == 4)
    {
        from_rank = rankIndex(move[1]);
        from_file = fileIndex(move[0]);
        to_rank = rankIndex(move[3]);
        to_file = fileIndex(move[2]);

        if (
            from_file != -1 &&
            from_rank != -1 &&
            to_file != -1 &&
            to_rank != -1 &&
            board[from_rank][from_file].piece != NULL)
        {
            char current_piece_type = board[from_rank][from_file].piece->type;

            if (current_piece_type == 'P' || current_piece_type == 'p')
            {

                valid_move = valid_pawn_move(board, from_rank, from_file, to_rank, to_file, possible_en_passant_square);

                if (valid_move == 3)
                    possible_en_passant_square = &board[to_rank][to_file];
                else if (valid_move == 0)
                {
                }
                else
                    possible_en_passant_square = NULL;

                if (valid_move && (to_rank == 0 || to_rank == size - 1))
                {
                    int user_choice = -1;

                    while (user_choice < 1 || user_choice > 4)
                    {
                        printf("\nPromotion Choice:\n");
                        printf("1. Queen\n2. Knight\n3. Bishop\n4. Rook\n\nEnter your choice: ");
                        scanf("%d", &user_choice);

                        if (user_choice < 1 || user_choice > 4)
                            printf("\nInvalid Input, Try Again\n");
                    }

                    pawn_promotion(board[from_rank][from_file].piece, to_rank, user_choice);
                }
            }
            else if (current_piece_type == 'N' || current_piece_type == 'n')
                valid_move = valid_knight_move(board, from_rank, from_file, to_rank, to_file);
            else if (current_piece_type == 'R' || current_piece_type == 'r')
            {
                valid_move = valid_rook_move(board, from_rank, from_file, to_rank, to_file, 0);
                if (valid_move)
                    board[from_rank][from_file].piece->piece_moved = 1;
            }
            else if (current_piece_type == 'B' || current_piece_type == 'b')
                valid_move = valid_bishop_move(board, from_rank, from_file, to_rank, to_file, 0);
            else if (current_piece_type == 'Q' || current_piece_type == 'q')
                valid_move = valid_queen_move(board, from_rank, from_file, to_rank, to_file);
            else if (current_piece_type == 'K' || current_piece_type == 'k')
            {
                valid_move = valid_king_move(board, from_rank, from_file, to_rank, to_file);

                if (valid_move)
                    board[from_rank][from_file].piece->piece_moved = 1;

                if (valid_move == 5 && board[from_rank][from_file].piece->color == 'W')
                    white.Rook[1].piece_moved = 1;

                if (valid_move == 6 && board[from_rank][from_file].piece->color == 'W')
                    white.Rook[0].piece_moved = 1;

                if (valid_move == 5 && board[from_rank][from_file].piece->color == 'B')
                    black.Rook[1].piece_moved = 1;

                if (valid_move == 6 && board[from_rank][from_file].piece->color == 'B')
                    black.Rook[0].piece_moved = 1;
            }
        }
    }

    system("cls");
    if (valid_move == 1 || valid_move == 3)
    {
        // printf("Valid Move\n");
        if (board[to_rank][to_file].piece != NULL)
        {
            printf("Invalid Move\n");
            return;
        }
        board[to_rank][to_file].piece = board[from_rank][from_file].piece;
        board[from_rank][from_file].piece = NULL;
        move_num++;
    }
    else if (valid_move == 2)
    {
        // printf("Valid Capture\n");
        board[to_rank][to_file].piece->piece_active = 0;
        board[to_rank][to_file].piece = board[from_rank][from_file].piece;
        board[from_rank][from_file].piece = NULL;
        move_num++;
    }
    else if (valid_move == 4)
    {
        if (to_rank - from_rank == 1)
        {
            board[to_rank - 1][to_file].piece->piece_active = 0;
            board[to_rank - 1][to_file].piece = NULL;
        }
        else if (to_rank - from_rank == -1)
        {
            board[to_rank + 1][to_file].piece->piece_active = 0;
            board[to_rank + 1][to_file].piece = NULL;
        }
        else
        {
            printf("Invaild Move\n");
            return;
        }

        board[to_rank][to_file].piece = board[from_rank][from_file].piece;
        board[from_rank][from_file].piece = NULL;
        move_num++;
    }
    else if (valid_move == 5)
    {
        // printf("Inside King Side Castling\n");
        board[from_rank][6].piece = board[from_rank][4].piece;
        board[from_rank][4].piece = NULL;

        board[from_rank][5].piece = board[from_rank][7].piece;
        board[from_rank][7].piece = NULL;
        move_num++;
    }
    else if (valid_move == 6)
    {
        // printf("Inside Queen Side Castling\n");
        board[from_rank][2].piece = board[from_rank][4].piece;
        board[from_rank][4].piece = NULL;

        board[from_rank][3].piece = board[from_rank][0].piece;
        board[from_rank][0].piece = NULL;
        move_num++;
    }

    else
    {

        printf("Invalid Move(%s) by ", move);
        if (move_num % 2)
            printf("White\n");
        else
            printf("Black\n");
    }
}

void main()
{
    SetConsoleOutputCP(65001);
    system("cls");
    printf("Use Symbols or Letters?\n\n");
    printf("Letters(0): K  Q  R  N  B  P\n");
    printf("Symbols(1): ♚  ♛  ♜  ♞  ♝  ♟\n");
    printf("\nNote: Only choose symbols if they are properly visible on your system.\n");
    printf("\nYour Response: ");
    scanf("%d", &use_symbols);
    system("cls");
    square board[size][size];
    reset_board(board);

    while (1)
    {
        char usr_inp;
        print_board(board);
        printf("\nCurrent Score: %d\n", calc_captured_pieces_cost());

        printf("\n");
        if (move_num % 2)
            printf("Enter White's Move: ");
        else
            printf("Enter Black's Move: ");

        scanf("%s", &usr_inp);
        piece_move(board, &usr_inp);
    }
}

// Sample Game: e2e4 e7e5 g1f3 g8f6 f3e5 d7d6 e5f3 f6e4 d1e2
// Pawn Promotion: e2e4 d7d5 e4d5 c7c6 d5c6 e7e5 c6b7 e8e7 b7a8
// King Side Castling: e2e4 e7e5 g1f3 b8c6 f1c4 g8f6 e1g1
// Queen Side Castling: e2e4 e7e5 d2d4 d7d5 b1c3 b8c6 c1e3 c8e6 b1c3 b8c6 d1d2 d8d7 e1c1