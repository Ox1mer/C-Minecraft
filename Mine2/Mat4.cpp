#include "Mat4.h"
#include <cmath>
#include <stdexcept>

Mat4::Mat4() {
    for (auto& row : data)
        row.fill(0.0);
}

Mat4::Mat4(double d) {
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            data[i][j] = (i == j ? d : 0.0);
}

Mat4 Mat4::identity() {
    return Mat4(1.0);
}

Mat4 Mat4::zero() {
    return Mat4();
}

Mat4 Mat4::translation(const Vec3& t) {
    Mat4 m = identity();
    m.data[0][3] = t.x;
    m.data[1][3] = t.y;
    m.data[2][3] = t.z;
    return m;
}

Mat4 Mat4::scale(const Vec3& s) {
    Mat4 m;
    m.data[0][0] = s.x;
    m.data[1][1] = s.y;
    m.data[2][2] = s.z;
    m.data[3][3] = 1.0;
    return m;
}

Mat4 Mat4::rotationX(double a) {
    double c = std::cos(a), s = std::sin(a);
    Mat4 m = identity();
    m.data[1][1] = c; m.data[1][2] = -s;
    m.data[2][1] = s; m.data[2][2] = c;
    return m;
}

Mat4 Mat4::rotationY(double a) {
    double c = std::cos(a), s = std::sin(a);
    Mat4 m = identity();
    m.data[0][0] = c; m.data[0][2] = s;
    m.data[2][0] = -s; m.data[2][2] = c;
    return m;
}

Mat4 Mat4::rotationZ(double a) {
    double c = std::cos(a), s = std::sin(a);
    Mat4 m = identity();
    m.data[0][0] = c; m.data[0][1] = -s;
    m.data[1][0] = s; m.data[1][1] = c;
    return m;
}

double& Mat4::operator()(int r, int c) {
    return data[r][c];
}

double Mat4::operator()(int r, int c) const {
    return data[r][c];
}

Mat4 Mat4::operator*(const Mat4& o) const {
    Mat4 r;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            double sum = 0.0;
            for (int k = 0; k < 4; ++k)
                sum += data[i][k] * o.data[k][j];
            r.data[i][j] = sum;
        }
    }
    return r;
}

Mat4& Mat4::operator*=(const Mat4& o) {
    *this = *this * o;
    return *this;
}

Vec4 Mat4::operator*(const Vec4& v) const {
    Vec4 r;
    r.x = data[0][0] * v.x + data[0][1] * v.y + data[0][2] * v.z + data[0][3] * v.w;
    r.y = data[1][0] * v.x + data[1][1] * v.y + data[1][2] * v.z + data[1][3] * v.w;
    r.z = data[2][0] * v.x + data[2][1] * v.y + data[2][2] * v.z + data[2][3] * v.w;
    r.w = data[3][0] * v.x + data[3][1] * v.y + data[3][2] * v.z + data[3][3] * v.w;
    return r;
}

Mat4 Mat4::transpose() const {
    Mat4 t;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            t.data[i][j] = data[j][i];
    return t;
}

static double det3(const std::array<std::array<double, 4>, 4>& m, int r0, int r1, int r2, int c0, int c1, int c2) {
    return
        m[r0][c0] * (m[r1][c1] * m[r2][c2] - m[r1][c2] * m[r2][c1]) -
        m[r0][c1] * (m[r1][c0] * m[r2][c2] - m[r1][c2] * m[r2][c0]) +
        m[r0][c2] * (m[r1][c0] * m[r2][c1] - m[r1][c1] * m[r2][c0]);
}

double Mat4::determinant() const {
    const auto& m = data;
    double det =
        m[0][0] * det3(m, 1, 2, 3, 1, 2, 3) -
        m[0][1] * det3(m, 1, 2, 3, 0, 2, 3) +
        m[0][2] * det3(m, 1, 2, 3, 0, 1, 3) -
        m[0][3] * det3(m, 1, 2, 3, 0, 1, 2);
    return det;
}

Mat4 Mat4::inverse() const {
    double det = determinant();
    if (std::fabs(det) < 1e-9)
        throw std::runtime_error("Matrix is singular and cannot be inverted");

    Mat4 inv;
    const auto& m = data;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            int r0 = (i + 1) % 4, r1 = (i + 2) % 4, r2 = (i + 3) % 4;
            int c0 = (j + 1) % 4, c1 = (j + 2) % 4, c2 = (j + 3) % 4;
            double cofactor = det3(m, r0, r1, r2, c0, c1, c2);
            inv.data[j][i] = ((i + j) % 2 ? -cofactor : cofactor) / det;
        }
    }
    return inv;
}

void Mat4::print() const {
    for (const auto& row : data) {
        for (double v : row)
            std::cout << v << " ";
        std::cout << "\n";
    }
}