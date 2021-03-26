

class BoardSpace:
    def __init__(self, coordinates, occupant):
        self._coordinates = coordinates
        self._occupant = occupant

    def get_occupant(self):
        return self._occupant

    def get_coordinates(self):
        return self._coordinates

    def is_occupied(self):
        return self._occupant.get_piece_type() != 'EMPTY'
