from main import Node, insert, delete, delete_min, delete_max
def run_tests():
    test_insert()
    test_delete()
    test_delete_min()
    test_delete_max()
def test_insert():
    root = None
    values = [1, 2, 3, 4, 5, 6]
    for value in values:
        root = insert(value, root)

    assert root.val == 4
    assert root.left.val == 2
    assert root.right.val == 5
    assert root.left.left.val == 1
    assert root.left.right.val == 3
    assert root.right.right.val == 6

    print("Test insert passed!")

def test_delete():
    root = None
    values = [1, 2, 3, 4, 5, 6]
    for value in values:
        root = insert(value, root)

    root = delete(2, root)
    assert root.val == 4
    assert root.left.val == 3
    assert root.left.left.val == 1
    assert root.left.right is None

    print("Test delete passed!")

def test_delete_min():
    root = None
    values = [1, 2, 3, 4, 5, 6]
    for value in values:
        root = insert(value, root)
    root = delete_min(root)
    assert root.val == 4
    assert root.left.val == 2
    assert root.left.left is None
    assert root.left.right.val == 3

    print("Test delete_min passed!")

def test_delete_max():
    root = None
    values = [1, 2, 3, 4, 5, 6]
    for value in values:
        root = insert(value, root)

    root = delete_max(root)
    assert root.val == 4
    assert root.right.val == 5
    assert root.right.right is None

    print("Test delete_max passed!")


run_tests()