
use std::{
    ptr::{
        NonNull,
    },
    pin:: {
        Pin,
    },
    marker::{
        PhantomPinned,
    },
};

//use crate::include::base::*;

#[derive(Debug)]
pub struct Unmoveable {
    pub year: u32,
    pub data: String,
    pub slice: NonNull<String>,
    pub _pin: PhantomPinned,
}

impl Unmoveable {
    pub fn new(data: String, year: u32) -> Pin<Box<Self>> {
        let res = Unmoveable {
            year,
            data,
            slice: NonNull::dangling(),
            _pin: PhantomPinned,
        };

        let mut boxed = Box::pin(res);
        let slice = NonNull::from(&boxed.data);

        unsafe {
            let mut_ref: Pin<&mut Self> = Pin::as_mut(&mut boxed);
            Pin::get_unchecked_mut(mut_ref).slice = slice;
        }

        boxed
    }
}


pub fn stdpin() {

    //create a self-ref struct with std::pin
    {
        let unmoved = Unmoveable::new("Hello".to_string(), 18);
        //println!("unmoved: {:?}", unmoved);

        let still_moved =  unmoved;

        assert_eq!(still_moved.slice, NonNull::from(&still_moved.data));
        //println!("still_moved: {:?}", still_moved);

        //打印信息输出:
        /*
                unmoved: Unmoveable { year: 18, data: "Hello", slice: 0x57133eea2ae0, _pin: PhantomPinned }
            still_moved: Unmoveable { year: 18, data: "Hello", slice: 0x57133eea2ae0, _pin: PhantomPinned }
         */

        //Conclusion: Although unmoved  moved still_moved, but the address(pointer) of "Hello" is not change; 
    }

}