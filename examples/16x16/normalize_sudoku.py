#!/usr/bin/env python3
import csv
import sys

def convert_sudoku_csv(file_path):
    matrix = []

    with open(file_path, 'r') as csvfile:
        reader = csv.reader(csvfile)
        for row in reader:
            converted_row = [int(cell, 16) + 1 if cell != 'x' else 0 for cell in row]
            matrix.append(converted_row)

    with open(file_path, 'w', newline='') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerows(matrix)

def main():
    if len(sys.argv) != 2:
        print("Usage: ./normalize_sudoku <filename>")
        sys.exit(1)

    input_filename = sys.argv[1]

    convert_sudoku_csv(input_filename)
    print(f"Sudoku in {input_filename} has been normalized.")

if __name__ == "__main__":
    main()

