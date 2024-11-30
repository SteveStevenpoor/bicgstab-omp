#include "Benchmark.h"
#include <gtest/gtest.h>

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);

  // Benchmark b;
  // b.ProduceTable();

  return RUN_ALL_TESTS();
}
