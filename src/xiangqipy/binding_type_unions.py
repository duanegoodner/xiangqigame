from collections import namedtuple
from typing import NamedTuple, Union, Literal
import xiangqi_bindings as bindings

ZobristCalculatorType = Union[
    bindings.ZobristCalculatorB032,
    bindings.ZobristCalculatorB064,
    bindings.ZobristCalculatorB128,
]

ZobristComponentType = Union[
    bindings.ZobristComponentNewB032C0,
    bindings.ZobristComponentNewB032C1,
    bindings.ZobristComponentNewB064C0,
    bindings.ZobristComponentNewB064C1,
    bindings.ZobristComponentNewB128C0,
    bindings.ZobristComponentNewB128C1,
]

TranspositionTableType = Union[
    bindings.TranspositionTableB032C0,
    bindings.TranspositionTableB032C1,
    bindings.TranspositionTableB064C0,
    bindings.TranspositionTableB064C1,
    bindings.TranspositionTableB128C0,
    bindings.TranspositionTableB128C1,
]

TranspositionTablePrunerType = Union[
    bindings.TranspositionTablePrunerB032C0,
    bindings.TranspositionTablePrunerB032C1,
    bindings.TranspositionTablePrunerB064C0,
    bindings.TranspositionTablePrunerB064C1,
    bindings.TranspositionTablePrunerB128C0,
    bindings.TranspositionTablePrunerB128C1,
]

ZobristCoordinatorType = Union[
    bindings.ZobristCoordinatorNewB032C0,
    bindings.ZobristCoordinatorNewB032C1,
    bindings.ZobristCoordinatorNewB064C0,
    bindings.ZobristCoordinatorNewB064C1,
    bindings.ZobristCoordinatorNewB128C0,
    bindings.ZobristCoordinatorNewB128C1,
]

MinimaxMoveEvaluatorType = Union[
    bindings.MinimaxMoveEvaluatorNewB032C0,
    bindings.MinimaxMoveEvaluatorNewB032C1,
    bindings.MinimaxMoveEvaluatorNewB064C0,
    bindings.MinimaxMoveEvaluatorNewB064C1,
    bindings.MinimaxMoveEvaluatorNewB128C0,
    bindings.MinimaxMoveEvaluatorNewB128C1,
]


class MinimaxEvaluatorSpecs(NamedTuple):
    zobrist_bit_size: Literal[32, 64, 128]
    num_confirmation_calculators: Literal[0, 1]


class MinimaxEvaluatorComponentTypes(NamedTuple):
    zobrist_calculator: ZobristCalculatorType
    zobrist_component: ZobristComponentType
    transposition_table: TranspositionTableType
    transposition_table_pruner: TranspositionTablePrunerType
    zobrist_coordinator: ZobristCoordinatorType
    minimax_move_evaluator: MinimaxMoveEvaluatorType


component_dispatch = {
    MinimaxEvaluatorSpecs(
        zobrist_bit_size=32, num_confirmation_calculators=0
    ): MinimaxEvaluatorComponentTypes(
        zobrist_calculator=bindings.ZobristCalculatorB032,
        zobrist_component=bindings.ZobristComponentNewB032C0,
        transposition_table=bindings.TranspositionTableB032C0,
        transposition_table_pruner=bindings.TranspositionTablePrunerB032C0,

    )
}
