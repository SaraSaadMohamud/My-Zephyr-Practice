/*
 * Ring Buffer Module - Homework Test Skeleton
 *
 * test_fresh_state is provided as a worked example. Fill in the remaining
 * 7 ZTEST bodies according to TEST_SPEC.md. Stubs call ztest_test_skip()
 * so the binary builds and runs cleanly before each test is implemented.
 *
 * Run:
 *   west twister -T tests/ring_buf -p native_sim
 */

#include <zephyr/ztest.h>
#include <errno.h>

#include "ring_buf.h"

/*
 * Shared before hook: every suite reinitialises the ring buffer with a
 * capacity of 4 so tests start from a clean, known state. Capacity 4 is
 * enough to exercise FIFO order (push 1, 2, 3) and overflow (full at 4).
 */
static void before(void *f)
{
	ARG_UNUSED(f);
	rb_init(4);
}

/*
 * ============================================================================
 * Test Suite: ring_buf_init
 *
 * Initial state and re-initialization behaviour.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_init, NULL, NULL, before, NULL, NULL);

/* PROVIDED — study this test before writing the rest. */
ZTEST(ring_buf_init, test_fresh_state)
{
	zassert_true(rb_is_empty(), "Fresh buffer must be empty");
	zassert_equal(rb_count(), 0, "Fresh buffer count must be 0");
}

ZTEST(ring_buf_init, test_reinit_clears_state)
{
	/* TODO(l8-task1): Push a value, call rb_init(4) again, then
	 * verify the buffer is empty and count is 0.
	 * See TEST_SPEC.md "Suite ring_buf_init" #2.
	*/
	zassert_equal(rb_push(99), 0,
                  "Push should succeed before reinitialization");

    zassert_equal(rb_init(4), 0,
                  "Reinitialization should succeed");

    zassert_true(rb_is_empty(),
                 "Buffer should be empty after reinitialization");

    zassert_equal(rb_count(), 0,
                  "Buffer count should be zero after reinitialization");
}

/*
 * ============================================================================
 * Test Suite: ring_buf_push_pop
 *
 * Single push/pop round-trip, FIFO order, full error path.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_push_pop, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_push_pop, test_single_push_pop)
{
	/* TODO(l8-task1): rb_push(42), rb_pop(&v) -> v == 42, buffer empty after.
	 * See TEST_SPEC.md "Suite ring_buf_push_pop" #1.
	*/

	int value = 0;

	zassert_equal(rb_push(42), 0,
                  "Push should succeed");

	zassert_equal(rb_pop(&value), 0,
                  "Pop should succeed");

	zassert_equal(value, 42,
                  "Popped value should match pushed value");

	zassert_true(rb_is_empty(),
                 "Buffer should be empty after reinitialization");

}

ZTEST(ring_buf_push_pop, test_fifo_order)
{
	/* TODO(l8-task1): rb_push(1), rb_push(2), rb_push(3); pop three times
	 * and verify the values come out as 1, 2, 3 in that order.
	 * See TEST_SPEC.md "Suite ring_buf_push_pop" #2.
	 */

	int value = 0;

    zassert_equal(rb_push(1), 0, "Push 1 should succeed");
    zassert_equal(rb_push(2), 0, "Push 2 should succeed");
    zassert_equal(rb_push(3), 0, "Push 3 should succeed");

    zassert_equal(rb_pop(&value), 0, "First pop should succeed");
    zassert_equal(value, 1, "First popped value should be 1");

    zassert_equal(rb_pop(&value), 0, "Second pop should succeed");
    zassert_equal(value, 2, "Second popped value should be 2");

    zassert_equal(rb_pop(&value), 0, "Third pop should succeed");
    zassert_equal(value, 3, "Third popped value should be 3");

    zassert_true(rb_is_empty(), "Buffer should be empty after all pops");
}

ZTEST(ring_buf_push_pop, test_push_full_returns_enospc)
{
	/* TODO(l8-task1): Fill the buffer to its capacity of 4, then push
	 * one more value -> -ENOSPC.
	 * See TEST_SPEC.md "Suite ring_buf_push_pop" #3.
	 */

	zassert_equal(rb_push(1), 0, "Push 1 should succeed");
    zassert_equal(rb_push(2), 0, "Push 2 should succeed");
    zassert_equal(rb_push(3), 0, "Push 3 should succeed");
	zassert_equal(rb_push(4), 0, "Push 4 should succeed");

	zassert_true(rb_is_full(), "Buffer should be full");

    zassert_equal(rb_push(99), -ENOSPC,
                  "Push to a full buffer should return -ENOSPC");

    zassert_equal(rb_count(), 4,
                  "Buffer count should remain 4");
}

/*
 * ============================================================================
 * Test Suite: ring_buf_boundaries
 *
 * Peek semantics and NULL-pointer boundary conditions.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_boundaries, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_boundaries, test_peek_does_not_consume)
{
	/* TODO(l8-task1): rb_push(7); rb_peek(&v) -> v == 7; rb_peek(&v) again
	 * -> v == 7; rb_count() still == 1.
	 * See TEST_SPEC.md "Suite ring_buf_boundaries" #1.
	 */

	int value = 0;

    zassert_equal(rb_push(7), 0,
                  "Push should succeed");

    zassert_equal(rb_peek(&value), 0,
                  "First peek should succeed");

    zassert_equal(value, 7,
                  "Peek should return the first value");

    zassert_equal(rb_count(), 1,
                  "Peek should not change the buffer count");

    value = 0;

    zassert_equal(rb_peek(&value), 0,
                  "Second peek should succeed");

    zassert_equal(value, 7,
                  "Second peek should return the same value");

    zassert_equal(rb_count(), 1,
                  "Buffer count should remain unchanged");
}

ZTEST(ring_buf_boundaries, test_pop_null_returns_einval)
{
	/* TODO(l8-task1): rb_pop(NULL) -> -EINVAL.
	 * See TEST_SPEC.md "Suite ring_buf_boundaries" #2.
	 */
	 zassert_equal(rb_pop(NULL), -EINVAL,
                  "Pop with NULL Pointer should return -EINVAL");
}

ZTEST(ring_buf_boundaries, test_is_full_after_fill)
{
	/* TODO(l8-task1): push 4 values -> rb_is_full() == true, rb_count() == 4.
	 * See TEST_SPEC.md "Suite ring_buf_boundaries" #3.
	 */
	zassert_equal(rb_push(1), 0, "Push 1 should succeed");
    zassert_equal(rb_push(2), 0, "Push 2 should succeed");
    zassert_equal(rb_push(3), 0, "Push 3 should succeed");
    zassert_equal(rb_push(4), 0, "Push 4 should succeed");

    zassert_true(rb_is_full(),
                 "Buffer should be full after pushing 4 elements");

    zassert_equal(rb_count(), 4,
                  "Buffer count should equal its capacity");
}

ZTEST(ring_buf_init, test_init_zero_capacity_returns_einval)
{
    zassert_equal(rb_init(0U), -EINVAL,
                  "Initializing with zero capacity should return -EINVAL");
}

ZTEST(ring_buf_init, test_init_capacity_over_max_returns_einval)
{
    zassert_equal(rb_init(RING_BUF_MAX_CAPACITY + 1U), -EINVAL,
                  "Capacity above maximum should return -EINVAL");
}

ZTEST(ring_buf_push_pop, test_pop_empty_returns_enodata)
{
    int value = 0;

    zassert_equal(rb_init(4U), 0,
                  "Buffer initialization should succeed");

    zassert_equal(rb_pop(&value), -ENODATA,
                  "Popping from an empty buffer should return -ENODATA");
}

ZTEST(ring_buf_boundaries, test_peek_null_returns_einval)
{
    zassert_equal(rb_init(4U), 0,
                  "Buffer initialization should succeed");

    zassert_equal(rb_peek(NULL), -EINVAL,
                  "Peeking with NULL should return -EINVAL");
}

ZTEST(ring_buf_boundaries, test_peek_empty_returns_enodata)
{
    int value = 0;

    zassert_equal(rb_init(4U), 0,
                  "Buffer initialization should succeed");

    zassert_equal(rb_peek(&value), -ENODATA,
                  "Peeking an empty buffer should return -ENODATA");
}
