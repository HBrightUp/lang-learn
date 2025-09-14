package main

import (
	"bytes"
	"fmt"
	"math"
	"unsafe"
)

const (
	ARR_LEN = 10
)

type Vertex struct {
	X int
	Y int
}

type shape interface {
	area() float32
}

type circle struct {
	x, y, radius float32
}

type rectangle struct {
	width, height float32
}

func (c circle) area() float32 {
	return math.Pi * c.radius * c.radius
}

func (r rectangle) area() float32 {
	return r.width * r.height
}

func getArea(s shape) float32 {
	return s.area()
}

type functinTyoe func(int, int) // // 声明了一个函数类型

func (f functinTyoe) Serve() {
	fmt.Println("serve2")
}

func serve(int, int) {
	fmt.Println("serve1")
}

func main() {
	fmt.Print("Basic start running ...\n")

	var is_success bool

	if is_success {
		fmt.Println(" success.")
	} else {
		fmt.Println("failed!")
	}

	var arr [10]int

	var i int

	for i = 0; i < ARR_LEN; i++ {
		arr[i] = i
	}

	fmt.Println("len of arr: ", len(arr)) //	10

	b := []byte("Golang")
	c := []byte("Go")
	fmt.Print(bytes.Contains(b, c)) // true

	s2 := "Bright, 上午好"
	println("len of s2, len: ", len(s2))
	m := func(r rune) rune {
		if r == '上' {
			r = '下'
		}

		return r
	}

	fmt.Println(s2)
	fmt.Println(string(bytes.Map(m, []byte(s2))))

	s := struct {
		a byte
		b byte
		c byte
		d int64
	}{0, 0, 0, 0}

	p := unsafe.Pointer(&s)
	up0 := uintptr(p)

	up := up0 + unsafe.Offsetof(s.c)
	p = unsafe.Pointer(up)
	pc := (*byte)(p)
	*pc = 30
	fmt.Println(s)

	var arr2 = [...]float32{23.33, 323.3, 25.3, 58, 323.345}
	v1 := Vertex{2, 4}
	fmt.Println(arr2, v1)

	circle := circle{x: 0, y: 0, radius: 5}
	rectangle := rectangle{width: 10, height: 5}

	fmt.Printf("circle area: %f\n", getArea(circle))
	fmt.Printf("rectangle area: %f\n", getArea(rectangle))

	a := functinTyoe(serve)
	fmt.Println(" init")
	a(1, 2) //	serve1
	fmt.Println(" use")
	a.Serve() //	serve2

	dict := map[string]string{"Red": "#da1377", "orange": "#e95a22"}

	for key, value := range dict {
		fmt.Printf("Key: %s, value: %s\n", key, value)
	}
}
