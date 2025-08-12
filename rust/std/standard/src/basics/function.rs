


#[cfg(test)]
mod test {
    
    #[test]
    fn test_Fn() {
        let mut  x = 5;

        //Fn: The value of the outer variable cannot be modified in the closure
        let  add_x = |y| {
            //x += 1;       // error
            x + y
        };

        assert_eq!(add_x(3), 8);
    }
    #[test] 
    fn test_FnMut() {

        let mut x = 5;
        let mut add_x = |y| {
            x += 1;
            x + y
        };

        assert_eq!(add_x(4), 10);
        assert_eq!(x, 6);

    }

    #[test]
    fn test_FnOnce() {
        let x = String::from("world");
        let take_x = || {
            println!("Take ownership.");
            let _ = std::mem::drop(x);
        };

        take_x();
        //take_x();     // error
    }
}