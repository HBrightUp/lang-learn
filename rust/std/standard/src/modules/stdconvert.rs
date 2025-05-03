
use std::{
    str::{
        FromStr,
    },
};

use crate::include::{
    person::{
        Person,
    },
};


use crate::include::docment::*;

pub fn stdconvert() {

    let mut v: Vec<u8> = vec![1, 2, 3];
    caesar(&mut v, 5);
    assert_eq!(v, [6, 7, 8]);
    null_terminate(&mut v);
    assert_eq!(v, [6, 7, 8 , 0]);

    let mut doc = Docment{
        info: String::from("example"),
        content: vec![17, 19, 8],
    };
    
    // Call AsMut<T>
    caesar(&mut doc, 1);
    assert_eq!(doc.content, [18, 20, 9]);

    // Call AsMut<T>
    null_terminate(&mut doc);
    assert_eq!(doc.content, [18, 20, 9, 0]);

    // Call From<String> trait，From<String>
    let d2 = Docment::from("Rust".to_string());
    assert_eq!(d2.info, String::from("Rust"));
    
    // Call From<Vec<u8>> , Call into trait
    let v2 = vec![1, 2, 3, 4];
    let d3 : Docment = v2.into();
    assert_eq!(d3.content, [1, 2, 3, 4]);

    // Call FromStr trait
    let d4 : Docment = Docment::from_str("Rust").unwrap();
    assert_eq!(d4.info, "Rust".to_string());

    //d4: info: Rust, content: true

    // Docment  -> String
    assert_eq!(d4.to_string(), String::from("info: Rust, content: true"));
    
    let p = Person {
        id: 100,
        name: "rust".to_string(),
        age: 19,
    };

    let d5: Docment = Docment::from(&p);
    assert_eq!(d5.info, p.name);

    let d6 = Docment::default();
    assert_eq!(d6.info, "default".to_string());

}

//Vec -> [u8]: 
//As a parameter , Vec can be automatically converted into [T] ;
fn caesar<T: AsMut<[u8]>>(data: &mut T, key: u8) {
    for byte in data.as_mut() {
        *byte = byte.wrapping_add(key);
    }
}

// add '0' to tail;
fn null_terminate<T: AsMut<Vec<u8>>>(data: &mut T) {
    fn doit(data: &mut Vec<u8>) {
        let len = data.len();
        if len == 0 || data[len - 1] != 0 {
            data.push(0);
        } 
    }
    doit(data.as_mut());
}

