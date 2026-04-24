// Implementation for Problem 121 - Complex
// Only allowed headers
#include <iostream>
#include <iomanip>
#include <cmath>
#include <stdexcept>

namespace sjtu {

// Exception class for division by zero
class divided_by_zero final : public std::exception {
  public:
    divided_by_zero()  = default;
    ~divided_by_zero() = default;
    const char *what() const noexcept override { return "complex divided by zero!"; }
};

// Helper for floating-point sign with 1e-6 epsilon
inline int sign(double x) {
    const double eps = 1e-6;
    return (x > eps) - (x < -eps);
}

class complex {
  private:
    double a_, b_; // a_ + b_ i

  public:
    // Constructors
    complex() : a_(0.0), b_(0.0) {}
    complex(double a) : a_(a), b_(0.0) {}
    complex(double a, double b) : a_(a), b_(b) {}

    // Accessors (return references)
    double &real() { return a_; }
    double &imag() { return b_; }

    // Unary operators
    complex operator-() const { return complex(-a_, -b_); }
    complex operator~() const { return complex(a_, -b_); }

    // Binary arithmetic operators
    complex operator+(const complex &rhs) const { return complex(a_ + rhs.a_, b_ + rhs.b_); }
    complex operator-(const complex &rhs) const { return complex(a_ - rhs.a_, b_ - rhs.b_); }
    complex operator*(const complex &rhs) const {
        // (a+bi)(c+di) = (ac - bd) + (ad + bc)i
        double c = rhs.a_, d = rhs.b_;
        return complex(a_ * c - b_ * d, a_ * d + b_ * c);
    }
    complex operator/(const complex &rhs) const {
        double c = rhs.a_, d = rhs.b_;
        if (sign(c) == 0 && sign(d) == 0) {
            throw divided_by_zero();
        }
        double denom = c * c + d * d;
        // ((a+bi)/(c+di)) = ((ac+bd) + (bc - ad)i) / (c^2 + d^2)
        double real = (a_ * c + b_ * d) / denom;
        double imag = (b_ * c - a_ * d) / denom;
        return complex(real, imag);
    }

    // Compound assignment operators
    complex &operator+=(const complex &rhs) {
        a_ += rhs.a_;
        b_ += rhs.b_;
        return *this;
    }
    complex &operator-=(const complex &rhs) {
        a_ -= rhs.a_;
        b_ -= rhs.b_;
        return *this;
    }
    complex &operator*=(const complex &rhs) {
        double c = rhs.a_, d = rhs.b_;
        double na = a_ * c - b_ * d;
        double nb = a_ * d + b_ * c;
        a_ = na;
        b_ = nb;
        return *this;
    }
    complex &operator/=(const complex &rhs) {
        double c = rhs.a_, d = rhs.b_;
        if (sign(c) == 0 && sign(d) == 0) {
            throw divided_by_zero();
        }
        double denom = c * c + d * d;
        double na = (a_ * c + b_ * d) / denom;
        double nb = (b_ * c - a_ * d) / denom;
        a_ = na;
        b_ = nb;
        return *this;
    }

    // Equality with epsilon tolerance
    bool operator==(const complex &rhs) const {
        return std::fabs(a_ - rhs.a_) < 1e-6 && std::fabs(b_ - rhs.b_) < 1e-6;
    }

    // Conversion to bool: false iff both parts are ~0
    explicit operator bool() const { return !(std::fabs(a_) < 1e-6 && std::fabs(b_) < 1e-6); }

    // Output format: [/-]<real>[+/-]<imag>i with 6 decimals
    friend std::ostream &operator<<(std::ostream &os, const complex &x) {
        double r = (std::fabs(x.a_) < 1e-6 ? 0.0 : x.a_);
        double i = (std::fabs(x.b_) < 1e-6 ? 0.0 : x.b_);

        // Real part: print sign only if negative
        if (r < 0) {
            os << '-';
            os << std::fixed << std::setprecision(6) << -r;
        } else {
            os << std::fixed << std::setprecision(6) << r;
        }

        // Imag part sign
        os << (i < 0 ? '-' : '+');
        os << std::fixed << std::setprecision(6) << (i < 0 ? -i : i) << 'i';
        return os;
    }
};

} // namespace sjtu

