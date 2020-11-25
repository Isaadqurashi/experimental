
struct CatchState {
    void next() {
        if (pos == 0)
            throw;
        probes[--pos](*this);
    }

    template <typename T, typename F>
    void typedProbe(F&& f) {
        try {
            next();
        } catch (const T& ex) {
            f(ex);
        }
    }

    const std::vector<std::function<void(CatchState&)>> probes;
    const std::exception_ptr ep;
    const std::type_info*& ti;
    std::ostream& os;
    size_t pos;
};

void addExceptionInfoProbe(std::function<void(CatchState&)> probe);

void addExceptionInfoProbe(std::function<void(CatchState&)> probe) {
    _exceptionInfoProbes().push_back(probe);
}

void probeException() {
    mallocFreeOStream << "terminate() called.";
    auto ep = std::current_exception();
    if (!ep) {
        mallocFreeOStream << " No exception is active";
        return;
    }
    mallocFreeOStream << " An exception is active; attempting to gather more information";
    writeMallocFreeStreamToLog();

    const std::type_info* typeInfo = nullptr;

    std::vector<std::function<void(CatchState&)>> probes{
        [](CatchState& state) {
            state.typedProbe<DBException>([&](auto&& ex) {
                state.ti = &typeid(ex);
                state.os << "DBException::toString(): " << redact(ex) << '\n';
            });
        },
        [](CatchState& state) {
            state.typedProbe<std::exception>([&](auto&& ex) {
                state.ti = &typeid(ex);
                state.os << "std::exception::what(): " << redact(ex.what()) << '\n';
            });
        },
        [](CatchState& state) {
            state.typedProbe<boost::exception>([&](auto&& ex) {
                state.ti = &typeid(ex);
                state.os << "boost::diagnostic_information(): "
                            << boost::diagnostic_information(ex) << '\n';
            });
        },
    };
    probes.insert(probes.end(), _exceptionInfoProbes().begin(), _exceptionInfoProbes().end());

    try {
        CatchState{probes, ep, typeInfo, mallocFreeOStream, probes.size()}.next();
    } catch(...) {}

    try {
        try {
            throw;
        } catch (const DBException& ex) {
            typeInfo = &typeid(ex);
            mallocFreeOStream << "DBException::toString(): " << redact(ex) << '\n';
        } catch (const std::exception& ex) {
            typeInfo = &typeid(ex);
            mallocFreeOStream << "std::exception::what(): " << redact(ex.what()) << '\n';
        } catch (const boost::exception& ex) {
            typeInfo = &typeid(ex);
            mallocFreeOStream << "boost::diagnostic_information(): "
                              << boost::diagnostic_information(ex) << '\n';
        } catch (...) {
            mallocFreeOStream << "A non-standard exception type was thrown\n";
        }

        if (typeInfo) {
            const std::string name = demangleName(*typeInfo);
            mallocFreeOStream << "Actual exception type: " << name << '\n';
        }
    } catch (...) {
        mallocFreeOStream << "Exception while trying to print current exception.\n";
        if (typeInfo) {
            // It is possible that we failed during demangling. At least try to print the
            // mangled name.
            mallocFreeOStream << "Actual exception type: " << typeInfo->name() << '\n';
        }
    }
}
