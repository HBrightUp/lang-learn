

pub fn stdarray() {

}


#[cfg(test)]
mod test {

    // str -> [Char]
    // array::as_ascii()
    #[test]
    fn test_as_ascii() {
        const HEX_DIGITS: [std::ascii::Char; 16] = *b"0123456789abcdef".as_ascii().unwrap();
        assert_eq!(HEX_DIGITS[1].as_str(), "1");
        assert_eq!(HEX_DIGITS[10].as_str(), "a");
    }

    #[test]
    fn test_as_mut_slice() {
        let mut arr = [12, 34, 53, 32];

        let slice = arr.as_mut_slice();
        assert_eq!(slice[2], 53);
    }

    #[test]
    fn test_each_mut() {
        let mut floats = [3.1, 2.7, -1.0];
        let float_refs: [&mut f64; 3] = floats.each_mut();
        *float_refs[0] = 0.0;
        assert_eq!(float_refs, [&mut 0.0, &mut 2.7, &mut -1.0]);
        assert_eq!(floats, [0.0, 2.7, -1.0]);
    }

    #[test]
    fn test_map() {
        let x = ["Ferris", "Bueller's", "Day", "Off"];
        let y = x.map(|v| v.len());
        assert_eq!(y, [6, 9, 3, 3]);
    }

    #[test]
    fn test_split_array_mut() {
        let mut v = [1, 0, 3, 0, 5, 6];
        let (left, right) = v.split_array_mut::<2>();
        assert_eq!(left, &mut [1, 0][..]);
        assert_eq!(right, &mut [3, 0, 5, 6]);
        left[1] = 2;
        right[1] = 4;
        assert_eq!(v, [1, 2, 3, 4, 5, 6]);
    }

    #[test]
    fn test_try_map() {
        
        let a = ["1", "2", "3"];
        let b = a.try_map(|v| v.parse::<u32>()).unwrap().map(|v| v + 1);
        assert_eq!(b, [2, 3, 4]);
    }
}