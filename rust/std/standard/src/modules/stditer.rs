


pub fn stditer() {

}



#[cfg(test)] 
mod test {
    //use super::*;

    #[test] 
    fn test_size_hint() {
        let a = vec![10, 23, 45, 88, 90, 3];
        let mut iter = a.iter();

        assert_eq!(iter.size_hint(), (6, Some(6)));
        let _ = iter.next();
        assert_eq!(iter.size_hint(), (5, Some(5)));
    }

    //iter::zip()   iter::next()
    #[test]
    fn test_zip() {
        let a1 = [1, 2, 3];
        let a2 = [4, 5, 6];
        let mut iter = std::iter::zip(a1, a2);

        assert_eq!(iter.next().unwrap(), (1, 4));
        assert_eq!(iter.next().unwrap(), (2, 5));
        assert_eq!(iter.next().unwrap(), (3, 6));
        assert!(iter.next().is_none());
    }

    //iter::from_fn()
    #[test]
    fn from_fn() {
        let mut count = 0;
        let counter = std::iter::from_fn( move || {
            count += 1;
            if count < 6 {
                Some(count)
            } else {
                None
            }
        });

        assert_eq!(counter.collect::<Vec<_>>(), &[1, 2, 3, 4, 5]);
    }
    
    
    
}