use std::{
    alloc::{
        self, Layout,
    },
    ptr::{
        self,NonNull,
    },
};


pub fn stdalloc() {
    alloc_relloc();
}

// alloc::alloc() && alloc::realloc()
pub fn alloc_relloc() {
    let  init_cap = 3;
    let layout = Layout::array::<i32>(init_cap).unwrap();
    let ptr = unsafe { alloc::alloc(layout) };
    let p1 = NonNull::new(ptr as *mut i32).unwrap();
    unsafe {  
        ptr::write(p1.as_ptr(), 10); 
        ptr::write(p1.as_ptr().add(1), 20); 
        ptr::write(p1.as_ptr().add(2), 30); 

        //Test: When index is equal to 33084, an error message appears: "Segmentation fault (core dumped)"
        // for index in 30000..35000 {
        //     println!("{:?}", index);
        //     ptr::write(p1.as_ptr().add(index), 40); 
        // }
        
        ptr::write(p1.as_ptr().add(100), 40); 

        let p_dangling = p1.as_ptr() as *mut i32;
        println!("alloc_relloc() old p_dangling: {:?}", p_dangling);

        assert_eq!(*p_dangling, 10);
        assert_eq!(*(p_dangling.wrapping_add(1)), 20);
        assert_eq!(*p_dangling.add(2), 30);
        assert_eq!(*(p_dangling.wrapping_add(3)), 0);

        // assert failed
        //assert_eq!(*p_dangling.add(4), 0);
        
    }

    // Reallocate memory and copy old data
    let  new_cap = init_cap * 2 + 1;
    let old_layout = Layout::array::<i32>(init_cap).unwrap();
    let old_ptr = p1.as_ptr() as *mut u8;
    let new_layout = Layout::array::<i32>(new_cap).unwrap();
    let new_ptr = unsafe{
        alloc::realloc(old_ptr, old_layout, new_layout.size())
    };

    let  p2 = match NonNull::new(new_ptr as *mut i32) {
        Some(p) => p,
        None => alloc::handle_alloc_error(new_layout),
    };
    
    
    unsafe {
        let p_dangling = p2.as_ptr() as *mut i32;
        println!("alloc_relloc() new p_dangling: {:?}", p_dangling);

        assert_eq!(*p_dangling, 10);
        assert_eq!(*(p_dangling.wrapping_add(2)), 30);

        ptr::write(p2.as_ptr().add(1), 300);
        assert_eq!(*(p_dangling.wrapping_add(1)), 300);

        assert_eq!(*(p_dangling.wrapping_add(5)), 0);
        ptr::write(p2.as_ptr().add(6), 888);
        assert_eq!(*(p_dangling.wrapping_add(6)), 888);

        //assert failed
        //assert_eq!(*(p_dangling.wrapping_add(100)), 40);
    }

    /*  When debugging, I found that the memory addresses allocated twice are always the same
    alloc_relloc() old p_dangling: 0x5b05e7c67b10
    alloc_relloc() new p_dangling: 0x5b05e7c67b10
    */

    unsafe {
        alloc::dealloc(
            p2.as_ptr() as *mut u8,
            Layout::array::<i32>(new_cap).unwrap(),
        );

        let p_dangling = p2.as_ptr() as *mut i32;
        println!("alloc_relloc() new p_dangling: {:?}", p_dangling);

        // assert failed because p2 wss dropped.
        //assert_eq!(*p_dangling, 10);
    }

}

#[cfg(test)] 
mod test {
    use super::*;
    #[test] 
    fn test_alloc() {
        alloc_relloc();
    }
}