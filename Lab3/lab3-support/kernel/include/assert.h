/**
 * @file assert.h
 *
 * @brief Assertion and debugging infrastructure.
 *
 * @author Kartik Subramanian
 */

#ifndef _ASSERT_H_
#define _ASSERT_H_

#ifdef NDEBUG

#define assert(e) ((void)0)

#else

void panic(const char* fmt, ...) __attribute__((noreturn, format (printf, 1, 2)));

#define assert(e) \
	((void)((e) ? 0 : (panic("Assertion Failure at %s:%d\n%s", __FILE__, \
		__LINE__, #e), 0)))
#endif

#endif /* _ASSERT_H_ */

