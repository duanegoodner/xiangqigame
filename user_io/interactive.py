import user_io.notation_converter as nc


def valid_input_syntax(user_input: str):
    file_char = user_input[0]
    rank_chars = user_input[1:]

    if file_char in nc.file_index_of.keys() and rank_chars in \
            nc.rank_index_of.keys():
        return True
    else:
        return False


def get_user_input(prompt: str):
    valid_input = None

    while not valid_input:
        user_input = input(prompt).strip()

        if valid_input_syntax(user_input):
            valid_input = user_input
        else:
            print('Invalid notation')

    return valid_input


def get_input_indices(prompt: str):
    valid_input = get_user_input(prompt)
    return nc.alg_to_indices(valid_input)





