import sys
from pathlib import Path
from unittest.mock import patch
import xiangqipy.app as app
import xiangqipy.game_summary_io as gio


def test_app_no_args():
    with patch.object(sys, 'argv', ['app.py']):
        app.run()

def test_app_save_summary():
    with patch.object(sys, 'argv', ['app.py']):
        game_summary = app.run(save_summary=True)
        export_path = gio.export_game_summary(game_summary=game_summary)
        assert export_path.exists()
        export_path.unlink()

def test_app_save_reimport_summary():
    with patch.object(sys, 'argv', ['app.py']):
        game_summary = app.run(save_summary=True)
        export_path = gio.export_game_summary(game_summary=game_summary)
        imported_summary = gio.import_game_summary(path=export_path)
        export_path.unlink()
    