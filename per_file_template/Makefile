
OBJS=main.o other_file.o

TARGET=per_file_template_test

all: $(TARGET)

main.o: main.cpp logv2.h
other_file.o: other_file.cpp logv2.h

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@  $(OBJS)

test: $(TARGET)
	./$(TARGET)

clean:
	rm $(TARGET) $(OBJS)

compile_commands.json:
	compiledb make

compiledb: compile_commands.json

tiny_test:

CXX=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang++

CXXFLAGS=-Wno-implicit-int-float-conversion \
	-Woverloaded-virtual \
	-Werror=unused-result \
	-Wpessimizing-move \
	-Wredundant-move \
	-Wno-undefined-var-template \
	-Wno-instantiation-after-specialization \
	-fsized-deallocation \
	-Wno-defaulted-function-deleted \
	-Wunused-exception-parameter \
	-stdlib=libc++ \
	-std=c++17 \
	-isysroot \
	/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX11.1.sdk \
	-mmacosx-version-min=10.13 \
	-target darwin17.0.0 \
	-arch x86_64 \
	-fno-omit-frame-pointer \
	-fno-strict-aliasing \
	-fasynchronous-unwind-tables \
	-ggdb \
	-Wall \
	-Wsign-compare \
	-Wno-unknown-pragmas \
	-Winvalid-pch \
	-Werror \
	-O0 \
	-Wno-unused-local-typedefs \
	-Wno-unused-function \
	-Wno-unused-private-field \
	-Wno-deprecated-declarations \
	-Wno-tautological-constant-out-of-range-compare \
	-Wno-tautological-constant-compare \
	-Wno-tautological-unsigned-zero-compare \
	-Wno-tautological-unsigned-enum-zero-compare \
	-Wno-unused-const-variable \
	-Wno-missing-braces \
	-Wno-inconsistent-missing-override \
	-Wno-potentially-evaluated-expression \
	-Wno-unused-lambda-capture \
	-Wno-exceptions \
	-Wunguarded-availability \
	-fstack-protector-strong \
	-fno-builtin-memcmp \
	-DSAFEINT_USE_INTRINSICS=0 \
	-DPCRE_STATIC \
	-DBOOST_THREAD_VERSION=5 \
	-DBOOST_THREAD_USES_DATETIME \
	-DBOOST_SYSTEM_NO_DEPRECATED \
	-DBOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS \
	-DBOOST_ENABLE_ASSERT_DEBUG_HANDLER \
	-DBOOST_LOG_NO_SHORTHAND_NAMES \
	-DBOOST_LOG_USE_NATIVE_SYSLOG \
	-DBOOST_LOG_WITHOUT_THREAD_ATTR \
	-DABSL_FORCE_ALIGNED_ACCESS \

