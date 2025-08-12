
pub fn add<T>(left: T, right: T) -> T
where
T: std::ops::Add<Output = T>
 {
    left + right
}

pub fn sub<T: std::ops::Sub<Output = T>>(left: T, right: T) -> T{
    left - right
}






#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn it_works() {
        let result = add(2, 2);
        assert_eq!(result, 4);
    }
}
