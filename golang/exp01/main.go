package main 

import (
	"fmt"
	"math"
	"unsafe"
)

var (
	x int
	y byte
)

const MAX_LEN int = 100;
const MAX_WIDTH = 10;

const (
	a = iota

)
func main() {

		fmt.Println("start start end");

		//var name = "bright";
		
		fmt.Println(math.Exp2(10));

		fmt.Print(3,45,57,"name", 3);

		vaild := false;

		var b,c int = 1,2;
		fmt.Print(b,c ,vaild);

		x = 100;
		y = 'b';

		if  x > int(y) {
			fmt.Print(x);
		} else {
			fmt.Print(y);
		}

		fmt.Print(MAX_LEN * MAX_WIDTH);

		dest := "helloworld";
		
		fmt.Print(unsafe.Sizeof(dest));


		var ptr  = & x;
		fmt.Println(*ptr);
		fmt.Println(ptr);

		var x interface{}



		switch x.(type) {
			case int: {
				fmt.Print("int type");
			}
			case float64:{
				fmt.Print("flaot type");
			}
			case bool: {
				fmt.Print("pointer type");
			}
			case nil: {
				fmt.Print("nil type");
			}
			default: {
				fmt.Print("defalut type");
			}

			
		}

		var sum int;
		for i := 0; i < 100; i++ {
			sum += i;
		}

		fmt.Print(sum);

		var x1 = 43;
		var x2 = 54;

		fmt.Print(finde_max(x1,x2));
}

func finde_max(x1,x2 int) int {
	if x1 > x2  {
		return x1;
	}
	return x2;
}