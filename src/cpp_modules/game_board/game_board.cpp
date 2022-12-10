
#include "game_board.hpp"

#include <iostream>

using namespace board_components;

GameBoard::GameBoard() : move_calculator_{MoveCalculator(board_map_)} {
    board_map_ = kStartingBoard;
}

Piece_t GameBoard::GetOccupant(BoardSpace space) {
    return board_map_[space.rank][space.file];
}

ExecutedMove GameBoard::ExecuteMove(Move move) {
    auto moving_piece = GetOccupant(move.start);
    auto destination_piece = GetOccupant(move.end);
    SetOccupant(move.end, moving_piece);
    SetOccupant(move.start, static_cast<Piece_t>(PieceType::kNnn));

    return ExecutedMove{move, moving_piece, destination_piece};
}

void GameBoard::UndoMove(ExecutedMove executed_move) {
    SetOccupant(executed_move.spaces.start, executed_move.moving_piece);
    SetOccupant(executed_move.spaces.end, executed_move.destination_piece);
}

vector<BoardSpace> GameBoard::GetAllSpacesOccupiedBy(PieceColor color) {
    return move_calculator_.utils_.GetAllSpacesOccupiedBy(color);
}

void GameBoard::SetOccupant(BoardSpace space, Piece_t piece) {
    board_map_[space.rank][space.file] = piece;
}

bool GameBoard::IsInCheck(PieceColor color) {
    auto gen_position = move_calculator_.utils_.GetGeneralPosition(color);
    auto opponent_moves = move_calculator_.CalcAllMovesNoCheckTest(opponent_of(color));
    return move_calculator_.utils_.IsSpaceAnyDestinationOfMoves(
        gen_position, opponent_moves);
}

vector<Move> GameBoard::CalcFinalMovesOf(PieceColor color) {
    auto un_tested_moves = move_calculator_.CalcAllMovesNoCheckTest(color);
    vector<Move> validated_moves;
    validated_moves.reserve(un_tested_moves.size());

    for (auto move : un_tested_moves) {
        auto executed_move = ExecuteMove(move);
        auto resulting_opponent_moves =
            move_calculator_.CalcAllMovesNoCheckTest(opponent_of(color));
        auto resulting_gen_position =
            move_calculator_.utils_.GetGeneralPosition(color);

        if (not move_calculator_.utils_.IsSpaceAnyDestinationOfMoves(
                resulting_gen_position, resulting_opponent_moves)) {
            validated_moves.emplace_back(move);
        }

        UndoMove(executed_move);
    }
    return validated_moves;
}

PieceColor GameBoard::GetColor(BoardSpace space) {
    return move_calculator_.utils_.GetColor(space);
}

PieceType GameBoard::GetType(BoardSpace space) {
    return move_calculator_.utils_.GetType(space);
}
