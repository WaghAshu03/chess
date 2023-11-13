#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define size 8

int move_num = 1;

void repeat_chr(char c, int num)
{
    for (int i = 0; i < num; i++)
    {
        printf("%c", c);
    }
}

typedef struct piece
{
    int cost;
    char type;
    char color;
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
    .King = {.cost = 900, .type = 'K', .color = 'W'},
    .Queen = {.cost = 9, .type = 'Q', .color = 'W'},
    .Knight = {{.cost = 3, .type = 'N', .color = 'W'}, {.cost = 3, .type = 'N', .color = 'W'}},
    .Bishop = {{.cost = 3, .type = 'B', .color = 'W'}, {.cost = 3, .type = 'B', .color = 'W'}},
    .Rook = {{.cost = 5, .type = 'R', .color = 'W'}, {.cost = 5, .type = 'R', .color = 'W'}},
    .Pawn = {
        {.cost = 1, .type = 'P', .color = 'W'},
        {.cost = 1, .type = 'P', .color = 'W'},
        {.cost = 1, .type = 'P', .color = 'W'},
        {.cost = 1, .type = 'P', .color = 'W'},
        {.cost = 1, .type = 'P', .color = 'W'},
        {.cost = 1, .type = 'P', .color = 'W'},
        {.cost = 1, .type = 'P', .color = 'W'},
        {.cost = 1, .type = 'P', .color = 'W'},
    }};

piece_side black = {
    .King = {.cost = -900, .type = 'k', .color = 'B'},
    .Queen = {.cost = -9, .type = 'q', .color = 'B'},
    .Knight = {{.cost = -3, .type = 'n', .color = 'B'}, {.cost = 3, .type = 'n', .color = 'B'}},
    .Bishop = {{.cost = -3, .type = 'b', .color = 'B'}, {.cost = 3, .type = 'b', .color = 'B'}},
    .Rook = {{.cost = -5, .type = 'r', .color = 'B'}, {.cost = 5, .type = 'r', .color = 'B'}},
    .Pawn = {
        {.cost = -1, .type = 'p', .color = 'B'},
        {.cost = -1, .type = 'p', .color = 'B'},
        {.cost = -1, .type = 'p', .color = 'B'},
        {.cost = -1, .type = 'p', .color = 'B'},
        {.cost = -1, .type = 'p', .color = 'B'},
        {.cost = -1, .type = 'p', .color = 'B'},
        {.cost = -1, .type = 'p', .color = 'B'},
        {.cost = -1, .type = 'p', .color = 'B'},
    }};

piece *captured_pieces[size * 4] = {NULL};

int calc_captured_pieces_cost()
{
    int total = 0;
    for (int i = 0; i < size * 4; i++)
    {
        if (captured_pieces[i] == NULL)
            break;
        total +=
            captured_pieces[i]->cost;
    }
    return total;
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
                printf(" %c |", board[row][col].piece->type);
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

    // Black pieces
    board[size - 1][fileIndex('a')].piece = &black.Rook[0];
    board[size - 1][fileIndex('b')].piece = &black.Knight[0];
    board[size - 1][fileIndex('c')].piece = &black.Bishop[0];
    board[size - 1][fileIndex('d')].piece = &black.Queen;
    board[size - 1][fileIndex('e')].piece = &black.King;
    board[size - 1][fileIndex('f')].piece = &black.Bishop[1];
    board[size - 1][fileIndex('g')].piece = &black.Knight[1];
    board[size - 1][fileIndex('h')].piece = &black.Rook[1];

    for (int i = 0; i < size; i++)
    {
        board[1][fileIndex('a' + i)].piece = &white.Pawn[i];
        board[size - 2][fileIndex('a' + i)].piece = &black.Pawn[i];
    }
}

int valid_pawn_move(
    square board[size][size],
    int from_rank,
    int from_file,
    int to_rank,
    int to_file,
    square *possible_en_passant_square)
{

    if (
        from_rank == -1 ||
        from_file == -1 ||
        to_rank == -1 ||
        to_file == -1 ||
        board[from_rank][from_file].piece == NULL || (board[from_rank][from_file].piece->type != 'P' && board[from_rank][from_file].piece->type != 'p'))
    {
        printf("Initial Invalid\n");
        return 0;
    }

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

void move_directly(square board[size][size], char *move)
{
    int move_len = 0;

    while (move[move_len] != '\0')
        move_len++;

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
            board[to_rank][to_file].piece = board[from_rank][from_file].piece;
            board[from_rank][from_file].piece = NULL;
        }
    }
    else
        printf("Invalid Move\n");
}

void piece_move(square board[size][size], char *move)
{
    static square *possible_en_passant_square = NULL;

    int move_len = 0,
        valid_move = 0,
        /*
        valid_move:
        0 -> Invalid Move
        1 -> Valid Move by any piece
        2 -> Valid Capture by any piece
        3 -> Two Moves by pawn
        4 -> Valid Capture by En passant
        */
        from_rank,
        from_file,
        to_rank,
        to_file;

    while (move[move_len] != '\0')
        move_len++;

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
            }
        }
    }

    if (valid_move == 1 || valid_move == 3)
    {
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
        for (int i = 0; i < size * 4; i++)
            if (captured_pieces[i] == NULL)
            {
                captured_pieces[i] = board[to_rank][to_file].piece;
                break;
            }

        board[to_rank][to_file].piece = board[from_rank][from_file].piece;
        board[from_rank][from_file].piece = NULL;
        move_num++;
    }
    else if (valid_move == 4)
    {
        if (to_rank - from_rank == 1)
        {
            for (int i = 0; i < size * 4; i++)
                if (captured_pieces[i] == NULL)
                {
                    captured_pieces[i] = board[to_rank - 1][to_file].piece;
                    break;
                }
            board[to_rank - 1][to_file].piece = NULL;
        }
        else if (to_rank - from_rank == -1)
        {
            for (int i = 0; i < size * 4; i++)
                if (captured_pieces[i] == NULL)
                {
                    captured_pieces[i] = board[to_rank + 1][to_file].piece;
                    break;
                }
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

    square board[size][size];
    reset_board(board);
    system("cls");
    printf("\nCurrent Score: %d\n", calc_captured_pieces_cost());

    while (1)
    {
        char usr_inp;
        print_board(board);
        printf("\n");
        if (move_num % 2)
            printf("Enter White's Move: ");
        else
            printf("Enter Black's Move: ");

        scanf("%s", &usr_inp);
        system("cls");
        piece_move(board, &usr_inp);

        printf("\nCurrent Score: %d\n", calc_captured_pieces_cost());
    }
}