#!/usr/bin/env python3
import csv
import sys

def is_valid_sudoku(file_path):
    matrix = []

    # Read the Sudoku from the CSV file
    with open(file_path, 'r') as csvfile:
        reader = csv.reader(csvfile)
        for row in reader:
            matrix.append([int(cell) for cell in row])

    # Check if any cell is zero
    if any(0 in row for row in matrix):
        return False

    # Check rows, columns, and 3x3 subgrids
    for i in range(9):
        if not is_valid_set(matrix[i]) or not is_valid_set([row[i] for row in matrix]) or not is_valid_subgrid(matrix, i // 3 * 3, i % 3 * 3):
            return False

    return True

def is_valid_set(nums):
    seen = set()
    for num in nums:
        if num in seen:
            return False
        seen.add(num)
    return True

def is_valid_subgrid(matrix, start_row, start_col):
    seen = set()
    for i in range(start_row, start_row + 3):
        for j in range(start_col, start_col + 3):
            num = matrix[i][j]
            if num in seen:
                return False
            seen.add(num)
    return True

def main():
    if len(sys.argv) != 2:
        print("Usage: ./validate_sudoku.py <filename>")
        sys.exit(1)

    input_filename = sys.argv[1]

    if is_valid_sudoku(input_filename):
        print(f"{input_filename} VALID")
    else:
        print(f"{input_filename} NOT VALID")

if __name__ == "__main__":
    main()

