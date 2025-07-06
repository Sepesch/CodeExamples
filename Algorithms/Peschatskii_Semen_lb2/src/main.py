class Stack:
    def __init__(self):
        self.__arr = []
        self.gallops_cnt = 0
        self.merge_cnt = 0

    def __len__(self):
        return len(self.__arr)

    def top(self):
        if len(self.__arr) == 0:
            return None
        else:
            return self.__arr[-1]

    def push(self, item):
        self.__arr.append(item)
        if len(self.__arr) >= 2:
            self.__merge()

    def pop(self):
        if len(self.__arr) == 0:
            return
        else:
            self.__arr.pop(-1)

    def __merge(self):
        ok = True
        while ok:
            if len(self.__arr) < 2:
                break
            y = self.__arr[-2]
            x = self.__arr[-1]
            if len(self.__arr) > 2:
                z = self.__arr[-3]
                if not (len(z) > len(x) + len(y) and len(y) > len(x)):
                    if len(z) < len(x):
                        self.__arr[-1] = self.merge_arr(self.__arr[-1], y, 3)
                        print(f"Gallops {self.merge_cnt}:", self.gallops_cnt)
                        self.gallops_cnt = 0
                        print(f"Merge {self.merge_cnt}:", *self.__arr[-1])
                        self.merge_cnt += 1
                        self.__arr.pop(-2)
                    else:
                        self.__arr[-1] = self.merge_arr(self.__arr[-1], y, 3)
                        print(f"Gallops {self.merge_cnt}:", self.gallops_cnt)
                        self.gallops_cnt = 0
                        print(f"Merge {self.merge_cnt}:", *self.__arr[-1])
                        self.merge_cnt += 1
                        self.__arr.pop(-2)
                else:
                    ok = False
            else:
                if not (len(y) > len(x)):
                    self.__arr[-1] = self.merge_arr(self.__arr[-1], y, 3)
                    print(f"Gallops {self.merge_cnt}:", self.gallops_cnt)
                    self.gallops_cnt = 0
                    print(f"Merge {self.merge_cnt}:", *self.__arr[-1])
                    self.merge_cnt += 1
                    self.__arr.pop(-2)
                else:
                    ok = False

    def merge_arr(self, arr1, arr2, gallop_start):
        rez = []
        first_cnt = 0
        second_cnt = 0
        first_ind = 0
        second_ind = 0
        while len(rez) < len(arr1) + len(arr2):
            if first_cnt == gallop_start:
                found_ind = self.binary_search(arr1[first_ind:], arr2[second_ind]) + first_ind
                rez.extend(arr1[first_ind:found_ind])
                first_ind = found_ind
                first_cnt = 0
                self.gallops_cnt += 1
            if second_cnt == gallop_start:
                found_ind = self.binary_search(arr2[second_ind:], arr1[first_ind]) + second_ind
                rez.extend(arr2[second_ind:found_ind])
                second_ind = found_ind
                second_cnt = 0
                self.gallops_cnt += 1
            if first_ind == len(arr1):
                rez.extend(arr2[second_ind:])
                break
            if second_ind == len(arr2):
                rez.extend(arr1[first_ind:])
                break
            if abs(arr1[first_ind]) > abs(arr2[second_ind]):
                rez.append(arr1[first_ind])
                first_ind += 1
                first_cnt += 1
                second_cnt = 0
            else:
                rez.append(arr2[second_ind])
                second_ind += 1
                second_cnt += 1
                first_cnt = 0
        return rez

    def binary_search(self, orig_arr, target):
        left, right = 0, len(orig_arr) - 1
        result_index = len(orig_arr)
        while left <= right:
            mid = (left + right) // 2
            mid_val = orig_arr[mid]
            if abs(mid_val) < abs(target):
                result_index = mid
                right = mid - 1
            else:
                left = mid + 1
        return result_index

    def final_merge(self):
        while len(self.__arr) >= 2:
            y = self.__arr[-2]
            x = self.__arr[-1]
            if len(self.__arr) > 2:
                z = self.__arr[-3]
                if len(z) < len(x):
                    self.__arr[-3] = self.merge_arr(self.__arr[-3], y, 3)
                    print(f"Gallops {self.merge_cnt}:", self.gallops_cnt)
                    self.gallops_cnt = 0
                    print(f"Merge {self.merge_cnt}:", *self.__arr[-3])
                    self.merge_cnt += 1
                    self.__arr.pop(-2)
                else:
                    self.__arr[-1] = self.merge_arr(self.__arr[-1], y, 3)
                    print(f"Gallops {self.merge_cnt}:", self.gallops_cnt)
                    self.gallops_cnt = 0
                    print(f"Merge {self.merge_cnt}:", *self.__arr[-1])
                    self.merge_cnt += 1
                    self.__arr.pop(-2)
            else:
                self.__arr[-1] = self.merge_arr(self.__arr[-1], y, 3)
                print(f"Gallops {self.merge_cnt}:", self.gallops_cnt)
                self.gallops_cnt = 0
                print(f"Merge {self.merge_cnt}:", *self.__arr[-1])
                self.merge_cnt += 1
                self.__arr.pop(-2)

def calculate_min_run(n):
    r = 0
    while n >= 16:
        r |= n & 1
        n >>= 1
    return n + r

def insertion_sort(orig_arr):
    for i in range(1, len(orig_arr)):
        key = orig_arr[i]
        j = i - 1
        while j >= 0 and abs(orig_arr[j]) < abs(key):
            orig_arr[j + 1] = orig_arr[j]
            j -= 1
        orig_arr[j + 1] = key
    return orig_arr

def separate_arr(array, min_run):
    runs = [[]]
    for i in range(len(array)):
        if len(runs[-1]) < min_run:
            runs[-1].append(array[i])
            if i == len(array) - 1:
                insertion_sort(runs[-1])
        else:
            ascending_order, descending_order = is_sorted_abs(runs[-1])
            if ascending_order and not descending_order:
                if abs(array[i]) > abs(runs[-1][-1]):
                    runs[-1].append(array[i])
                else:
                    insertion_sort(runs[-1])
                    runs.append([array[i]])
                continue
            if not ascending_order and descending_order:
                if abs(array[i]) < abs(runs[-1][-1]):
                    runs[-1].append(array[i])
                else:
                    insertion_sort(runs[-1])
                    runs.append([array[i]])
                continue
            if not ascending_order and not descending_order:
                insertion_sort(runs[-1])
                runs.append([array[i]])
                continue
            if ascending_order and descending_order:
                if abs(array[i]) == abs(runs[-1][-1]):
                    runs[-1].append(array[i])
                    continue
                insertion_sort(runs[-1])
                runs.append([array[i]])
    return runs

def is_sorted_abs(arr):
    ascending = all(abs(arr[i]) <= abs(arr[i + 1]) for i in range(len(arr) - 1))
    descending = all(abs(arr[i]) >= abs(arr[i + 1]) for i in range(len(arr) - 1))
    return ascending, descending

def tim_sort(orig_arr):
    min_run = calculate_min_run(len(orig_arr))
    runs = separate_arr(orig_arr, min_run)
    for i in range(len(runs)):
        print(f"Part {i}:", *runs[i])
    stack = Stack()
    for i in runs:
        stack.push(i)
    stack.final_merge()
    return stack.top()

n = int(input())
inp = [int(x) for x in input().split()]
print("Answer:", *tim_sort(inp))