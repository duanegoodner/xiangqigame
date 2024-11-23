#pragma once

#include <concepts>
#include <gtest/gtest.h>
#include <memory>


// Concept to ensure the builder has a build method returning a std::shared_ptr<ObjectType>
template<typename Builder, typename Object>
concept BuilderConcept = requires(Builder b) {
    { b.build() } -> std::same_as<std::shared_ptr<Object>>;
};

// Struct to encapsulate both builder and object types
template <typename BuilderType, typename ObjectType>
struct BuilderObjectPair {
  using Builder = BuilderType;
  using Object = ObjectType;
};

// Test fixture template using the BuilderConcept
template <typename T>
requires BuilderConcept<typename T::Builder, typename T::Object>
class SharedPtrBuilderTest : public ::testing::Test {
protected:
    typename T::Builder builder_;  // Access the builder type from the template parameter
};

TYPED_TEST_SUITE_P(SharedPtrBuilderTest);

TYPED_TEST_P(SharedPtrBuilderTest, BuilderReturnsValidSharedPtr) {
  auto object = this->builder_.build();
  ASSERT_NE(object, nullptr) << "Builder should not return nullptr";
  ASSERT_EQ(object.use_count(), 1) << "Initial use count should be 1";
}

TYPED_TEST_P(SharedPtrBuilderTest, SharedPtrCopyIncreasesRefCount) {
  auto object = this->builder_.build();
  {
    auto object_copy = object;
    EXPECT_EQ(object.use_count(), 2);
  }
  ASSERT_EQ(object.use_count(), 1)
      << "Use count should return to 1 after copy is destroyed";
}

TYPED_TEST_P(SharedPtrBuilderTest, SharedPtrMoveDoesNotIncreaseRefCount) {
  auto object = this->builder_.build();
  {
    auto object_moved = std::move(object);
    EXPECT_EQ(object_moved.use_count(), 1) << "Use count should remain 1 after move";
    EXPECT_EQ(object, nullptr) << "Original shared_ptr should be null after move";
  }
  EXPECT_EQ(object, nullptr)
      << "object should still be in scope (with value = null)";
}


REGISTER_TYPED_TEST_SUITE_P(
    SharedPtrBuilderTest,
    BuilderReturnsValidSharedPtr,
    SharedPtrCopyIncreasesRefCount,
    SharedPtrMoveDoesNotIncreaseRefCount
    // Other tests can be registered here
);
