if __name__ == "__main__":
    # PT_0 is the original pt from the skinny paper
    lanes = [9, 15, 8, 13, 10, 14, 12, 11, 0, 1, 2, 3, 4, 5, 6, 7]

    #
    pt_i = [[0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]]

    iterations = 16

    # for every PT_i
    for i in range(1, iterations):
        pt_i.append([])
        for j in range(16):
            pt_i[i].append(pt_i[i - 1][lanes[j]])

    for i in range(iterations):
        if i % 2 != 0 or i == 0: continue
        print(f'uint8_t pt_{i}[] = {{', end="")
        for n in pt_i[i]:
            print(f"{n}, ", end="")
        print("};")
