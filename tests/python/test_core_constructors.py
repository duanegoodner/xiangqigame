from typing import Callable

import xiangqi_bindings as bindings
import pytest


@pytest.fixture
def game_board() -> bindings.GameBoard:
    return bindings.GameBoard()


@pytest.fixture
def piece_position_points() -> bindings.PiecePositionPoints:
    return bindings.PiecePositionPoints()


@pytest.fixture
def zobrist_calculator_constructors() -> dict[
    int,
    Callable[
        ...,
        bindings.ZobristCalculator32
        | bindings.ZobristCalculator64
        | bindings.ZobristCalculator128,
    ],
]:
    return {
        32: bindings.ZobristCalculator32,
        64: bindings.ZobristCalculator64,
        128: bindings.ZobristCalculator128,
    }


@pytest.fixture
def zobrist_component_constructors() -> dict[
    int,
    Callable[
        ...,
        bindings.ZobristComponentNew32
        | bindings.ZobristComponentNew64
        | bindings.ZobristComponentNew128,
    ],
]:
    return {
        32: bindings.ZobristComponentNew32,
        64: bindings.ZobristComponentNew64,
        128: bindings.ZobristComponentNew128,
    }


@pytest.fixture
def transposition_table_constructors() -> dict[
    int,
    Callable[
        ...,
        bindings.TranspositionTable32
        | bindings.TranspositionTable64
        | bindings.TranspositionTable128,
    ],
]:
    return {
        32: bindings.TranspositionTable32,
        64: bindings.TranspositionTable64,
        128: bindings.TranspositionTable128,
    }


@pytest.fixture
def transposition_table_pruner_constructors() -> dict[
    int,
    Callable[
        ...,
        bindings.TranspositionTablePruner32
        | bindings.TranspositionTablePruner64
        | bindings.TranspositionTablePruner128,
    ],
]:
    return {
        32: bindings.TranspositionTablePruner32,
        64: bindings.TranspositionTablePruner64,
        128: bindings.TranspositionTablePruner128,
    }


@pytest.fixture
def zobrist_coordinator_constructors() -> dict[
    int,
    Callable[
        ...,
        bindings.ZobristCoordinatorNew32
        | bindings.ZobristCoordinatorNew64
        | bindings.ZobristCoordinatorNew128,
    ],
]:
    return {
        32: bindings.ZobristCoordinatorNew32,
        64: bindings.ZobristCoordinatorNew64,
        128: bindings.ZobristCoordinatorNew128,
    }


@pytest.fixture
def zobrist_components(
    zobrist_calculator_constructors, zobrist_component_constructors
) -> dict[
    int,
    bindings.ZobristComponentNew32
    | bindings.ZobristComponentNew64
    | bindings.ZobristComponentNew128,
]:
    result = {}
    for (
        bit_size,
        calculator_constructor,
    ) in zobrist_calculator_constructors.items():
        primary_calculator = calculator_constructor()
        confirmation_calculators = [calculator_constructor()]
        result[bit_size] = zobrist_component_constructors[bit_size](
            primary_calculator=primary_calculator,
            confirmation_calculators=confirmation_calculators,
        )

    return {
        32: bindings.ZobristComponentNew32(
            primary_calculator=bindings.ZobristCalculator32(),
            confirmation_calculators=[bindings.ZobristCalculator32()],
        ),
        64: bindings.ZobristComponentNew64(
            primary_calculator=bindings.ZobristCalculator64(),
            confirmation_calculators=[bindings.ZobristCalculator64()],
        ),
        128: bindings.ZobristComponentNew128(
            primary_calculator=bindings.ZobristCalculator128(),
            confirmation_calculators=[bindings.ZobristCalculator128()],
        ),
    }


@pytest.fixture
def transposition_table_pruners(
    transposition_table_constructors, transposition_table_pruner_constructors
):
    result = {}
    for (
        bit_size,
        transposition_table_constructor,
    ) in transposition_table_constructors.items():
        tr_table_guard = bindings.TranspositionTableGuard()
        tr_table = transposition_table_constructor()
        result[bit_size] = transposition_table_pruner_constructors[bit_size](
            tr_table=tr_table, tr_table_guard=tr_table_guard
        )
    return result


@pytest.fixture
def pre_search_move_sorter(
    game_board, piece_position_points
) -> bindings.PreSearchMoveSorter:
    return bindings.PreSearchMoveSorter(
        game_board=game_board, game_position_points=piece_position_points
    )


@pytest.fixture
def zobrist_coordinators(
    zobrist_coordinator_constructors,
    zobrist_components,
    transposition_table_constructors,
    transposition_table_pruner_constructors,
) -> dict[
    int,
    bindings.ZobristCoordinatorNew32
    | bindings.ZobristCoordinatorNew64
    | bindings.ZobristCoordinatorNew128,
]:
    result = {}
    for (
        bit_size,
        coordinator_constructor,
    ) in zobrist_coordinator_constructors.items():
        zobrist_component = zobrist_components[bit_size]
        tr_table = transposition_table_constructors[bit_size]()
        tr_table_guard = bindings.TranspositionTableGuard()
        tr_table_pruner = transposition_table_pruner_constructors[bit_size](
            tr_table=tr_table, tr_table_guard=tr_table_guard
        )

        result[bit_size] = coordinator_constructor(
            zobrist_component=zobrist_component,
            tr_table=tr_table,
            tr_table_guard=tr_table_guard,
            tr_table_pruner=tr_table_pruner,
        )
    return result


@pytest.fixture
def minimax_move_evaluator_constructors() -> dict[
    int,
    Callable[
        ...,
        bindings.MinimaxMoveEvaluator32New
        | bindings.MinimaxMoveEvaluator64New
        | bindings.MinimaxMoveEvaluator128New,
    ],
]:
    return {
        32: bindings.MinimaxMoveEvaluator32New,
        64: bindings.MinimaxMoveEvaluator64New,
        128: bindings.MinimaxMoveEvaluator128New,
    }


@pytest.fixture
def minimax_move_evaluators(
    minimax_move_evaluator_constructors,
    game_board,
    piece_position_points,
    zobrist_coordinators,
    pre_search_move_sorter,
):
    result = {}
    for (
        bit_size,
        evaluator_constructor,
    ) in minimax_move_evaluator_constructors.items():
        result[bit_size] = evaluator_constructor(
            evaluating_player=bindings.PieceColor.kRed,
            search_depth=4,
            game_board=game_board,
            game_position_points=piece_position_points,
            hash_calculator=zobrist_coordinators[bit_size],
            move_sorter=pre_search_move_sorter,
        )
    return result


class TestCoreConstructors:

    def test_zobrist_calculator_constructors(
        self, zobrist_calculator_constructors
    ):
        results = {
            bit_size: constructor()
            for bit_size, constructor in zobrist_calculator_constructors.items()
        }

        seeded_results = {
            bit_size: constructor(1234)
            for bit_size, constructor in zobrist_calculator_constructors.items()
        }

    def test_game_board(self, game_board):
        result = bindings.GameBoard()

    def test_piece_position_points(self, piece_position_points):
        result = bindings.PiecePositionPoints()

    def test_zobrist_components(self, zobrist_components):
        result = zobrist_components

    def test_transposition_table(self, transposition_table_constructors):
        result = {
            bit_size: tr_table_constructor()
            for bit_size, tr_table_constructor in transposition_table_constructors.items()
        }

    def test_transposition_table_guard(self):
        tr_table_guard = bindings.TranspositionTableGuard()

    def test_transposition_table_pruner(
        self,
        transposition_table_pruners,
    ):
        result = transposition_table_pruners

    def test_pre_search_move_sorter(self, pre_search_move_sorter):
        result = pre_search_move_sorter

    def test_zobrist_coordinator_new(self, zobrist_coordinators):
        result = zobrist_coordinators

    def test_minimax_move_evaluators_new(self, minimax_move_evaluators):
        result = minimax_move_evaluators

