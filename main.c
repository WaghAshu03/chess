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

    if (rank_diff == 0 || file_diff == 0)
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

    int move_len = strlen(move),
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
            else if (current_piece_type == 'N' || current_piece_type == 'n')
                valid_move = valid_knight_move(board, from_rank, from_file, to_rank, to_file);
            else if (current_piece_type == 'R' || current_piece_type == 'r')
                valid_move = valid_rook_move(board, from_rank, from_file, to_rank, to_file, 0);
            else if (current_piece_type == 'B' || current_piece_type == 'b')
                valid_move = valid_bishop_move(board, from_rank, from_file, to_rank, to_file, 0);
            else if (current_piece_type == 'Q' || current_piece_type == 'q')
                valid_move = valid_queen_move(board, from_rank, from_file, to_rank, to_file);
            else if (current_piece_type == 'K' || current_piece_type == 'k')
                valid_move = move_directly(board, move);
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
    printf("\nBlack Score: %d\n", calc_captured_pieces_cost());

    while (1)
    {
        char usr_inp;
        print_board(board);
        printf("\nWhite Score: %d\n", calc_captured_pieces_cost() * -1);

        printf("\n");
        if (move_num % 2)
            printf("Enter White's Move: ");
        else
            printf("Enter Black's Move: ");

        scanf("%s", &usr_inp);
        system("cls");
        piece_move(board, &usr_inp);

        printf("\nBlack Score: %d\n", calc_captured_pieces_cost());
    }
}