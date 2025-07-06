from typing import Union
import time
import random
class Node:
    def __init__(self, val, left=None, right=None):
        self.val = val
        self.left: Union[Node, None] = left
        self.right: Union[Node, None] = right
        self.height: int = 1

def check(root: Node) -> bool:
    def is_balanced(node):
        if not node:
            return True, 0
        left_balanced, left_height = is_balanced(node.left)
        right_balanced, right_height = is_balanced(node.right)
        if not left_balanced or not right_balanced:
            return False, 0
        if abs(left_height - right_height) > 1:
            return False, 0
        return True, max(left_height, right_height) + 1
    balanced, _ = is_balanced(root)
    return balanced
def get_tree_height(node):
        if not node:
            return 0
        return 1 + max(get_tree_height(node.left), get_tree_height(node.right))
def get_max(node):
    curr=node
    while curr.right:
        curr=curr.right
    return curr.val
def get_min(node):
    curr=node
    while curr.left:
        curr=curr.left
    return curr.val
def diff(root: Node) -> int:
    def in_order(diffs,node):
        if node.left is not None:
            diffs.append(abs(node.val-node.left.val))
            in_order(diffs,node.left)
        if node.right is not None:
            diffs.append(abs(node.val - node.right.val))
            in_order(diffs, node.right)
    diffs = []
    in_order(diffs,root)
    return min(diffs)
def get_height(node: Node) -> int:
    if not node:
        return 0
    return node.height

def get_balance(node: Node) -> int:
    if not node:
        return 0
    return get_height(node.left) - get_height(node.right)

def right_rotate(y: Node) -> Node:
    x = y.left
    T2 = x.right
    x.right = y
    y.left = T2
    y.height = max(get_height(y.left), get_height(y.right)) + 1
    x.height = max(get_height(x.left), get_height(x.right)) + 1
    return x

def left_rotate(x: Node) -> Node:
    y = x.right
    T2 = y.left
    y.left = x
    x.right = T2
    x.height = max(get_height(x.left), get_height(x.right)) + 1
    y.height = max(get_height(y.left), get_height(y.right)) + 1
    return y

def insert(val, node: Node) -> Node:
    if not node:
        return Node(val)
    if val < node.val:
        node.left = insert(val, node.left)
    elif val > node.val:
        node.right = insert(val, node.right)
    else:
        return node
    node.height = 1 + max(get_height(node.left), get_height(node.right))
    balance = get_balance(node)
    if balance > 1 and val < node.left.val:
        return right_rotate(node)
    if balance < -1 and val > node.right.val:
        return left_rotate(node)
    if balance > 1 and val > node.left.val:
        node.left = left_rotate(node.left)
        return right_rotate(node)
    if balance < -1 and val < node.right.val:
        node.right = right_rotate(node.right)
        return left_rotate(node)
    return node

def delete(val, node: Node) -> Node:
    if not node:
        return node
    if val < node.val:
        node.left = delete(val, node.left)
    elif val > node.val:
        node.right = delete(val, node.right)
    else:
        if node.left is None:
            return node.right
        elif node.right is None:
            return node.left
        else:
            temp = node.right
            while temp.left:
                temp = temp.left
            node.val = temp.val
            node.right = delete(temp.val, node.right)
    node.height = 1 + max(get_height(node.left), get_height(node.right))
    balance = get_balance(node)
    if balance > 1 and get_balance(node.left) >= 0:
        return right_rotate(node)
    if balance > 1 and get_balance(node.left) < 0:
        node.left = left_rotate(node.left)
        return right_rotate(node)
    if balance < -1 and get_balance(node.right) <= 0:
        return left_rotate(node)
    if balance < -1 and get_balance(node.right) > 0:
        node.right = right_rotate(node.right)
        return left_rotate(node)

    return node
def delete_max(node: Node):
    target = get_max(node)
    return delete(target, node)
def delete_min(node: Node):
    target = get_min(node)
    return delete(target, node)
def showTree(node):
    def build_tree_string(node, curr_index):
        if node is None:
            return [], 0, 0, 0

        line1 = []
        line2 = []
        node_repr = str(node.val)

        new_root_width = gap_size = len(node_repr)

        l_box, l_box_width, l_root_start, l_root_end = build_tree_string(node.left, 2 * curr_index + 1)
        r_box, r_box_width, r_root_start, r_root_end = build_tree_string(node.right, 2 * curr_index + 2)

        if l_box_width > 0:
            l_root = (l_root_start + l_root_end) // 2 + 1
            line1.append(' ' * (l_root + 1))
            line1.append('_' * (l_box_width - l_root))
            line2.append(' ' * l_root + '/')
            line2.append(' ' * (l_box_width - l_root))
            new_root_start = l_box_width + 1
            gap_size += 1
        else:
            new_root_start = 0

        line1.append(node_repr)
        line2.append(' ' * new_root_width)

        if r_box_width > 0:
            r_root = (r_root_start + r_root_end) // 2
            line1.append('_' * r_root)
            line1.append(' ' * (r_box_width - r_root + 1))
            line2.append(' ' * r_root + '\\')
            line2.append(' ' * (r_box_width - r_root))
            gap_size += 1
        new_root_end = new_root_start + new_root_width - 1

        gap = ' ' * gap_size
        new_box = [''.join(line1), ''.join(line2)]
        for i in range(max(len(l_box), len(r_box))):
            l_line = l_box[i] if i < len(l_box) else ' ' * l_box_width
            r_line = r_box[i] if i < len(r_box) else ' ' * r_box_width
            new_box.append(l_line + gap + r_line)

        return new_box, len(new_box[0]), new_root_start, new_root_end

    tree_height = get_tree_height(node)
    if tree_height == 0:
        return

    tree_string, _, _, _ = build_tree_string(node, 0)
    for line in tree_string:
        print(line)

n=10
c=1
arr=[]
for i in range(n+1):
    arr.append(c)
    c+=1
root = Node(arr[0])
for i in range(1, n):
    root=insert(arr[i], root)
print("Исходное дерево:")
showTree(root)

delete(10, root)
print("\nДерево после удаления узла 10")
showTree(root)

delete_min(root)
print("\nДерево после удаления минимального узла")
showTree(root)

delete_max(root)
print("\nДерево после удаления максимального узла")
showTree(root)

insert(23, root)
print("\nДерево после добавления узла 23")
showTree(root)