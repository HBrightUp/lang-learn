
pub fn stdoption() {
    option_map();
}


// map
fn option_map() {

    let val : Option<i32> = Some(3);
    let new_val = val.map( |val| {
        val * 2
    });
    assert_eq!(new_val, Some(6));
}



#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn test_ptr() {
        option_map();
    }
}