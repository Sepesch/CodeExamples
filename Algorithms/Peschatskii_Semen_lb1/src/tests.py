from UnrolledLinkedList import UnrolledLinkedList
def test_init():
    subject = UnrolledLinkedList()
    assert subject.head.data == []
    assert subject.nodeSize == 16

    subject = UnrolledLinkedList([1, 10, 11])
    assert subject.head.data == [1,10,11]
    assert subject.nodeSize == 16

    subject = UnrolledLinkedList([1,10,11], 8)
    assert subject.head.data == [1,10,11]
    assert subject.nodeSize == 8

def test_push():
    subject=UnrolledLinkedList()
    subject.push(1)
    assert subject.head.data==[1]

    subject.push(10)
    assert subject.head.data==[1, 10]

def test_rebalance():
    subject = UnrolledLinkedList([1,2,3,4,5,6,7,8,9,10,11,12,13], 5)
    assert subject.head.data==[0, 1, 2, 3, 4]
    assert subject.head.next.data==[5,6,7,8,9]
    assert subject.head.next.next.data==[10,11,12]

def test_valSearch():
    subject = UnrolledLinkedList([1,2,3,4,5,6,7,8,9,10,11,12,13])
    assert subject.valSearch(3) == (0, 4)
    assert subject.valSearch(9) == (1, 4)
    assert subject.valSearch(12) == (2, 2)

def test_IndexRemove():
    subject = UnrolledLinkedList([1, 10, 11, 100, 101])
    subject.IndexRemove(0, 4)
    assert subject.head.data==[1,10,11,100]

    subject.IndexRemove(0, 0)
    assert subject.head.data==[10, 11, 100]

    subject.IndexRemove(0, 1)
    assert subject.head.data==[10,100]

def test_IndexSearch():
    subject = UnrolledLinkedList([1, 10, 11, 101, 110, 111], 2)
    assert subject.IndexSearch(0, 1)==10
    assert subject.IndexSearch(1, 0)==11
    assert subject.IndexSearch(2, 0)==110

def test_valRemove():
    subject = UnrolledLinkedList([1, 10, 11, 100, 101, 110, 111])
    subject.valRemove(1)
    assert subject.head.data==[10, 11, 100, 101, 110, 111]
    subject.valRemove(101)
    assert  subject.head.data==[10, 11, 100, 110, 111]
    subject.valRemove(111)
    assert  subject.head.data==[10,11,100,110]

def test_insert():
    subject = UnrolledLinkedList([1,2,3])
    subject.insert(0, 8)
    assert subject.head.data==[8,1,2,3]
    subject.insert(2, 7)
    assert subject.head.data==[8,1,7,2,3]
    subject.insert(5)
    assert subject.head.data==[8,1,7,2,3,5]