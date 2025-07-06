class Node:
    def __init__(self, n=None):
        self.data=[]
        self.next=n
    def add(self, data):
        self.data.append(data)

    def __str__(self):
        return ' '.join(map(str, self.data))