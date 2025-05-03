
//warn: Just for demonstration, in this case Box::leak is incorrect to use
struct AppConfig{
    file_suffix: Vec<String>,
}


pub fn stdboxed() {
    boxed_as_ptr();
    boxed_raw();

    let config = get_config();
    assert_eq!(config.file_suffix, &["rs".to_string(), "md".to_string()]);
}


// Box<T> -> * mut T   &&  * mut T -> Box<T>
// new && into_raw && from_raw
fn boxed_raw() {
    let b = Box::new(10);
    let  ptr = Box::into_raw(b);
    unsafe { 
        *ptr = 100; 
        assert_eq!(*ptr, 100);
        drop(Box::from_raw(ptr));
        
        // The memory pointed to by 'ptr' has been manually drop and is not allowed to be used again
        //assert_eq!(*ptr, 100);
    }
}

// as_ptr  && as_mut_ptr
fn boxed_as_ptr() {
    let mut b = Box::new(10);

    let p1 = Box::as_ptr(&b);
    let p2 = Box::as_mut_ptr(&mut b);

    unsafe {
        let v1 = p1.read();
        let v2 = p2.read();
        assert_eq!(v1, v2);

        p2.write(12);

        let v3 = p1.read();
        assert_eq!(v3, 12);

    }
}

// Warning: Using Box::leak() to load the configuration file is just to show its use, but it is not actually using action;
// Warning: This function will cause a memory leak once it is called.
fn get_config() ->&'static mut AppConfig {

    let config = Box::new(AppConfig{
        file_suffix: vec!["rs".to_string(), "md".to_string()],
    });

    Box::leak(config)
}


#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn test_ptr() {
        boxed_raw();
        boxed_as_ptr();
    }

    #[test] 
    fn test_leak() {
        let _config = get_config();
    }
}