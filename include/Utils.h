#pragma once

#include <Eigen>
#include <chrono>

using namespace std::chrono;
using namespace Eigen;

class Utils {
public:
  template <typename Func, typename... Args>
  static double GetExecTime(Func &&f, Args &&...args) {
    auto begin = high_resolution_clock::now();
    f(std::forward<Args>(args)...);
    auto end = high_resolution_clock::now();
    duration<double> time = end - begin;

    return time.count();
  }

  static VectorXd mv_product_seq(const SparseMatrix<double, Eigen::RowMajor> &A,
                                 const VectorXd &v) {
    int rows = A.rows();
    VectorXd res = VectorXd(rows);
    for (int row = 0; row < rows; row++) {
      double res_row = 0.0;
      for (SparseMatrix<double, Eigen::RowMajor>::InnerIterator it(A, row); it;
           ++it) {
        res_row += it.value() * v(it.col());
      }
      res[row] = res_row;
    }

    return res;
  }

  static void Seq_BiCGStab(const SparseMatrix<double, Eigen::RowMajor> &A,
                           VectorXd &x, const VectorXd &b,
                           const double &tolerance, const int &max_iter) {

    size_t n = A.cols();

    VectorXd r = b - mv_product_seq(A, x);
    VectorXd r0 = r;

    double r0_norm = r0.squaredNorm();
    double b_norm = b.squaredNorm();

    int current_iter = 0;
    int restarts = 0;

    double tol2 = tolerance * tolerance * b_norm;
    double eps2 = NumTraits<VectorXd::Scalar>::epsilon();
    eps2 *= eps2;

    if (b_norm == 0) {
      x = VectorXd::Zero(n);
      return;
    }

    double rho = 1;
    double alpha = 1;
    double w = 1;

    VectorXd Ap = VectorXd::Zero(n), p = VectorXd::Zero(n);
    VectorXd As = VectorXd::Zero(n), s = VectorXd::Zero(n);

    while (r.squaredNorm() > tol2 && current_iter < max_iter) {
      double rho_old = rho;
      rho = r0.dot(r);

      // In case an old residual is almost orthogonal to a new residual.
      // If we don't restart, the convergence speed will be too low .
      if (std::abs(rho) < eps2 * r0_norm) {
        r = b - mv_product_seq(A, x);
        r0 = r;
        rho = r0_norm = r.squaredNorm();
        if (restarts++ == 0)
          current_iter = 0;
      }

      double beta = (rho / rho_old) * (alpha / w);
      p = r + beta * (p - w * Ap);

      Ap = mv_product_seq(A, p);
      alpha = rho / r0.dot(Ap);

      s = r - alpha * Ap;
      As = mv_product_seq(A, s);

      double tmp = As.squaredNorm();
      if (tmp > 0.0) {
        w = As.dot(s) / tmp;
      } else {
        w = 0;
      }

      x += alpha * p + w * s;
      r = s - w * As;

      ++current_iter;
    }
  }
};