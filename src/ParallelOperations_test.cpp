#include "ParallelOperations.h"
#include <Eigen>
#include <gtest/gtest.h>
#include <iostream>
#include <random>


using namespace Eigen;


class ParallelOperationsTest : public testing::Test {
 protected:
    int n = 110;
    SparseMatrix<double, Eigen::RowMajor> A;
    VectorXd x, y, b;

    ParallelOperationsTest() : A(SparseMatrix<double, Eigen::RowMajor>(n, n)),
                               x(VectorXd(n)), y(VectorXd(n)), b(VectorXd::Ones(n)) {
        double lower_bound = 0;
        double upper_bound = 1;
        std::uniform_real_distribution<double> unif(lower_bound,upper_bound);
        std::default_random_engine re;

        for (int i = 0; i < n; i++) {
            A.insert(i, i) = unif(re) + 1;
            if (i - 2 >= 0) A.insert(i, i - 2) = unif(re);
            if (i - 1 >= 0) A.insert(i, i - 1) = unif(re);
            if (i + 1 < n) A.insert(i, i + 1) = unif(re);
            if (i + 2 < n) A.insert(i, i + 2) = unif(re);
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
    int iter = 0;

    ParallelOperations::BiCGStab(A, x, b, tolerance, max_iter*4, iter);
    std::cout << "BICGSTAB ITER: " << iter << std::endl;
    std::cout << "BICGSTAB TOLERANCE: " << tolerance << std::endl;

    tolerance = 1e-6;
    iter = 0;

    ParallelOperations::Eigen_BiCGStab(A, y, b, tolerance, max_iter*4, iter);
    std::cout << "BICGSTAB EIGEN ITER: " << iter << std::endl;
    std::cout << "BICGSTAB EIGEN TOLERANCE: " << tolerance << std::endl;


    // for (int i = 0; i < n; i++) {
    //     std::cout << x[i] << "    " << y[i] << std::endl;
    // }

    EXPECT_TRUE(isVectorXdEq(x, y));
    EXPECT_FALSE(isVectorXdEq(x, b));
    EXPECT_FALSE(isVectorXdEq(y, VectorXd::Zero(n)));
}

