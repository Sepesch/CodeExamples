from UnrolledLinkedList import UnrolledLinkedList
from CalculateNodeSize import calculate_optimal_node_size
def check(arr1, arr2, nSize=None):
    if nSize==None:
        nSize=calculate_optimal_node_size(len(arr1))
    List=UnrolledLinkedList(n_size=nSize)
    for i in arr1:
        List.push(i)
        List.showList()
    print()
    for i in arr2:
        List.valRemove(i)
        List.showList()
check([i for i in range(10000)], [1,2,20,25,40,100,30,15,3,0,2,5], calculate_optimal_node_size(10000))
