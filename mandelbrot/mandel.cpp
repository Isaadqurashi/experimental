#include <cstdio>
#include <iostream>
#include <complex>
#include <cmath>
#include <iomanip>


struct ScreenBuffer {
    ScreenBuffer(size_t w, size_t h)
        : w{w}, h{h}, buffer{std::make_unique<char[]>(w * h)} {}

    std::ostream& draw(std::ostream& os) const {
        for (size_t y = 0; y < h; ++y) {
            os << "[";
            for (size_t x = 0; x < w; ++x) {
                char c = buffer[y * w + x];
                os << (c ? c : ' ');
            }
            os << "]\n";
        }
        return os;
    }

    size_t w;
    size_t h;

    std::unique_ptr<char[]> buffer;
};

static constexpr std::complex<double> kC{.0, .5};

bool member(std::complex<double> z) {
    // return z.real() > z.imag();
    for (int k = 0; k < 200; ++k)
        z = z*z + kC;
    return std::abs(z) <= 100;
}

void calc(ScreenBuffer& screen) {
    double yMin = -2.;
    double yMax = 2.;
    double xMin = -2.;
    double xMax = 2.;

    std::cout << std::fixed << std::setprecision(2);

    for (size_t y = 0; y < screen.h; ++y) {
        double ip = yMax + y * (yMin - yMax) / screen.h;
        for (size_t x = 0; x < screen.w; ++x) {
            double rp = xMin + x * (xMax - xMin) / screen.w;
            std::complex<double> z{rp, ip};
            //std::cout << "(" << x << ":" << y << ":" << z << ")" << std::endl;
            if (member(z)) {
                screen.buffer[y * screen.w + x] = 'x';
            }
        }
    }
}

int main(int argc, char** argv) {
    ScreenBuffer buf{120, 80};
    calc(buf);
    buf.draw(std::cout);
    return 0;
}
