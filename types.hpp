#include <Eigen/Core>

template< typename Scalar = float, int rows = Eigen::Dynamic, int cols = Eigen::Dynamic >
using MatrixRM = Eigen::Matrix< Scalar, rows, cols, Eigen::RowMajor >;

template< typename Scalar = float, int rows = Eigen::Dynamic, int cols = Eigen::Dynamic >
using MatrixCM = Eigen::Matrix< Scalar, rows, cols, Eigen::ColMajor >;

