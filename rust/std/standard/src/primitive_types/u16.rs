

pub fn stdu16() {

}


#[cfg(test)]
mod test {
    //se super::*;


    //u16::from_le_bytes()
    #[test]
    fn test_from() {
        let value = u16::from_le_bytes([0x34, 0x12]);
        assert_eq!(value, 0x1234);
    }
}