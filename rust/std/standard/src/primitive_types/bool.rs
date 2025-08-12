






#[cfg(test)]
mod test {

    #[test]
    fn test_then() {

        assert_eq!(false.then(|| 0), None);
        assert_eq!(true.then(|| 0), Some(0));
        
        let mut a = 20;
        let b = 2;

        (a > b).then( || {
            a += 1;
        });

        assert_eq!(a,21);
    }
}