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
        [0x1, 0x0, 0x3, 0x2, 0x5, 0x4, 0x7, 0x6, 0x9, 0x8, 0xb, 0xa, 0xd, 0xc, 0xf, 0xe],  # PT 0 (nibble-swapped state)
        [0xE, 0x8, 0xC, 0x9, 0xF, 0xB, 0xA, 0xD, 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7]  # PT 1 lanes of swapped nibbles (e.g., 0xF is at index 0xE in PT0)
        # ...
    ]
    flipped_start = flip(pt_i[0])

    # calculate all PT's
    for i in range(1, 16):
        pt_i.append([])
        for j in range(16):
            pt_i[i].append(pt_i[i - 1][pt_i[1][j]])

    # because of how we laid out the state in memory, flip every 2 cells
    lanes = pt_i[iteration]

    # print the specified iteration
    print(f"PT_{iteration}\n", pt_i[iteration])
    for (i, n) in enumerate(lanes):
        print(f'y[{hex(i)}] = x[{hex(flipped_start.index(n))}];', end="")
        if i == len(lanes) - 1:
            print("   }")
        else:
            print("   \\")
