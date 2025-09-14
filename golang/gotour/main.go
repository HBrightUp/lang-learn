package main

import (
	"fmt"
	"math"
	"math/rand"
)

const (
	Big   = 1 << 100
	Small = Big >> 99
)

var (
	MAX_UINT64 uint64 = 1<<64 - 1
)

func add(x, y int) int {
	return x + y
}

func swap(x, y string) (string, string) {
	return y, x
}

func split(sum int) (x, y int) {
	x = sum * 4 / 9
	y = sum - x
	return
}

var c, python, java = true, true, "java"

func needInt(x int) int { return x*10 + 1 }
func needFloat(x float64) float64 {
	return x * 0.1
}

func printSlice(s []int) {
	fmt.Printf("len=%d cap=%d %v\n", len(s), cap(s), s)
}

type Vertex struct {
	Lat, Long float64
}

func compute(fn func(float64, float64) float64) float64 {
	return fn(3, 4)
}

func adder() func(int) int {
	sum := 0
	return func(x int) int {
		sum += x
		return sum
	}
}

func main() {
	fmt.Println("Start go tour,")

	fmt.Println("The number you like best is ", rand.Intn(10))
	fmt.Printf("Now, you have %g questions.", math.Sqrt(7))
	fmt.Println(math.Pi)
	fmt.Println(add(10, 5))

	a, b := swap("hello", "world")
	fmt.Println(a, b)

	fmt.Println(split(17))

	i, j := 1, 2
	fmt.Println(i, j, c, python, java)
	fmt.Printf("type: %T, value: %v\n", MAX_UINT64, MAX_UINT64)

	fmt.Println(needInt(Small))
	fmt.Println(needFloat(Small))
	fmt.Println(needFloat(float64(Big)))

	sum := 1
	for sum < 1000 {
		sum += sum
	}
	fmt.Println(sum) //	1024

	primes := []int{2, 3, 5, 7, 11, 13}
	var s []int = primes[1:4]
	fmt.Println(s)

	q := []int{2, 3, 5, 7, 11, 13}
	q[1] = 100
	fmt.Println(q)

	s = []int{2, 3, 5, 7, 11, 13}
	printSlice(s)

	// 截取切片使其长度为 0
	s = s[:0]
	printSlice(s)
	if s == nil {
		fmt.Println("nil slice") // cap = 6, so it's not empty slice.
	}

	// 扩展其长度
	s = s[:4]
	printSlice(s)

	// 舍弃前两个值
	s = s[2:]
	printSlice(s)

	s1 := make([]int, 2, 10)
	fmt.Printf("len: %v, cap: %v\n", len(s1), cap(s1))

	s1 = append(s1, 10)
	s1 = append(s1, 20)
	fmt.Println(s1)

	var pow = []int{1, 2, 4, 8, 16, 32, 64, 128}
	for i, v := range pow {
		fmt.Printf("2**%d = %d\n", i, v)
	}

	pow = make([]int, 10)
	for i := range pow {
		pow[i] = 1 << uint(i)
	}

	for _, value := range pow {
		fmt.Printf("%d\n", value)
	}

	m := make(map[string]Vertex)
	m["Bell labs"] = Vertex{
		29.23244, -4443.43234,
	}
	fmt.Println(m["Bell labs"])
	elem, ok := m["Bell labs"]
	if ok {
		fmt.Println("delete Bell labs: ", elem)

		delete(m, "Bell labs")
		fmt.Println("len: ", len(m)) // 0
	}

	fmt.Println(compute(math.Pow)) //	81

	pos, neg := adder(), adder()
	for i := 0; i < 10; i++ {
		fmt.Println(
			pos(i),
			neg(-2*i),
		)
	}

	f := fibonacci()
	for i := 0; i < 10; i++ {
		fmt.Println(f())
	}
}

func fibonacci() func() int {
	x := 0
	index := 0
	var prev1, prev2 int

	return func() int {
		if index == 0 {
			index++
			x = 0
			prev2 = x
			return x
		}

		if index == 1 {
			x = 1
			index++
			prev1 = x
			return x
		}

		x = prev1 + prev2
		prev2 = prev1
		prev1 = x

		return x
	}

}
