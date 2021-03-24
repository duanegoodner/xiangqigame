# Full code for an abstract game of Xiangqi.
# See README for details

class Gamepiece:
    """
    Represents a Gamepiece.
    """

    def __init__(self, label, color, piece_type, position):
        """
        Initializes a Gamepiece. Piece needs to be initialized with its
        ._position [row, col] values  matching the indices of the element
        the Gamepiece occupies in a Gameboard object's ._map data member. Any
        code that changes a Gampiece object's location in the ._map list must
        be accompanied by code that changes that Gampepiece's ._position data
        member. Decoupling these parameters was helpful for code development,
        but in future versions, it will be useful to simplify to by making
        ._position a function of the ._map indices (or vice-versa).
        :param label: 4-character string in format cXYn. c = 'b' for black,
        'r' for red. XY = 1st two letters in name of piece type. n = integer >=
         0 to uniquely identify specific piece of color c and type XY. Used
        for identifying pieces when printing board. In future versions, may be
        useful as library keys.
        :param color: string identifying player that piece belongs to ('red' or
         'black')
        :param position: list of integers the form [row-index, col-index] that
        represents position of Gamepiece on a Gameboard map.
        """
        self._label = label
        self._color = color
        self._piece_type = piece_type
        self._position = position

    def __repr__(self):
        """
        Overrides default __repr__ so that print(Gamepiece) returns ._label
        data member instead of memory address.
        :return: ._label data member of Gamepiece object.
        """
        return repr(self._label)

    def get_position(self):
        """
        Returns position of Gamepiece object.
        :return: list of the form [row-index, col-index] that represents
        position of Gamepiece.
        """
        return self._position

    def get_forward_direction(self):
        """
        Gets a piece's forward direction.
        :return: Two-integer list describing a piece's forward movement vector.
        """
        if self._color == 'red':
            return [1, 0]
        if self._color == 'black':
            return [-1, 0]

    def get_backward_direction(self):
        """
        Gets a piece's backward direction.
        :return: Two-integer list describing a piece's backward movement vector.
        """
        if self._color == 'red':
            return [-1, 0]
        if self._color == 'black':
            return [1, 0]

    def set_position(self, new_position):
        """
        Changes the ._position data member of Gamepiece object. This method
        does NOT change Gamepiece object's position in a Gameboard's ._map
        list.
        :param new_position: list of the form [row-index, col-index] that
        represents new position of Gamepiece.
        """
        self._position = new_position

    def get_label(self):
        """
        Gets label of a Gamepiece object.
        :return: label: 4-character string in format cXYn. c = 'b' for black,
        'r' for red. XY = 1st two letters in
        name of piece type. n = integer >= 0 to uniquely identify specific
        piece of color c and type XY.
        """
        return self._label

    def get_color(self):
        """
        Gets the color of a Gamepiece object.
        :return: self._color, a string = 'red' or 'black'
        """
        return self._color

    def get_type(self):
        """
        Gets the type of a Gamepiece object.
        :return: self._piece_type, a string indicating the a Gamepiece object's
        type (General, Advisor, Elephant,
        Horse, ... etc.)
        """
        return self._piece_type

    def get_opp_color(self):
        """
        Gets the color of the opponent of a Gamepiece object.
        :return: 'black' if self._color is 'red'; 'red' if self._color is
        'black'
        """

        if self._color == 'black':
            return 'red'
        if self._color == 'red':
            return 'black'

    def get_move_delta(self, destination):
        """
        Gets the the vector for moving a piece from current location to new
        location.
        :param destination: Two-element list of integers of the form [row, col]
        that represent Gamepiece's position.
        :return: Two-element list of integers representing [# of rows, # of cols]
        to move Gamepiece to a new location.
        """
        return [destination[0] - self._position[0],
                destination[1] - self._position[1]]

    def get_orthogonal_path(self, destination, board):
        """
        Gets list of spaces between two spaces that are in either same row or
        same column.
        Only call after already confirming that have one (and only 1) 0 in
        move_delta
        :param destination: two-integer list representing [row, col] of
        :param board: Gameboard object. self must be an element of Gameboard's
        ._map list.
        :return: path = list of two-element lists that represent board spaces
        between Gamepiece object's current location and a proposed destination.
        """

        # Get move "vector"
        move_delta = self.get_move_delta(destination)

        # Determine which index of orthogonal move vector is non-zero
        index_nz = [index for index in range(2) if move_delta[index] != 0][0]

        # Get list of
        if index_nz == 0:
            path = board.get_spaces_betw_col(self._position, destination)
        if index_nz == 1:
            path = board.get_spaces_betw_row(self._position, destination)

        return path


class General(Gamepiece):
    """
    Represents a General object. Inherits from Gamepiece.
    """

    def __init__(self, color, index, position):
        """
        Initializes a General object with all of the same data members as a
        generic Gampepiece object.
        :param color: a string = 'red' or 'black' indicating which player
        General belongs to
        :param index: an integer used as part of Gamepiece label. For General,
        index will always be 0
        since there is only one General of each color.
        :param position: list of integers the form [row-index, col-index] that
        represents position of General object on a Gameboard map.
      """
        super().__init__(color[0] + 'GE' + str(index), color, 'general',
                         position)

    def is_legal_move(self, destination, board):
        """
        Indicates whether or not moving General object to particular location
        on Gameboard is legal.
        :param destination: 2-element list of integers representing [row-index,
        col-index] of location
        in Gameboard map that is being checked for a legal move.
        :param board: a Gameboard object
        :return: Boolean True if moving General to destination is legal. False
        if not legal.
        """

        # Check for flying general move first, since it is the only move that
        # would allow general to leave castle
        other_gen_pos = board.get_general(self.get_opp_color()).get_position()

        # If in same column as other team's general and destinatin is other
        # general's position
        if self._position[1] == other_gen_pos[
            1] and destination == other_gen_pos:

            # Find out if all spaces between the two generals are empty
            if set(board.get_pieces_between_col(
                    self.get_position(), destination)) \
                    == {None}:
                return True

        # Check if destination is in castle
        if destination not in board.get_castle(self._color):
            return False

        # Check if destination is occupied by piece of same color.
        # Could factor this out?
        if board.is_occ_byteam(destination, self._color):
            return False

        # Check if proposed move meets "1 orthogonal space" requirement
        move_delta = self.get_move_delta(destination)
        if 0 not in move_delta or (
                1 not in move_delta and -1 not in move_delta):
            return False

        return True


class Advisor(Gamepiece):
    """
    Represents an Advisor Gamepiece object.
    """

    def __init__(self, color, index, position):
        """
        Initializes an Advisor object with all of the same data members as a
        generic Gampepiece object..
        :param color: a string = 'red' or 'black' indicating which player
        Advisor belongs to
        :param index: integer used as part of Gamepiece label. For Advisor,
        index = 0 or 1 since each player starts with 2 Advisors.
        :param position: list of integers the form [row-index, col-index] that
        represents position of General object on a Gameboard map.
        """
        super().__init__(color[0] + 'AD' + str(index), color, 'advisor',
                         position)

    def is_legal_move(self, destination, board):
        """
        Indicates whether or not moving Advisor object to particular location
        on Gameboard is legal.
        :param destination: 2-element list of integers representing [row-index,
        col-index] of location in Gameboard
        map that is being checked for a legal move.
        :return: Boolean True if moving Advisor to destination is legal. False
        if not legal.
        """

        # Check if destination is occupied by piece of same color.
        # Could factor this out?
        if board.is_occ_byteam(destination, self._color):
            return False

        # Check if destination is in castle.
        if destination not in board.get_castle(self._color):
            return False

        # Check if proposed move meets "single diagaonal point" requirement
        move_delta = self.get_move_delta(destination)
        if abs(move_delta[0]) != 1 or abs(move_delta[1]) != 1:
            return False

        return True


class Elephant(Gamepiece):
    """
    Represents and Elephant Gamepiece object.
    """

    def __init__(self, color, index, position):
        """
        Initializes an Elephant object with all of the same data members as a
        generic Gampepiece object..
        :param color: a string = 'red' or 'black' indicating which player
        Elephant belongs to
        :param index: integer used as part of Gamepiece label. For Advisor,
        index = 0 or 1 since each player starts
        with 2 Elephants.
        :param position: list of integers the form [row-index, col-index] that
        represents position of Elephant object on
        a Gameboard map.
        """
        super().__init__(color[0] + 'EL' + str(index), color, 'elephant',
                         position)

    def is_legal_move(self, destination, board):
        """
        Indicates whether or not moving Elephant object to particular location
        on Gameboard is legal.
        :param destination: 2-element list of integers representing [row-index,
        col-index] of location in Gameboard
        map that is being checked for a legal move.
        :return: Boolean True if moving Elephant to destination is legal. False
        if not legal.
        """

        # Check if destination is occupied by piece of same color.
        # Could factor this out?
        if board.is_occ_byteam(destination, self._color):
            return False

        # check if proposed destination is in elephant's homeland
        # (correct side of river)
        if destination not in board.get_homeland(self._color):
            return False

        # check if proposed destination meets two diagonal requirement
        move_delta = self.get_move_delta(destination)
        if abs(move_delta[0]) != 2 or abs(move_delta[1]) != 2:
            return False

        # check if move to proposed destination is blocked
        block_pos = [self._position[0] + int(move_delta[0] / 2),
                     self._position[1] + int(move_delta[1] / 2)]
        if board.is_occupied(block_pos):
            return False

        return True


class Horse(Gamepiece):
    """
    Represents a Horse object.
    """

    def __init__(self, color, index, position):
        """
        Initializes a Horse object with all of the same data members as a
        generic Gampepiece object..
        :param color: a string = 'red' or 'black' indicating which player Horse
        belongs to
        :param index: integer used as part of Gamepiece label. For Horse,
        index = 0 or 1 since each player starts with 2 Horses.
        :param position: list of integers the form [row-index, col-index] that
        represents position of Horse object on a Gameboard map.
        """
        super().__init__(color[0] + 'HO' + str(index), color, 'horse',
                         position)

    def is_legal_move(self, destination, board):
        """
        Indicates whether or not moving Horse object to particular location on
        Gameboard is legal.
        :param destination: 2-element list of integers representing [row-index,
        col-index] of location in Gameboard
        map that is being checked for a legal move.
        :return: Boolean True if moving Horse to destination is legal. False if
        not legal.
        """

        # Check if destination is occupied by piece of same color.
        # Could factor this out?
        if board.is_occ_byteam(destination, self._color):
            return False

        # Check if move meets 1:2 (or 2:1) horizontal:vertical requirement
        move_delta = self.get_move_delta(destination)
        hmoves = [[row, col] for row in range(-2, 3) for col in range(-2, 3) if
                  abs(col) != abs(row) and 0 not in [row, col]]

        if move_delta not in hmoves:
            return False

        # Check if a blocking position is occupied
        if abs(move_delta[0]) > abs(move_delta[1]):
            block_pos = [self._position[0] + int(move_delta[0] / 2),
                         self._position[1]]
        elif abs(move_delta[0]) < abs(move_delta[1]):
            block_pos = [self._position[0],
                         self._position[1] + int(move_delta[1] / 2)]

        if board.is_occupied(block_pos):
            return False

        return True


class Chariot(Gamepiece):
    """
    Represents a Chariot object.
    """

    def __init__(self, color, index, position=None):
        """
        Initializes a Chariot object with all of the same data members as a
        generic Gampepiece object..
        :param color: a string = 'red' or 'black' indicating which player
        Chariot belongs to
        :param index: integer used as part of Gamepiece label. For Chariot,
        index = 0 or 1 since each player starts
        with 2 Chariots.
        :param position: list of integers the form [row-index, col-index] that
        represents position of Chariot object on
        a Gameboard map.
        """
        super().__init__(color[0] + 'CH' + str(index),
                         color, 'chariot', position)

    def is_legal_move(self, destination, board):
        """
        Indicates whether or not moving Chariot object to particular location
        on Gameboard is legal.
        :param destination: 2-element list of integers representing [row-index,
        col-index] of location in Gameboard
        map that is being checked for a legal move.
        :param board: Gameboard object used in current game
        :return: Boolean True if moving Chariot to destination is legal. False
        if not legal.
        """

        # Using separate method for chariot since can be used for part of
        # cannon check
        return board.chariot_check(self, destination)


class Cannon(Gamepiece):
    """
    Represents a Cannon object.
    """

    def __init__(self, color, index, position=None):
        """
        Initializes a Cannon object with all of the same data members as a
        generic Gampepiece object..
        Could improve by doing orthogonal move check first.
        :param color: a string = 'red' or 'black' indicating which player
        Cannon belongs to
        :param index: integer used as part of Gamepiece label. For Cannon,
        index = 0 or 1 since each player starts with 2 Cannons.
        :param position: list of integers the form [row-index, col-index] that
        represents position of Cannon object on a Gameboard map.
        """
        super().__init__(color[0] + 'CA' + str(index), color, 'cannon',
                         position)

    def is_legal_move(self, destination, board):
        """
        Indicates whether or not moving Chariot object to particular location
        on Gameboard is legal.
        :param destination: 2-element list of integers representing [row-index,
        col-index] of location in Gameboard
        map that is being checked for a legal move.
        :return: Boolean True if moving Chariot to destination is legal. False
        if not legal.
        """

        # if moving to unoccupied space,same rules as chariot
        if not board.is_occupied(destination):
            return board.chariot_check(self, destination)

        # Check if proposed move is orthogonal
        move_delta = self.get_move_delta(destination)
        if 0 not in move_delta:
            return False

        # Check if destination is occupied by piece of same color.
        # Could factor this out?
        if board.is_occ_byteam(destination, self._color):
            return False

        # if attempting to move to space occupied by opponent,
        # check if meets jump criteria
        if board.is_occ_byteam(destination, self.get_opp_color()):

            path = self.get_orthogonal_path(destination, board)

            if len(set([board.get_piece(square) for square in path])
                   - {None}) != 1:
                return False

        return True


class Soldier(Gamepiece):
    """
    Represents a Soldier object.
    """

    def __init__(self, color, index, position):
        """
        Initializes a Soldier object with all of the same data members as a
        generic Gampepiece object.
        :param color: a string = 'red' or 'black' indicating which player
        Soldier belongs to
        :param index: integer used as part of Gamepiece label. For Cannon,
        index = 0, 1, 2, or 3 since each player
        starts with 4 Soldiers.
        :param position: list of integers the form [row-index, col-index] that
        represents position of Cannon object on a Gameboard map.
        """
        super().__init__(color[0] + 'SO' + str(index), color, 'soldier',
                         position)

    def is_legal_move(self, destination, board):
        """
        Indicates whether or not moving Chariot object to particular location
        on Gameboard is legal.
        :param destination: 2-element list of integers representing [row-index,
        col-index] of location in Gameboard
        map that is being checked for a legal move.
        :return: Boolean True if moving Chariot to destination is legal. False
        if not legal.
        """

        # Check that move is just one space horizontal or vertical
        move_delta = self.get_move_delta(destination)

        if 0 not in move_delta or (
                1 not in move_delta and -1 not in move_delta):
            return False

        # Check if occupied by teammate. Factor this out
        if board.is_occ_byteam(destination, self._color):
            return False

        # Move not allowed if it is in backward direction
        if self.get_move_delta(destination) == self.get_backward_direction():
            return False

        # If soldier on its own side of river, move not allowed if it is not
        # forward
        if self.get_position() in board.get_homeland(self._color):
            if self.get_move_delta(destination) != \
                    self.get_forward_direction():
                return False

        return True


class Gameboard:
    """
    Represents a Gameboard object used by a XiangqiGame object.
    """

    def __init__(self):
        """
        Initializes Gameboard object with  ._map, ._piece_list, and (in future
        rev) ati_library data members. ._map is a 9 element list of 10 element
        lists to represent 10 row, 9 col board. ._castles are lists of spaces
        in the castles. ._re_red and ._re_black are river edges.
        """
        # Start with empty ._map
        self._map = [[None] * 9 for row in range(10)]
        self._castle_red = [[row, col + 3] for row in range(3) for col in
                            range(3)]
        self._castle_black = [[row + 7, col + 3] for row in range(3) for col in
                              range(3)]
        self._re_red = 4
        self._re_black = 5

        # Populate the ._map list with Gamepieces
        self._map[0][4] = General('red', 0, [0, 4])
        self._map[9][4] = General('black', 0, [9, 4])
        for piece in range(5):
            self._map[3][2 * piece] = Soldier('red', piece, [3, 2 * piece])
        for piece in range(5):
            self._map[6][2 * piece] = Soldier('black', piece, [6, 2 * piece])
        for piece in range(2):
            self._map[0][2 * piece + 3] = Advisor('red', piece,
                                                  [0, 2 * piece + 3])
        for piece in range(2):
            self._map[9][2 * piece + 3] = Advisor('black', piece,
                                                  [9, 2 * piece + 3])
        for piece in range(2):
            self._map[0][4 * piece + 2] = Elephant('red', piece,
                                                   [0, 4 * piece + 2])
        for piece in range(2):
            self._map[9][4 * piece + 2] = Elephant('black', piece,
                                                   [9, 4 * piece + 2])
        for piece in range(2):
            self._map[0][6 * piece + 1] = Horse('red', piece,
                                                [0, 6 * piece + 1])
        for piece in range(2):
            self._map[9][6 * piece + 1] = Horse('black', piece,
                                                [9, 6 * piece + 1])
        for piece in range(2):
            self._map[0][8 * piece] = Chariot('red', piece, [0, 8 * piece])
        for piece in range(2):
            self._map[9][8 * piece] = Chariot('black', piece, [9, 8 * piece])
        for piece in range(2):
            self._map[2][6 * piece + 1] = Cannon('red', piece,
                                                 [2, 6 * piece + 1])
        for piece in range(2):
            self._map[7][6 * piece + 1] = Cannon('black', piece,
                                                 [7, 6 * piece + 1])

    def get_map(self):
        """
        :return: self._map, a nine element list of 10 elements in [row][col] form.
        """
        return self._map

    def get_all_spaces(self):
        """
        :return: a list of the [row, col] indices of all elements in a
        Gameboard object's ._map list.
        """
        return [[row, col] for row in range(len(self._map)) for col in
                range(len(self._map[0]))]

    def get_castle(self, color):
        """
        Gets a list of board locations that are part of either the red or black
        castle.
        :param color: a string = 'red' or 'black'
        :return: A list of two-integer lists in the form [row, col]
        corresponding to board spaces that are in
        the color team's castle.
        """
        if color == 'red':
            return self._castle_red
        if color == 'black':
            return self._castle_black

    def get_river_edge(self, color):
        """
        Gets the river edge of either red or black team.
        :param color: a string = 'red' or 'black'
        :return: an integer corresponding to the row index that represents the river edge on color team's
        side of the board.
        """
        if color == 'red':
            return self._re_red
        if color == 'black':
            return self._re_black

    def get_homeland(self, color):
        """
        Summary: Gets list of board spaces that make up the "home territory" of
        a particular
        piece / team color.
        :param color: a string = 'red' or 'black'
        :return: a list of [row, col] indices of  elements in ._board that make
        up the home
        territory of team specified by color.
        """
        if color == 'red':
            return [[row, col] for row in range(self._re_red + 1) for col in
                    range(9)]
        if color == 'black':
            return [[row, col] for row in range(self._re_black, 10) for col in
                    range(9)]

    def is_occupied(self, position):
        """
        Determines if a particular element of ._map is occupied by a Gamepiece.
        :param position: Two integer list corresponding to [row, col] of an
        element in ._map.
        :return: Boolean True if a Gamepiece occupies position. False if no
        Gamepiece at position.
        """
        if self._map[position[0]][position[1]] is not None:
            return True
        if self._map[position[0]][position[1]] is None:
            return False

    def is_occ_byteam(self, position, color):
        """
        Determines if particular position on Gameboard is occupied by
        particular team.
        :param position: Two integer list representing [row, col]  of board
        location.
        :param color: string = 'black' or 'red'
        :return: Boolean True or False.
        """
        if self._map[position[0]][position[1]] is not None:
            if self._map[position[0]][position[1]].get_color() == color:
                return True

        return False

    def get_spaces_betw_col(self, pos_a, pos_b):
        """
        Gets list of spaces between two board spaces located in same column.
        :param pos_a: [row, col] of position A
        :param pos_b: [row, col] of position B
        :return: List of [row, col] indices of board spaces between (but not
        including) positions A and B.
        """
        if pos_a[1] == pos_b[1]:
            col = pos_a[1]
            path_start = min(pos_a[0], pos_b[0]) + 1
            path_end = max(pos_a[0], pos_b[0])
            return [[row, col] for row in range(path_start, path_end)]

    def get_pieces_between_col(self, pos_a, pos_b):
        """
         Gets list of pieces between two board spaces located in same column.
        :param pos_a: [row, col] of position A
        :param pos_b: [row, col] of position B
        :return: List of Gamepieces in spaces between (but not including)
        positions A and B.
        """

        # Should only be called if positions are in same col, but double-check:
        if pos_a[1] == pos_b[1]:
            col = pos_a[1]
            path_start = min(pos_a[0], pos_b[0]) + 1
            path_end = max(pos_a[0], pos_b[0])
            return [self.get_map()[row][col]
                    for row in range(path_start, path_end)]

    def get_spaces_betw_row(self, pos_a, pos_b):
        """
        Gets list of spaces between two board spaces located in same row.
        :param pos_a: [row, col] of position A
        :param pos_b: [row, col] of position B
        :return: List of [row, col] indices of baard spaces between (but not
        including) positions A and B.
        """
        if pos_a[0] == pos_b[0]:
            row = pos_a[0]
            path_start = min(pos_a[1], pos_b[1]) + 1
            path_end = max(pos_a[1], pos_b[1])
            return [[row, col] for col in range(path_start, path_end)]

    def chariot_check(self, piece, destination):
        """
        Determines if move a Chariot Gamepiece to specific destination is legal
        :param piece: a Chariot object.
        :param destination: [row, col] indices of board location.
        :return: Boolean True or False
        """

        # Get move vector from current location to destination
        move_delta = piece.get_move_delta(destination)

        # Orthogonal move not allowed for Chariot.
        if 0 not in move_delta:
            return False

        # Can't move to location if occupied by piece of same color
        if self.is_occ_byteam(destination, piece._color):
            return False

        # Get list of spaces in path to destination
        path = piece.get_orthogonal_path(destination, self)

        # If any Gamepiece in path, can't move to destination
        # (destination is not part of path)
        if set([self.get_piece(square) for square in path]) - {None} != set():
            return False

        return True

    def get_general(self, color):
        """
        Gets Gampepiece object corresponding to General of particular team.
        :param color: string = 'red' or 'black'
        :return: General gamepiece object.
        """
        for piece in self.get_team_pieces(color):
            if piece.get_type() == 'general':
                return piece

    def get_all_pieces(self):
        """
        Gets all pieces on a Gamebord.
        :return: list of all pieces present in a Gameboard object's ._map data
        member.
        """
        all_piece_list = []
        for row in range(len(self._map)):
            row_list = [item for item in self._map[row] if item != None]
            all_piece_list += row_list

        return all_piece_list

    def get_team_pieces(self, color):
        """
        Gets list of pieces present on ._map data member of Gameboard object.
        :return: a list of oll Gamepiece objects present in the ._map data
        member of ._board.
        """
        all_piece_list = self.get_all_pieces()
        team_piece_list = [piece for piece in all_piece_list if
                           piece.get_color() == color]

        return team_piece_list

    def move_piece(self, from_indices, to_indices):
        """
        Moves a Gamepiece object from one locatoin to another.Calling method
        needs to ensure that element
        from_indices in ._map is not None.
        :param from_indices: list representing [row, col] of location piece is
        being moved from
        :param to_indices: list representing [row, col] of location piece is
        being moved to
        """

        # Get / save Gamepiece object being moved
        from_piece = self.get_piece(from_indices)

        # Change ._map element being moved away from to None
        self._map[from_indices[0]][from_indices[1]] = None

        # Change destination ._map element to the Gamepiece object that is
        # being moved
        self._map[to_indices[0]][to_indices[1]] = from_piece

        # Update ._position data member of piece being moved
        from_piece.set_position(to_indices)

    def print_board(self):
        """
        ._map data member of Gameboard object .
        """

        ita_lib = {
            0: 'a',
            1: 'b',
            2: 'c',
            3: 'd',
            4: 'e',
            5: 'f',
            6: 'g',
            7: 'h',
            8: 'i',
        }

        # Need copy module for deep copy
        import copy

        # Create disp_map, a deep copy of self._map
        # (use deep copy to ensure don't mutate self._map)
        disp_map = copy.deepcopy(self._map)

        header = [(ita_lib[entry] * 4).upper() for entry in range(9)]
        print(header)

        for row in range(len(self._map)):
            print(self._map[9 - row])

    def get_piece(self, board_space):
        """
        Gets the Gamepiece object that occupies element board_space in ._map
        :param board_space: List of 2 integers representing the [row, column]
        of particular element of ._map.
        :return: Gampepiece object if board_space is occupied; returns None if
        not occupied.
        """
        return self._map[board_space[0]][board_space[1]]

    def remove_piece(self, position):
        """
        Removes Gamepiece from ._map by setting element = None.
        :param position: list of two integers that specify [row, col] in ._map
        to be set to None
         """
        self._map[position[0]][position[1]] = None

    def test_move(self, from_indices, to_indices, game):
        """
        Tests if move is legal by checking Gamepiece ._is_legal. If it is,
        executes move, tests if move causes
        moving team to be in check, and then un-does the move. Params include a
        XiangqiGame object because
        the Game objects is_in_check method is used.
        :param from_indices: ._map indices of location of piece involved in the
        test move.
        :param to_indices: ._map indices of location of test move destination.
        :param game: a XiangiGame object.
        :return: Boolean True of False
        """

        # Test if move is legal based on everything except in-check
        if not self.get_piece(from_indices).is_legal_move(to_indices, self):
            return False

        # Get / save object currently at space being moved to.
        # Could be a Gamepiece or None.
        to_piece = self.get_piece(to_indices)
        # print('to_piece', to_piece)
        # print('from_piece', self.get_piece(from_indices))

        # If to_piece is a Gamepiece, set the ._position data member of that
        # Gamepiece to None
        if to_piece is not None:
            to_piece.set_position(None)

        # Move piece at from_indices to to_indices
        self.move_piece(from_indices, to_indices)

        # Test if team making test move in is check with new board
        # configuration
        cause_check = game.is_in_check(self.get_piece(to_indices).get_color())

        # Undo-move
        self.move_piece(to_indices, from_indices)

        # Return element to_indices of ._map to original value
        # (either a particular Gamepiece or None)
        self._map[to_indices[0]][to_indices[1]] = to_piece
        if to_piece is not None:
            to_piece.set_position(to_indices)

        # If test move did not cause moving piece's team to be in check,
        # then tested move is legal.
        return not cause_check

    def allowed_move_list(self, color, game):
        """
        Generates a list of current allowed moves for a particular team in a
        XiangqiGame object. This list depends on the ._map data member of the
        XiangqiGame object's ._board data member (a Gameboard object).
        :param color: string = 'red' or 'black' (color of the team whose
        allowed move list is being generated)
        :param game: a XiangqiGame object
        :return: A list of allowed moves. Each element of the list is of the
        form: [from_indices, to_indices] where from_indices and to_indices are
        each 2-integer lists and correspond to the indices of elements in a
        Gameboard object's ._map data member.
        """

        move_list = []

        # For each piece in ._map with its ._color data member == color
        for piece in self.get_team_pieces(color):

            # Check every space in board
            # (could speed up by only checking spaces not occuped by color)
            for space in self.get_all_spaces():

                # If moving piece to particular board space is a legal move
                if self.test_move(piece.get_position(), space, game):
                    # Add [piece's location, particular board space's location]
                    # to move_list
                    move_list.append([piece.get_position(), space])

        return move_list


class XiangqiGame:
    """
    Represents a XiangqiGame object with _game_state, _board, and _whose_turn
    data members.
    """

    def __init__(self):
        """
        Initializes XiangqiGame object.
        """
        self._game_state = 'UNFINISHED'
        self._board = Gameboard()
        self._whose_turn = 'red'
        self._legal_moves = self._board.allowed_move_list(self._whose_turn,
                                                          self)

    def get_game_state(self):
        """
        Returns ._gamestate
        :return: ._game_state, a string = 'UNFINISHED', 'BLACK_WON', or 'RED_WON'
        """
        return self._game_state

    def set_legal_moves(self):
        """
        Calculates list of legal moves in Game based on status of ._board data
        member and self._whose_turn.
        :return: List of legal moves. Each element in list is of the form
        [from_row, from_col], [to_row, to_col]
        """
        self._legal_moves = self._board.allowed_move_list(self._whose_turn,
                                                          self)

    def set_loser(self, color):
        """
        Sets ._games_tate to opposite team of ._whose_turn.
        :param color: string = 'red' or 'black'
        """
        if color == 'red':
            self._game_state = 'BLACK_WON'
        if color == 'black':
            self._game_state = 'RED_WON'

    def get_whose_turn(self):
        """
        Returns color of player whose turn it is.
        :return: _whose_turn, a string = 'black' or 'red'
        """
        return self._whose_turn

    def is_in_check(self, player):
        """
        Checks if player is in check.
        :param player: a string = 'red' or 'black'
        :return: Boolean true if player is in check.
        """

        gen_pos = self._board.get_general(player).get_position()

        opp_pieces = [piece for piece in self._board.get_all_pieces() if
                      piece.get_color() != player]

        for piece in range(len(opp_pieces)):
            if opp_pieces[piece].is_legal_move(gen_pos, self._board):
                return True

        return False

    def update_turn(self):
        """
        Changes value of _whose_turn to opposing player.
        """

        if self._whose_turn == 'red':
            self._whose_turn = 'black'
            return
        if self._whose_turn == 'black':
            self._whose_turn = 'red'
            return

    def alg_to_ind(self, alg_pos):
        """
        Summary:
        :param alg_pos: String containing one letter (a - i) followed by a
        1-digit or 2-digit number
        (1 - 10). The letter corresponds to a column and number corresponds to
        a row.
        :return: List containing two integers. Represents same position as
        alg_position, but using index
        notation to reference element in self._map. 1st integer = row index.
        2nd integer = column index.
        """
        ati_lib = {
            'a': 0,
            'b': 1,
            'c': 2,
            'd': 3,
            'e': 4,
            'f': 5,
            'g': 6,
            'h': 7,
            'i': 8
        }  # Need to change this library to a data member.
        row_ind = int(alg_pos[1:]) - 1
        col_ind = ati_lib[alg_pos[0]]

        return [row_ind, col_ind]

    def make_move(self, from_sq, to_sq):
        """
        Checks is move from one location to another is legal, and executes move
        if legal.
        :param from_sq: string = algebraic notation of location on ._board's
        ._map data member that user
        is attempting to move piece from.
        :param to_sq: string = algebraic notation of location on ._board's
        ._map data member that user
        is attempting to move piece to.
        :return: Boolean True if move is executed. False if move is not legal
        (and not executed).
        """

        # Convert algebraic board notation to [row][col] indices of ._board's
        # ._map list
        from_indices = self.alg_to_ind(from_sq)
        to_indices = self.alg_to_ind(to_sq)

        # Get object that occupies the from_indices element of ._board's
        # ._map list
        from_piece = self._board.get_piece(from_indices)

        # Test 1: If object at from_indices element element of ._map list is
        # None, can't move
        if from_piece is None:
            return False

        # Test 2: If object at from_indices element of ._map list is a piece
        # belonging to other team, can't move it
        if from_piece.get_color() != self.get_whose_turn():
            return False

        # Test 3: If move is not in list of legal moves, can't execute it
        if [from_indices, to_indices] not in self._legal_moves:
            return False

        # Test 4: If game has already been won, can't make a move
        if self.get_game_state() != 'UNFINISHED':
            return False

        # Tests 1 - 4 above passed, execute the move
        self._board.move_piece(from_indices, to_indices)

        # Change XiangqiGame object's ._whose_turn data member to the other
        # team
        self.update_turn()

        # Determine list of legal moves now that it's a new player's turn
        self.set_legal_moves()

        # Test if the updated ._whose_turn team has any legal moves
        # legal_moves =
        # self._board.allowed_move_list(self.get_whose_turn(), self)
        if len(self._legal_moves) == 0:
            self.set_loser(self.get_whose_turn())

        return True

    def show_game_status(self):
        """
        Shows game status (._map, ._whose_turn, and which if any player is in check)
        """

        self._board.print_board()
        print('Game State =  ', self.get_game_state(), '  Whose turn:     ',
              self.get_whose_turn())
        print('red in check: ', self.is_in_check('red'),
              '       black in check: ', self.is_in_check('black'))
        print()

    def move_and_report(self, from_space, to_space):
        """
        Executes a move and reports state of game.
        :param from_space: Board space in algebraic notation.
        :param to_space:  Board space in algebraic notation
        """

        result = self.make_move(from_space, to_space)
        if result:
            move_ok = 'Move executed'
        if not result:
            move_ok = 'Move NOT executed'
        print('Attempted move from: ', from_space, ' to: ', to_space)
        print(move_ok)
        self.show_game_status()
