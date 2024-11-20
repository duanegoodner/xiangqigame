import xiangqi_bindings as bindings
# from xiangqipy.command_input import PlayerInputNew, XiangqiGameCommandNew
from typing import Any


def collect_items_if_key_startswith(
    key_prefix: str, run_kwargs: dict[str, Any]
) -> dict[str, Any]:
    return {
        key: value
        for key, value in run_kwargs.items()
        if key.startswith(key_prefix)
    }


def build_xiangqi_command_new(
    run_kwargs: dict[str, Any]
) -> XiangqiGameCommandNew:
    red_kwargs = collect_items_if_key_startswith(
        key_prefix="red", run_kwargs=run_kwargs
    )
    black_kwargs = collect_items_if_key_startswith(
        key_prefix="black", run_kwargs=run_kwargs
    )
    other_kwargs = {
        key: value
        for key, value in run_kwargs.items()
        if not (key.startswith("red") or key.startswith("black"))
    }

    return XiangqiGameCommandNew(
        red_input=PlayerInputNew(**red_kwargs),
        black_input=PlayerInputNew(**black_kwargs),
        **other_kwargs,
    )
