def get_height():
    while True:
        try:
            height = int(input("Height: "))

            if height < 1 or height > 8:
                raise ValueError()

            return height
        except ValueError:
            pass


height = get_height()

for i in range(height):
    for j in range(height + i + 1):
        if j + 1 < height - i:
            print(" ", end="")
        else:
            print("#", end="")

        if j == height - 1:
            for k in range(2):
                print(" ", end="")
    print()
