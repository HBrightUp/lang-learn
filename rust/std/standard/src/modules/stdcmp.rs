use std::{
    cmp::{
        max,
    },
};
use crate::include::person::{
    Person,
};



pub fn stdcmp() {
    
    sort_person();
}

// PartialOrd PartialEq cmp::max()
//Implementing custom data comparison and sorting
fn sort_person() {
    let mut v = vec![
        Person {
            id: 3,
            name: "3-name".to_string(),
            age:18,
        },
        Person {
            id: 2,
            name: "2-name".to_string(),
            age: 12,
        },
        Person {
            id: 1,
            name: "1-name".to_string(),
            age:19,
        },
        Person {
            id: 2,
            name: "2-name-copy".to_string(),
            age:17,
        },
    ];

    // Call PartialOrd
    assert_eq!(v[0] < v[1], false);

    // Call PartialEq
    assert_eq!(v[1] == v[3], true);

    let v_max = max(&v[1], &v[2]);
    assert_eq!(v_max.id, 1);

    // Call PartialOrd 
    v.sort();   
        
    // check
    let mut age = v.get(0).unwrap().age;
    for p in v.iter() {
        assert!(p.age >= age);
        age = p.age;
    }
    
}

#[cfg(test)] 
mod test {
   use super::*;

   #[test] 
   fn test_cmp() {
        sort_person();
   }

 
}


