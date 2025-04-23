package main

import (
	"testing"
)

func Test_getByteSize(t *testing.T) {
	data := []struct {
		name     string
		size     int
		isMb     bool
		expected int
	}{
		{"mib", 1024, false, 1024 * 1024 * 1024},
		{"mb", 1024, true, 1000 * 1000 * 1024},
	}

	for _, d := range data {
		t.Run(d.name, func(t *testing.T) {
			result := getByteSize(d.size, d.isMb)
			if result != d.expected {
				t.Error("incorrect result, expected:", d.expected, "actual:", result)
			}
		})
	}
}
