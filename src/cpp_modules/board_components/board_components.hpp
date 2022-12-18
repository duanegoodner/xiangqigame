
#ifndef _BOARD_COMPONENTS_
#define _BOARD_COMPONENTS_

#include <array>
#include <cassert>
#include <random>
#include <vector>

using namespace std;

namespace board_components
{

    namespace game_piece
    {

        typedef int Piece_t;

        enum PieceType : int
        {
            kNnn = 0,
            kGen = 1,
            kAdv = 2,
            kEle = 3,
            kHor = 4,
            kCha = 5,
            kCan = 6,
            kSol = 7
        };

        enum PieceColor : int
        {
            kRed = -1,
            kNul = 0,
            kBlk = 1
        };

        const int kNumPieceTypeVals = 8;

        inline PieceColor opponent_of(PieceColor color)
        {
            return static_cast<PieceColor>(-1 * color);
        }

    } // namespace board_components::game_piece

    namespace data_types
    {

        using namespace board_components::game_piece;

        typedef int Piece_t;
        typedef int BoardIdx_t;
        const BoardIdx_t kNumRanks = 10;
        const BoardIdx_t kNumFiles = 9;

        typedef array<array<Piece_t, kNumFiles>, kNumRanks> BoardMap_t;

    }  // namespace board_components::data_types

    namespace locations
    {

        using namespace board_components::data_types;
        using namespace board_components::game_piece;

        const BoardIdx_t kRedRiverEdge = 5;
        const BoardIdx_t kBlackRiverEdge = 4;

        struct CastleEdges
        {
            BoardIdx_t min_rank;
            BoardIdx_t max_rank;
            BoardIdx_t min_file;
            BoardIdx_t max_file;
        };

        const CastleEdges kRedCastleEdges = {7, 9, 3, 5};
        const CastleEdges kBlackCastleEdges = {0, 2, 3, 5};

        struct BoardDirection
        {
            BoardIdx_t rank, file;
        };

        struct BoardSpace
        {

            BoardIdx_t rank, file;

            bool IsOnBoard()
            {
                return ((0 <= rank) && (rank < kNumRanks) && (0 <= file) && (file < kNumFiles));
            }

            bool IsInHomelandOf(PieceColor color)
            {
                assert(color != PieceColor::kNul);
                if (color == PieceColor::kRed)
                {
                    return rank >= kRedRiverEdge;
                }
                else
                {
                    return rank <= kBlackRiverEdge;
                }
            }

            bool IsInCastleOf(PieceColor color)
            {
                assert(color != PieceColor::kNul);
                auto castle_edges =
                    color == PieceColor::kRed ? kRedCastleEdges : kBlackCastleEdges;
                return (castle_edges.min_rank <= rank) && (rank <= castle_edges.max_rank) &&
                       (castle_edges.min_file <= file) && (file <= castle_edges.max_file);
            }

            BoardSpace operator+(const BoardDirection direction)
            {
                return BoardSpace{rank + direction.rank, file + direction.file};
            }

            bool operator==(const BoardSpace other)
            {
                return (rank == other.rank) && (file == other.file);
            }

            bool operator!=(const BoardSpace other)
            {
                return (rank != other.rank) || (file != other.file);
            }
        };

    }  // namespace board_components::locations

    namespace piece_move
    {

        using namespace board_components::data_types;
        using namespace board_components::game_piece;
        using namespace board_components::locations;

        struct Move
        {
            BoardSpace start;
            BoardSpace end;

            bool operator==(const Move other)
            {
                return (start == other.start) && (end == other.end);
            }
        };

        struct MoveCollection
        {
            vector<Move> moves;
            MoveCollection() : moves{} {};
            MoveCollection(vector<Move> my_moves) : moves{my_moves} {};
            MoveCollection(size_t reserve_size) : moves{}
            {
                moves.reserve(reserve_size);
            }
            bool Contains(Move move)
            {
                for (auto entry : moves)
                {
                    if ((move.start == entry.start) && (move.end == entry.end))
                    {
                        return true;
                    }
                }
                return false;
            }
            void Append(Move move)
            {
                moves.emplace_back(move);
            }
            void Concat(vector<Move> other_moves)
            {
                moves.insert(moves.end(), other_moves.begin(), other_moves.end());
            }
            void Concat(MoveCollection other)
            {
                moves.insert(moves.end(), other.moves.begin(), other.moves.end());
            }
        };

        struct ExecutedMove
        {
            Move spaces;
            Piece_t moving_piece;
            Piece_t destination_piece;
        };

    } // namespace board_components::piece_move

} // namespace board_components

#endif // _SHARED_COMPONENTS_