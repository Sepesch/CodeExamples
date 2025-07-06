from main import *
def test_rabin_karp():
    result = Rabin_Karp('abacaba', 'aba')
    assert result == [0, 4]


def test_graham():
    result = graham_scan([[3, 1], [6, 8], [1, 7], [9, 3], [9, 6], [9, 0]])
    assert result == [[1, 7], [3, 1], [9, 0], [9, 3], [9, 6], [6, 8]]


def test_graham_square():
    result = graham_scan([[3, 1], [6, 8], [1, 7], [9, 3], [9, 6], [9, 0]])
    assert calculate_area(result) == 47.5