#pragma once

#include <concepts>


template <typename T, typename KeyType>
concept PseudoRandomlyGeneratedComponentConcept = requires(
    T t
) {
    {t.seed()} -> std::same_as<KeyType>;
};