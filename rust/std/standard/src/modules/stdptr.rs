use std::{
    ptr::{
        NonNull,
    },
};


pub fn stdptr() {

    ptr_nonnull_danglig();
    ptr_nonnull_as_ptr();

    {
        // declare Dangling Pointer 
        let _p1 = NonNull::<u32>::dangling();

        let mut x = 100u32;
        let mut p2 = unsafe{ NonNull::new_unchecked(&mut x as *mut _)};
        let p3 = NonNull::new(&mut x as *mut _).unwrap();
        assert_eq!(p2, p3);

        //p1:0x4, p2:0x7fff0b4561c4， p3:0x7fff0b4561c4
        //p1: 0x4, It seems to be a fixed value ?
        //println!("p1:{:?}, p2:{:?}, p3:{:?}", p1, p2, p3);

        let x2 = unsafe { *p2.as_ptr()};
        assert_eq!(x2, 100u32);

        unsafe {*p2.as_ptr() += 2u32};
        let x3 = unsafe {*p3.as_ptr() };
        assert_eq!( x3, 102u32);

        let x_ref = unsafe { p2.as_mut()};
        *x_ref *= 2u32;
        let x4 = unsafe {*p3.as_ptr()};
        assert_eq!(x4, 204u32);

        // NonNULL - > i8 ptr
        let p4 = p2.cast::<i8>();
        assert_eq!(p2, p3);

        //  u32 -> i8 : The data overflowed, but the program did not report an error.
        let x5 = unsafe {*p4.as_ptr()};
        assert_eq!(x5, -52);
    }

    // NonNull<slice>
    {
        let mut a1 = [5, 6, 7];
        let p1 = NonNull::new(a1.as_mut_ptr()).unwrap();
        let s = NonNull::slice_from_raw_parts(p1, a1.len());
        assert_eq!(unsafe {s.as_ref()[2]}, 7);

        // Declare a NonNull ptr to slice which length is 3;
        let _p2 : NonNull::<[i8]> = NonNull::slice_from_raw_parts(NonNull::dangling(), 3);
        
        let a2 = &mut [1, 2, 4];
        let p3 = NonNull::slice_from_raw_parts(NonNull::new(a2.as_mut_ptr()).unwrap(), a2.len());
        unsafe {
            assert_eq!(p3.get_unchecked_mut(1).as_ptr(), a2.as_mut_ptr().add(1));
        }

    }

}

//NonNull::dangling()
fn ptr_nonnull_danglig() {
    let _ptr = NonNull::<u32>::dangling();

    // It is just defined, not initialized yet, and its contents cannot be accessed.
}


// NonNull::as_ptr()
fn ptr_nonnull_as_ptr() {
    let mut x = 10u32;
    let ptr = NonNull::new(&mut x).unwrap();

    let x_value = unsafe { *ptr.as_ptr() };
    assert_eq!(x, x_value);

    unsafe { *ptr.as_ptr() += 2 };
    let x_value = unsafe { *ptr.as_ref() };
    assert_eq!(x_value, 12 );
}


#[cfg(test)]
mod test {
    use super::*;

    // NonNull ->  
    #[test]
    fn test_nonnull() {
        ptr_nonnull_as_ptr();
    }
}