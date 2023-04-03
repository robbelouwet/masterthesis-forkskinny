def flip(arr):
    res = []

    i = 0
    while i < len(arr):
        res.append(arr[i + 1])
        res.append(arr[i])
        i += 2

    return res


if __name__ == "__main__":

    # choose the iteration you want to print
    iteration = 2

    pt_i = [
        [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15],  # PT 0
        [9, 15, 8, 13, 10, 14, 12, 11, 0, 1, 2, 3, 4, 5, 6, 7]   # PT 1
        # ...
    ]
    flipped_start = flip(pt_i[0])

    # calculate all PT's
    for i in range(1, 16):
        pt_i.append([])
        for j in range(16):
            pt_i[i].append(pt_i[i - 1][pt_i[1][j]])

    # because of how we laid out the state in memory, flip every 2 cells
    lanes = flip(pt_i[iteration])

    # print the specified iteration
    print(f"PT_{iteration}\n", pt_i[iteration])
    for (i, n) in enumerate(lanes):
        print(f'y[{hex(i)}] = x[{hex(flipped_start.index(n))}];', end="")
        if i == len(lanes) - 1:
            print("   }")
        else:
            print("   \\")
