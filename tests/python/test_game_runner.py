import pytest
import xiangqi_bindings as bindings


@pytest.fixture
def custom_red_player_spec():
    return bindings.PlayerSpec(
        color=bindings.PieceColor.kRed,
        evaluator_type=bindings.EvaluatorType.kMinimax,
        zobrist_key_size_bits=128,
        zobrist_calculator_count=1,
        minimax_search_depth=3,
    )


@pytest.fixture
def custom_black_player_spec():
    return bindings.PlayerSpec(
        color=bindings.PieceColor.kBlk,
        evaluator_type=bindings.EvaluatorType.kMinimax,
        zobrist_key_size_bits=32,
        zobrist_calculator_count=2,
        minimax_search_depth=3,
    )


def test_default_player_spec():
    red_player_spec = bindings.PlayerSpec(color=bindings.PieceColor.kRed)
    assert red_player_spec.color == bindings.PieceColor.kRed
    assert red_player_spec.evaluator_type == bindings.EvaluatorType.kMinimax
    assert red_player_spec.zobrist_key_size_bits == 64
    assert red_player_spec.zobrist_calculator_count == 2
    assert red_player_spec.minimax_search_depth == 4


def test_custom_player_specs(custom_red_player_spec, custom_black_player_spec):
    assert custom_red_player_spec.color == bindings.PieceColor.kRed
    assert (
        custom_red_player_spec.evaluator_type
        == bindings.EvaluatorType.kMinimax
    )
    assert custom_red_player_spec.zobrist_key_size_bits == 128
    assert custom_red_player_spec.zobrist_calculator_count == 1
    assert custom_red_player_spec.minimax_search_depth == 5

    assert custom_black_player_spec.color == bindings.PieceColor.kBlk
    assert (
        custom_black_player_spec.evaluator_type
        == bindings.EvaluatorType.kRandom
    )
    assert custom_black_player_spec.zobrist_key_size_bits == 0
    assert custom_black_player_spec.zobrist_calculator_count == 0
    assert (
        custom_black_player_spec.minimax_search_depth == 0
    )

def test_init_game_runner(custom_red_player_spec, custom_black_player_spec):
    game_runner = bindings.GameRunner(
        red_player_spec=custom_red_player_spec,
        black_player_spec=custom_black_player_spec
    )

def test_game_runner_run_game(custom_red_player_spec, custom_black_player_spec):
    game_runner = bindings.GameRunner(
        red_player_spec=custom_red_player_spec,
        black_player_spec=custom_black_player_spec
    )
    game_runner.run_game()
