package main

import (
	"fmt"
	"io"
	"math"
	"math/rand"
	"strings"
	"time"
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

	v := Vertex{3, 4}
	v.Scale(10)
	fmt.Println("scaled: ", v.Abs())

	var i1 I
	i1 = &T{"hello"}
	describle(i1)

	var t *T
	i2 := t
	describle(i2) //	(<nil>, *main.T)

	var i3 I
	describle(i3) //	(<nil>, <nil>)

	var i4 interface{}
	i4 = 42
	describle2(i4) //	(42, int)

	i4 = "bright"
	describle2(i4) //	(bright, string)

	s2, ok := i4.(string)
	fmt.Println(s2, ok)

	do(31)
	do("hello")
	do(true)

	a1 := Person{"Bright", 20}
	z1 := Person{"hml", 18}
	fmt.Println(a1, z1)

	hosts := map[string]IPAddr{
		"loopback":  {127, 0, 0, 1},
		"googleDNS": {8, 8, 8, 8},
	}

	for name, ip := range hosts {
		fmt.Printf("%v : %v\n", name, ip)
	}

	if err := run(); err != nil {
		fmt.Println(err)
	}

	fmt.Println(Sqrt(2))
	fmt.Println(Sqrt(-2))

	r := strings.NewReader("Hello, Reader!")
	b1 := make([]byte, 8)

	for {
		n, err := r.Read(b1)
		fmt.Printf("n = %v err = %v b = %v\n", n, err, b1)
		fmt.Printf("b[:n] = %q\n", b1[:n])
		if err == io.EOF {
			break
		}
	}
}

type ErrNeagativeSqrt struct {
	Value float64
}

func (e ErrNeagativeSqrt) Error() string {
	return fmt.Sprintf("cannot calculate square root of negative number: %v", e.Value)
}

func Sqrt(x float64) (float64, error) {

	if x < 0 {
		return 0, &ErrNeagativeSqrt{x}
	}

	return math.Sqrt(x), nil
}

type MyError struct {
	When time.Time
	what string
}

func (e MyError) Error() string {
	return fmt.Sprintf("at %v: %s", e.When, e.what)
}

func run() error {
	return &MyError{
		time.Now(),
		"destory",
	}
}

type IPAddr [4]byte

func (ip *IPAddr) String() string {
	return string(ip[:])
}

type Person struct {
	Name string
	Age  int
}

func (p Person) String() string {
	return fmt.Sprintf("%v : %v years", p.Name, p.Age)
}

func do(i interface{}) {
	switch i.(type) {
	case int:
		fmt.Println("int type")
	case string:
		fmt.Println("string type")
	default:
		fmt.Println("unknown type")
	}
}

func describle2(i interface{}) {
	fmt.Printf("(%v, %T)\n", i, i)
}

func describle(i I) {
	fmt.Printf("(%v, %T)\n", i, i)
}

type I interface {
	M()
}

type T struct {
	S string
}

func (t *T) M() {
	fmt.Println(t.S)
}

func (v Vertex) Abs() float64 {
	return math.Sqrt(v.Lat*v.Lat + v.Long*v.Long)
}

func (v *Vertex) Scale(f float64) {
	v.Lat = v.Lat * f
	v.Long = v.Long * f
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
