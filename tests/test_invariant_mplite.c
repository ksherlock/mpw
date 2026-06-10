#include <check.h>
#include <stdlib.h>
#include <string.h>
#include "mplite/mplite.h"

START_TEST(test_double_free_no_overlapping_allocations)
{
    // Invariant: After any sequence of alloc/free operations, no two live
    // allocations should ever overlap (share the same memory region)
    
    // Setup pool allocator
    char pool_buffer[4096];
    mplite_t pool;
    int ret = mplite_init(&pool, pool_buffer, sizeof(pool_buffer), 8, NULL);
    ck_assert_int_eq(ret, MPLITE_OK);
    
    // Test cases: allocation sizes to test
    size_t alloc_sizes[] = {16, 32, 64};
    int num_sizes = sizeof(alloc_sizes) / sizeof(alloc_sizes[0]);
    
    for (int i = 0; i < num_sizes; i++) {
        size_t size = alloc_sizes[i];
        
        // Allocate first block
        void *ptr1 = mplite_malloc(&pool, size);
        ck_assert_ptr_nonnull(ptr1);
        memset(ptr1, 0xAA, size);
        
        // Free it (first free - valid)
        mplite_free(&pool, ptr1);
        
        // Double free (the vulnerability) - this corrupts free list
        mplite_free(&pool, ptr1);
        
        // Now allocate two blocks - they should NOT overlap
        void *ptr2 = mplite_malloc(&pool, size);
        void *ptr3 = mplite_malloc(&pool, size);
        
        if (ptr2 != NULL && ptr3 != NULL) {
            // Security invariant: two live allocations must not be the same
            ck_assert_msg(ptr2 != ptr3,
                "SECURITY VIOLATION: Two allocations returned same address "
                "(overlapping memory) after double-free");
            
            // Write to both and verify no corruption
            memset(ptr2, 0xBB, size);
            memset(ptr3, 0xCC, size);
            
            unsigned char *p2 = (unsigned char *)ptr2;
            unsigned char *p3 = (unsigned char *)ptr3;
            
            for (size_t j = 0; j < size; j++) {
                ck_assert_msg(p2[j] == 0xBB,
                    "SECURITY VIOLATION: ptr2 corrupted by write to ptr3");
                ck_assert_msg(p3[j] == 0xCC,
                    "SECURITY VIOLATION: ptr3 corrupted by write to ptr2");
            }
            
            mplite_free(&pool, ptr2);
            mplite_free(&pool, ptr3);
        }
    }
}
END_TEST

Suite *security_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Security");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_double_free_no_overlapping_allocations);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = security_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}