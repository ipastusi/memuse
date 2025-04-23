package main

import (
	"slices"
	"testing"
)

func Test_parseAllocationConfig(t *testing.T) {
	var emptyStages []Stage

	data := []struct {
		name   string
		input  string
		stages []Stage
		ok     bool
	}{
		{"one", "1000:20", []Stage{{1000, 20}}, true},
		{"two", "1:2|3:4", []Stage{{1, 2}, {3, 4}}, true},
		{"three", "1:2|3:4|500:60", []Stage{{1, 2}, {3, 4}, {500, 60}}, true},
		{"nosleep", "1:0|4:0", []Stage{{1, 0}, {4, 0}}, true},
		{"invalid", "", emptyStages, false},
		{"invalid", " ", emptyStages, false},
		{"invalid", "1", emptyStages, false},
		{"invalid", "a", emptyStages, false},
		{"invalid", "a:a", emptyStages, false},
		{"invalid", "a:1", emptyStages, false},
		{"invalid", "a1:1", emptyStages, false},
		{"invalid", "1:1a", emptyStages, false},
		{"invalid", "1:a", emptyStages, false},
		{"invalid", "1:-", emptyStages, false},
		{"invalid", "1:2:3", emptyStages, false},
		{"invalid", "0:0", emptyStages, false},
		{"invalid", "1:2:", emptyStages, false},
		{"invalid", "1:2::", emptyStages, false},
		{"invalid", "1:-2", emptyStages, false},
		{"invalid", "-1:2", emptyStages, false},
		{"invalid", "1.0:2.0", emptyStages, false},
		{"invalid", ":", emptyStages, false},
		{"invalid", "1:", emptyStages, false},
		{"invalid", ":2", emptyStages, false},
		{"invalid", "|", emptyStages, false},
		{"invalid", "1:2|", emptyStages, false},
		{"invalid", "1:2||", emptyStages, false},
		{"invalid", "|1:2", emptyStages, false},
		{"invalid", "|1:2|", emptyStages, false},
	}

	for _, d := range data {
		t.Run(d.name, func(t *testing.T) {
			result, ok := parseAllocationConfig(d.input)
			if ok != d.ok {
				t.Error("incorrect stages, expected:", d.ok, "actual:", ok)
			}
			if !slices.Equal(result, d.stages) {
				t.Error("incorrect stages, expected:", d.stages, "actual:", result)
			}
		})
	}
}
