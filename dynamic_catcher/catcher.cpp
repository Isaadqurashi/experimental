
#include <functional>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <typeinfo>

// 


class CatchState {
public:
    using Probe = std::function<std::string(CatchState&)>;

    explicit CatchState(std::vector<Probe> probes)
        : _probes(std::move(probes)) {}

    std::string doCatch() {
        _pos = _probes.size();
        return _nextProbe();
    }

    template <typename T, typename F>
    std::string typedProbe(F&& desc) {
        try {
            return _nextProbe();
        } catch (const T& ex) {
            return desc(ex);
        }
    }

private:
    std::string _nextProbe() {
        if (!_pos) throw;
        return _probes[--_pos](*this);
    }

    std::vector<Probe> _probes;
    size_t _pos;
};

decltype(auto) _exceptionInfoProbes() {
    static auto& v = *new std::vector<CatchState::Probe>{};
    return v;
}

void addExceptionInfoProbe(CatchState::Probe probe) {
    _exceptionInfoProbes().push_back(probe);
}

//  Frontmatter for the test....

namespace boost {
struct exception{};
std::string diagnostic_information(exception) { return "boost::exception"; }
}
struct DBException {};
std::string redact(const DBException&) { return "redacted DBException"; };
std::string redact(std::string) { return "redacted string"; };

void initProbes() {
    addExceptionInfoProbe([](CatchState& state) {
            return state.typedProbe<DBException>([](auto&& ex) {
                    std::ostringstream os;
                    os << "DBException::toString(): " << redact(ex) << '\n';
                    return os.str();
                    });
            });
    addExceptionInfoProbe([](CatchState& state) {
            return state.typedProbe<std::exception>([](auto&& ex) {
                    std::ostringstream os;
                    os << "std::exception::what(): " << redact(ex.what()) << '\n';
                    return os.str();
                    });
            });
    addExceptionInfoProbe([](CatchState& state) {
            return state.typedProbe<boost::exception>([](auto&& ex) {
                    std::ostringstream os;
                    os << "boost::diagnostic_information(): "
                       << boost::diagnostic_information(ex) << '\n';
                    return os.str();
                    });
            });
}

std::ostream& probeException(std::ostream& os) {
    if (!std::current_exception()) {
        os << "No active exception";
    } else {
        try {
            os << CatchState{_exceptionInfoProbes()}.doCatch();
        } catch (...) {
            os << "Exception while trying to print current exception" << std::endl;;
        }
    }
    return os;
}

struct WeirdException {};
static auto dum1 = (addExceptionInfoProbe([](CatchState& state) {
        return state.typedProbe<WeirdException>([](auto&& ex) {
            return "WeirdException";
        });
    }), 0);

struct SpecificStdException : std::exception {};
static auto dum2 = (addExceptionInfoProbe([](CatchState& state) {
        return state.typedProbe<SpecificStdException>([](auto&& ex) {
            return "SpecificStdException";
        });
    }), 0);


template <typename T>
std::ostream& testProbe(std::ostream& os, T&& obj) {
    try {
        throw obj;
    } catch (...) {
        return os << probeException << std::endl;
    }
}

int main() {
    initProbes();
    try {
        std::ostream& os = std::cout;
        testProbe(os, DBException{});
        testProbe(os, WeirdException{});
        testProbe(os, std::out_of_range{"testing"});
        testProbe(os, SpecificStdException{});
    } catch(...) {
        std::cout << "uncaught\n";
    }
    return 0;
}
