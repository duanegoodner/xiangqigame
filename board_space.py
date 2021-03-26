

class BoardSpace:
    def __init__(self, occupant):
        self._occupant = occupant

    def get_occupant(self):
        return self._occupant

    def is_occupied(self):
        return self._occupant.get_piece_type() != 'EMPTY'
