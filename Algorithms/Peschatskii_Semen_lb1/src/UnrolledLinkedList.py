from Node import Node
class UnrolledLinkedList:
    def __init__(self, arr=None, n_size=16):
        self.head=Node()
        self.nodeSize=n_size
        self.last=self.head
        if arr is not None:
            for i in arr:
                self.push(i)

    def push(self, item):
        self.last.add(item)
        self.rebalance()

    def rebalance(self):
        currNode=self.head
        while currNode:
            if len(currNode.data)>self.nodeSize:
                newNode=Node()
                currNode.next=newNode
                self.last=currNode.next
            if currNode.next:
                while len(currNode.data) > self.nodeSize:
                    currNode.next.data.insert(0, currNode.data.pop())
                while len(currNode.data) < self.nodeSize:
                    if currNode.next.data:
                        currNode.data.insert(self.nodeSize-1, currNode.next.data.pop(0))
                    else:
                        break

            if len(self.last.data)==0 and self.last != self.head:
                prev=self.head
                while prev.next!=self.last:
                    prev=prev.next
                prev.next=None
                self.last=prev
            currNode=currNode.next

    def valSearch(self, value):
        currNode=self.head
        i=0
        while currNode.next:
            for j in currNode.data:
                if j == value:
                    return (i, j)
            i+=1
            currNode=currNode.next
        return (-1, -1)

    def IndexRemove(self, nIndex, pIndex):
        currNode = self.head
        for i in range(nIndex):
            if currNode is None or currNode.next is None:
                return
            currNode=currNode.next
        if pIndex<=self.nodeSize:
            return currNode.data.pop(pIndex)
        self.rebalance()

    def IndexSearch(self, nIndex, pIndex):
        currNode = self.head
        for i in range(nIndex):
            if currNode is None or currNode.next is None:
                return
            currNode=currNode.next
        if pIndex<=self.nodeSize:
            return currNode.data[pIndex]
        self.rebalance()
    def valRemove(self, value):
        currNode=self.head
        i=0
        while currNode:
            if i == 0:
                for j in range(len(currNode.data)):
                    if currNode.data[j] == value:
                        currNode.data.pop(j)
                        i=+1
                        break
                currNode=currNode.next
            else:
                break
        self.rebalance()
    def insert(self, index, val):
        currNode = self.head
        while currNode is not None:
            if index < len(currNode.data):
                currNode.data.insert(index, val)
                break
            index-=len(currNode.data)
            currNode=currNode.next
        self.rebalance()
    def showList(self):
        currNode=self.head
        i=0
        while currNode is not None:
            print(f'Node {i}:', end=' ')
            print(*currNode.data)
            i+=1
            currNode=currNode.next