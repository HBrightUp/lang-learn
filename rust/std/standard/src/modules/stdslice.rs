use std::{
    slice::{
        self,
    },
};

pub fn stdslice() {
    slice_from_raw_parts_mut();
    slice_chunks();
    slice_split();
    
}

//slice::from_raw_parts_mut()
fn slice_from_raw_parts_mut() {

    let mut v = vec![65, 122, 66, 67];
    let s = unsafe {
        slice::from_raw_parts_mut(v.as_mut_ptr(), v.len())
    };

    assert_eq!(v.len(), s.len());
    for (k, v) in v.iter().enumerate() {
        assert_eq!(s[k], *v);
    }
    
}


//slice:split()
fn slice_split() {

    //Split: Find the first data that is true, truncate it, and return the following data;
    let slice = [10, 40, 33, 20];
    let mut iter = slice.split(|num| num % 3 == 0);

    assert!(iter.next().is_some());
    assert_eq!(iter.as_slice(), &[20]);
}

//slice:chunks()
fn slice_chunks() {
    //chunks: Split the array in steps of N
    let s1 = ['l', 'o', 'r', 'e', 'm'];
        let iter1 = s1.chunks(2);
        for _c in iter1 {
            //println!("{:?}", c);
        }

        /*  print:
            ['l', 'o']
            ['r', 'e']
            ['m']
         */

        let iter2 = s1.chunks_exact(2);
        assert_eq!(iter2.remainder(), &['m'][..]);
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn tetst_slice() {
        slice_split();
        slice_chunks();
        slice_from_raw_parts_mut();
    }
}
