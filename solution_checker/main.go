package main

import (
	"encoding/csv"
	"fmt"
	"os"
)

func compareCSVFiles(file1, file2 string) (bool, error) {
	// Read data from the first CSV file
	data1, err := readCSVFile(file1)
	if err != nil {
		return false, err
	}

	// Read data from the second CSV file
	data2, err := readCSVFile(file2)
	if err != nil {
		return false, err
	}

	// Compare the content of the two CSV files
	return compareCSVData(data1, data2), nil
}

func readCSVFile(filename string) ([][]string, error) {
	// Open the CSV file
	file, err := os.Open(filename)
	if err != nil {
		return nil, err
	}
	defer file.Close()

	// Parse the CSV data
	reader := csv.NewReader(file)
	data, err := reader.ReadAll()
	if err != nil {
		return nil, err
	}

	return data, nil
}

func compareCSVData(data1, data2 [][]string) bool {
	// Check if the number of rows is the same
	if len(data1) != len(data2) {
		return false
	}

	// Check if the content of each row is the same
	for i, row1 := range data1 {
		row2 := data2[i]
		if len(row1) != len(row2) {
			return false
		}
		for j, value1 := range row1 {
			value2 := row2[j]
			if value1 != value2 {
				return false
			}
		}
	}

	return true
}

func main() {
	if len(os.Args) < 3 {
		fmt.Println("Usage: ./check file1.csv file2.csv")
		os.Exit(1)
	}

	file1 := os.Args[1]
	file2 := os.Args[2]

	match, err := compareCSVFiles(file1, file2)
	if err != nil {
		fmt.Println("Error:", err)
		os.Exit(1)
	}

	if match {
		fmt.Println("Matching")
		os.Exit(0)
	} else {
		fmt.Println("Not matching")
		os.Exit(1)
	}
}

