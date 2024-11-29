#include "ParallelOperations.h"
#include <Eigen>
#include <gtest/gtest.h>
#include <iostream>


using namespace Eigen;


class ParallelOperationsTest : public testing::Test {
 protected:
    int n = 1000;
    SparseMatrix<double, Eigen::RowMajor> A;
    VectorXd x, y, b;

    ParallelOperationsTest() : A(SparseMatrix<double, Eigen::RowMajor>(n, n)),
                               x(VectorXd(n)), y(VectorXd(n)), b(VectorXd::Ones(n)) {
        for (int i = 0; i < n; i++) {
            A.insert(i, i) = static_cast<double>(i) + 123.172 + i*i;
            if (i > 0 && i < n - 1) {
                A.insert(i, i + 1) = i*i + 973.1;
                A.insert(i, i - 1) = i*i + 324.12324;
            }
        }
    }

};


bool isVectorXdEq(VectorXd v, VectorXd u) {
    bool isEq = true;
    if (v.size() == u.size()) {
        for (int i = 0; i < v.size(); i++) {
            isEq = isEq & (std::fabs(v[i] - u[i]) < 1e-6);
        }
    } else {
        isEq = false;
    }

    return isEq;
}


TEST_F(ParallelOperationsTest, TestingMVProduct) {
    VectorXd v = b;
    VectorXd u = A * v;
    EXPECT_TRUE(isVectorXdEq(ParallelOperations::mv_product(A, v), u));

    v = u;
    u = A * v;
    EXPECT_TRUE(isVectorXdEq(ParallelOperations::mv_product(A, v), u));

    v = u;
    u = A * v;
    EXPECT_TRUE(isVectorXdEq(ParallelOperations::mv_product(A, v), u));

    v = u;
    u = A * v;
    EXPECT_FALSE(isVectorXdEq(ParallelOperations::mv_product(A, v), b));
}

TEST_F(ParallelOperationsTest, TestBiCGStab) {
    double tolerance = 1e-6;
    int max_iter = 2 * n;
    ParallelOperations::BiCGStab(A, x, b, tolerance, max_iter);
    ParallelOperations::Eigen_BiCGStab(A, y, b, tolerance, max_iter);

    EXPECT_TRUE(isVectorXdEq(x, y));
    EXPECT_FALSE(isVectorXdEq(x, b));
    EXPECT_FALSE(isVectorXdEq(y, VectorXd::Zero(n)));
}

