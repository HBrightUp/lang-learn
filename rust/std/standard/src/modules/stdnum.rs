use std::{
    num::{
        Wrapping,NonZeroU64
    },
};

pub fn stdnum() {

    //NonZeroU64:
    {   
        // Binary of 6 : 0000000000000000000000000000000000000000000000000000000000000110
        if let Some(data) = std::num::NonZeroU64::new(6u64) {
            assert_eq!(data.get(), 6);
            assert_eq!(data.leading_zeros(),61);
            assert_eq!(data.trailing_zeros(), 1);
        };

        //checked_add: check overload
        {
            let one = NonZeroU64::new(1).unwrap();
            let two = NonZeroU64::new(2).unwrap();
            let max = NonZeroU64::new(u64::MAX).unwrap();

            assert_eq!(Some(two), one.checked_add(1));
            assert_eq!(None, max.checked_add(1));
        }

        // log2
        assert_eq!(NonZeroU64::new(9).unwrap().ilog2(), 3);

        //lg
        assert_eq!(NonZeroU64::new(100).unwrap().ilog10(), 2);

        //(a + b) >> 1
        {
            let one = NonZeroU64::new(1).unwrap();
            let two = NonZeroU64::new(2).unwrap();
            let four = NonZeroU64::new(4).unwrap();

            assert_eq!(one.midpoint(four), two);
            assert_eq!(four.midpoint(one), two);
        }

        assert_eq!(NonZeroU64::MIN.get(), 1u64);
        assert_eq!(NonZeroU64::MAX.get(), u64::MAX);
        assert_eq!(NonZeroU64::BITS, u64::BITS);  // 64u32
    }

    //wrapping: 
    {
        let zero = Wrapping(0u32);
        let one = Wrapping(1u32);

        assert_eq!(u32::MAX, (zero - one).0);
    }

}


