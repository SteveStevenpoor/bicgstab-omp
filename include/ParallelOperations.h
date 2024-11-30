#pragma once

#include <Eigen>
#include <iostream>
#include <omp.h>

using namespace std;
using namespace Eigen;

class ParallelOperations {
private:
  static const int round_robin_cycle = 1;

public:
  ParallelOperations() = delete;

  static VectorXd mv_product(const SparseMatrix<double, Eigen::RowMajor> &A,
                             const VectorXd &v);

  static void BiCGStab(const SparseMatrix<double, Eigen::RowMajor> &A,
                       VectorXd &x, const VectorXd &b, double &tolerance,
                       const int &max_iter, int &iter);

  static void Eigen_BiCGStab(const SparseMatrix<double, Eigen::RowMajor> &A,
                             VectorXd &x, const VectorXd &b,
                             double &tolerance, const int &max_iter, int &iter);
};
