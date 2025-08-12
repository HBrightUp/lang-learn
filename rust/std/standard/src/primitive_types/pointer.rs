
pub fn stdpointer() {

}







#[cfg(test)]
mod test {
    //use super::*;

    #[test]
    fn test_pointer() {

    }

    //pointer::offset()
    #[test]
    fn test_offset() {
        let s: &str = "123";
        let ptr: *const u8 = s.as_ptr();

        unsafe {
            assert_eq!(*ptr.offset(1) as char, '2');
            assert_eq!(*ptr.offset(2) as char, '3');
        }
    }
}