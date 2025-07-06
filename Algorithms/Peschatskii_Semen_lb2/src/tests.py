import random
from main import *


def test_insertion_sort():
    arr = [3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5]
    sorted_arr = insertion_sort(arr)
    assert sorted_arr == [9, 6, 5, 5, 5, 4, 3, 3, 2, 1, 1], f"Error: {sorted_arr}"
    print("test_insertion_sort passed")


def test_separate_arr():
    arr = [3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5]
    min_run = 3
    runs = separate_arr(arr, min_run)
    assert runs == [[4, 3, 1], [9, 5, 1], [6, 5, 2], [5, 3]], f"Error: {runs}"
    print("test_separate_arr passed")


def test_is_sorted_abs():
    arr1 = [1, 2, 3, 4]
    arr2 = [4, 3, 2, 1]
    arr3 = [1, 3, 2, 4]
    assert is_sorted_abs(arr1) == (True, False), f"Error: {is_sorted_abs(arr1)}"
    assert is_sorted_abs(arr2) == (False, True), f"Error: {is_sorted_abs(arr2)}"
    assert is_sorted_abs(arr3) == (False, False), f"Error: {is_sorted_abs(arr3)}"
    print("test_is_sorted_abs passed")


def test_merge_arr():
    stack = Stack()
    arr1 = [1, 3, 5]
    arr2 = [2, 4, 6]
    merged_arr = stack.merge_arr(arr1, arr2, 3)
    assert merged_arr == [2, 4, 6, 1, 3, 5], f"Error: {merged_arr}"
    print("test_merge_arr passed")



def test_timsort_empty_list():
    assert tim_sort([]) == []


def test_timsort_single_element():
    assert tim_sort([42]) == [42]


def test_timsort_already_sorted():
    assert tim_sort([5, -5, 4, 3, -2]) == [5, -5, 4, 3, -2]


def test_timsort_reverse_sorted():
    assert tim_sort([1, 2, 3, 4, 5]) == [5, 4, 3, 2, 1]


def run_tests():
    test_insertion_sort()
    test_separate_arr()
    test_is_sorted_abs()
    test_merge_arr()
    test_timsort_empty_list()
    test_timsort_single_element()
    test_timsort_already_sorted()
    test_timsort_reverse_sorted()

    print("All tests passed")


if __name__ == "__main__":
    run_tests()