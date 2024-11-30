#pragma once

#include "CSVWriter.h"
#include "ParallelOperations.h"
#include "Utils.h"

using namespace std;

class Benchmark {
  SparseMatrix<double, Eigen::RowMajor> A;
  VectorXd x, y, b;
  CSVWriter w;
  double tolerance = 1e-6;

private:
  void CreateMatrix(const int &n) {
    A = SparseMatrix<double, Eigen::RowMajor>(n, n);
    x = VectorXd(n);
    y = VectorXd(n);
    b = VectorXd::Ones(n);
    for (int i = 0; i < n; i++) {
      A.insert(i, i) = static_cast<double>(i) + 123.172 + i * i;
      if (i > 0 && i < n - 1) {
        A.insert(i, i + 1) = i * i + 973.1;
        A.insert(i, i - 1) = i * i + 324.12324;
      }
    }
  }

public:
  void ProduceTable() {
    vector<int> ns = {200, 500, 1000};
    int iter = 0;
    for (int n : ns) {
      w << n << (string)",";

      CreateMatrix(n);
      omp_set_num_threads(2);
      w << Utils::GetExecTime(ParallelOperations::BiCGStab, A, x, b, tolerance,
                              2 * n, iter) << (string)",";
      w << Utils::GetExecTime(ParallelOperations::Eigen_BiCGStab, A, y, b,
                              tolerance, 2 * n, iter) << (string)",";

      x(VectorXd(n));
      y(VectorXd(n));

      omp_set_num_threads(4);
      w << Utils::GetExecTime(ParallelOperations::BiCGStab, A, x, b, tolerance,
                              2 * n, iter) << (string)",";
      w << Utils::GetExecTime(ParallelOperations::Eigen_BiCGStab, A, y, b,
                              tolerance, 2 * n, iter) << (string)",";

      x(VectorXd(n));
      w << Utils::GetExecTime(Utils::Seq_BiCGStab, A, x, b, tolerance, 2 * n);

      w.endl();
    }
  }
};