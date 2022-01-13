file_index_of = {
    'a': 0,
    'b': 1,
    'c': 2,
    'd': 3,
    'e': 4,
    'f': 5,
    'g': 6,
    'h': 7,
    'i': 8
}

rank_index_of = {str(num): num - 1 for num in range(1, 11)}


def alg_to_indices(alg_val):
    return rank_index_of[alg_val[1:]], file_index_of[alg_val[0]]





