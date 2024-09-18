import numpy as np
import xiangqigame_core  # Your pybind-generated module

# A dictionary to map (is_signed, size) to corresponding NumPy dtype
dtype_map = {
    (True, 1): np.int8,
    (True, 2): np.int16,
    (True, 4): np.int32,
    (True, 8): np.int64,
    (False, 1): np.uint8,
    (False, 2): np.uint16,
    (False, 4): np.uint32,
    (False, 8): np.uint64,
}


def get_dynamic_integer_type():
    """Determines and returns the NumPy dtype based on integer type info from xiangqigame_core."""
    is_signed = xiangqigame_core.is_signed_points_type()
    size = xiangqigame_core.size_of_points_type()

    key = (is_signed, size)
    if key in dtype_map:
        return dtype_map[key]
    else:
        raise ValueError(f"Unsupported type with size {size} and signedness {is_signed}")


# Example usage: create a NumPy array with the dynamically determined type
dtype = get_dynamic_integer_type()
array = np.array([1, 2, 3, 4], dtype=dtype)

print("Array:", array)
print("Dtype of array:", array.dtype)


# Example function using the dynamic dtype
def process_array(arr: np.ndarray) -> np.generic:
    """Process array and ensure sum has the same dtype as the array."""
    return np.sum(arr, dtype=arr.dtype)


# Call the function with the dynamically typed array
result = process_array(array)

print("Result:", result)
print("Dtype of result:", type(result))
