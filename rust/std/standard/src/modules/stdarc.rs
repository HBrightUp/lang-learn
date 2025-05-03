use std::{
    sync::{
        Arc, Mutex,
    },
    cell::{
        RefCell,
    }
};


pub fn stdarc() {

}


#[cfg(test)]
mod test {

    use super::*;

    #[test]
    fn test_arc() {
        let p1 = Arc::new(Mutex::new(RefCell::new(5)));
        let p2 = p1.lock().unwrap();
        let value = p2.clone().into_inner();    
        assert_eq!(value, 5);
    }
}
