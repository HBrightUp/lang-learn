use std::{
    hash::{
        DefaultHasher,Hash,Hasher,
    },
};

use crate::include::person::*;

pub fn stdhash() {
    let s1 = Person {
        id: 5,
        name: "Janet".to_string(),
        age: 10,
    };
    let s2 = Person {
        id: 5,
        name: "Bob".to_string(),
        age: 12,
    };
    
    assert!(calculate_hash(&s1) != calculate_hash(&s2));
}

fn calculate_hash<T: Hash>(t: &T) -> u64 {
    let mut s = DefaultHasher::new();
    t.hash(&mut s);
    s.finish()
}
