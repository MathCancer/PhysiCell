/*
 * dfba_test.h
 *
 * Minimal assertion helpers for the standalone dFBA tests.
 *
 * The pre-existing test programs in this directory print results and always
 * exit 0, so they cannot fail a build. These macros record failures and
 * DFBA_TEST_SUMMARY() returns a non-zero exit code when any check failed, so a
 * test binary can actually gate a change.
 *
 * Deliberately header-only and dependency-free: AGENTS.md rules out pulling in
 * a test framework.
 */

#ifndef DFBA_TEST_H_
#define DFBA_TEST_H_

#include <cmath>
#include <iostream>
#include <sstream>
#include <string>

namespace dfba_test {

static int checks_run = 0;
static int checks_failed = 0;
static std::string current_section = "";

inline void section(const std::string& name)
{
    current_section = name;
    std::cout << "\n--- " << name << " ---" << std::endl;
}

inline void report(bool ok, const std::string& what, const std::string& detail)
{
    checks_run++;
    if (ok)
    {
        std::cout << "  ok   " << what << std::endl;
        return;
    }
    checks_failed++;
    std::cout << "  FAIL " << what << std::endl;
    if (!detail.empty())
        std::cout << "         " << detail << std::endl;
}

template <typename T>
inline std::string to_str(const T& value)
{
    std::ostringstream os;
    os << value;
    return os.str();
}

inline int summary()
{
    std::cout << "\n=================================================" << std::endl;
    std::cout << checks_run - checks_failed << "/" << checks_run << " checks passed";
    if (checks_failed > 0)
        std::cout << "  (" << checks_failed << " FAILED)";
    std::cout << std::endl;
    std::cout << "=================================================" << std::endl;
    return checks_failed == 0 ? 0 : 1;
}

} // namespace dfba_test

#define CHECK(cond, what) \
    dfba_test::report((cond), (what), "")

/* Arguments are evaluated exactly once: test expressions may have side effects
   (knockOutGene/restoreGene return a count and mutate the model). */
#define CHECK_EQ(actual, expected, what)                                        \
    do {                                                                        \
        auto _a = (actual);                                                     \
        auto _e = (expected);                                                   \
        std::ostringstream _os;                                                 \
        _os << "expected " << _e << ", got " << _a;                             \
        dfba_test::report(_a == _e, (what), _os.str());                         \
    } while (0)

#define CHECK_NEAR(actual, expected, tol, what)                                 \
    do {                                                                        \
        auto _a = (actual);                                                     \
        auto _e = (expected);                                                   \
        auto _t = (tol);                                                        \
        std::ostringstream _os;                                                 \
        _os << "expected " << _e << " +/- " << _t << ", got " << _a;            \
        dfba_test::report(std::fabs(_a - _e) <= _t, (what), _os.str());         \
    } while (0)

#define DFBA_TEST_SECTION(name) dfba_test::section(name)
#define DFBA_TEST_SUMMARY()     dfba_test::summary()

#endif /* DFBA_TEST_H_ */
