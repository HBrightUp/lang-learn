use std::{
    mem::{
        self,
    },
};


pub fn stdmem() {

    mem_take();
    mem_swap();
    mem_size_of();
    mem_replace();
    mem_alian_of();
}

// take
fn mem_take() {

    let mut v: Vec<i32> = vec![1, 2];
    let old_v = mem::take(&mut v);
    assert_eq!(vec![1, 2], old_v);
    assert!(v.is_empty());
}

// swap
fn mem_swap() {
    let mut x = 5;
    let mut y = 42;

    mem::swap(&mut x, &mut y);
    assert_eq!(42, x);
    assert_eq!(5, y);
}

// size_of && size_of_val
fn mem_size_of() {
    #[repr(C)]
    struct FieldStruct {
        first: u8,
        second: u16,
        third: u8
    }
    //add  repr(C)，it's size is 6, otherwise is 4 ;
    // For C language, you need to manually align bytes, but not need for rust;
    assert_eq!(6, mem::size_of::<FieldStruct>());

    #[repr(C)]
    struct FieldStructOptimized {
        first: u8,
        third: u8,
        second: u16
    }

    assert_eq!(4, mem::size_of::<FieldStructOptimized>());

    // get size of array
    let x: [u8; 13] = [0; 13];
    let y: &[u8] = &x;
    assert_eq!(13, mem::size_of_val(y));
}

// replace
fn mem_replace() {
    let mut v = vec![1, 2];
    let old_v = std::mem::replace(&mut v, vec![3, 4, 5]);
    assert_eq!(v, vec![3, 4, 5]);
    assert_eq!(old_v, vec![1, 2]);
}

// align_of && align_of_val
fn mem_alian_of() {
    assert_eq!(4, mem::align_of::<i32>());
    assert_eq!(4, mem::align_of_val(&5i32));
}

#[cfg(test)]
mod test {
    use super::*;

    #[test] 
    fn test_mem() {
        mem_alian_of();
        mem_replace();
        mem_size_of();
        mem_swap();
        mem_take();
    }
}